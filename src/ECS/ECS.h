#pragma once

#include <Util/CoreStatics.h>
#include <bitset>
#include <vector>
#include <assert.h>
#include <unordered_map>
#include <typeindex>
#include <set>

/**
 * Using a bitset of MaxComponents size allows us to flag the presence of the component
 * with ID matching the index of that bit in the signature.
 * 
 * Possible TODO: rather than having a maximum number of components restricted by
 * number of bits in a single data type, could refactor to use a data structure
 * to store them instead
 */
typedef std::bitset<CoreStatics::MaxNumComponents> Signature;

/**
 * Entity class. Just contains an ID.
 * We will pass systems COPIES of this class to store rather than pointers, since
 * the class itself is just a wrapper around plain old data.
 * Holds the current number of entities as well as the max number (size of uint32)
 * which also serves as the NullID.
 */
class Entity
{
public:
	Entity() : EntityID(NumEntities++) { assert(NumEntities < NullID); }
	const unsigned int GetID() const { return EntityID; }

	Entity& operator =(const Entity& Other) = default;
	bool operator==(const Entity& Other) { return EntityID == Other.GetID(); }
	bool operator<(const Entity& Other) { return EntityID < Other.GetID(); }
	bool operator>(const Entity& Other) { return EntityID > Other.GetID(); }
	
	constexpr static unsigned int NullID = -1; // INT_MAX

protected:
	static unsigned int NumEntities; // = 0;

private:
	unsigned int EntityID;
};

/**
 * Abstract interface for all components that just keeps track of the current
 * NumComponents to assign out as IDs. 
 * 
 * Possible TODO: data structure note above
 */
class IComponent
{
protected:
	static unsigned int NumComponents; // = 0;
};

/**
 * Component with a specific ID
 */
 template <typename T>
class Component : IComponent
{
public:
	Component() { assert(NumComponents + 1 < CoreStatics::MaxNumComponents); }

	static unsigned int GetID()
	{ 
		static auto ComponentID = NumComponents++;
		return ComponentID;
	}
};

/**
 * A system to handle a specific component signature
 */
class System
{
public:
	void AddEntity(Entity EntityToAdd);
	void RemoveEntity(Entity EntityToRemove);

	const std::vector<Entity>& GetEntities() const { return Entities; }
	const Signature& GetComponentSignature() const { return ComponentSignature; }

	virtual void Update() = 0;

protected:
	template <typename T>
	void RequireComponent();

private:
	Signature ComponentSignature;
	std::vector<Entity> Entities;
};

/**
 * Abstract interface for pools (so we can keep pointers to non-template IPool)
 */
class IPool
{
public:
	virtual ~IPool() = default;
};

/**
 * 
 */
template <typename T>
class Pool : public IPool
{
public:
	 Pool(const unsigned int size = 100) { Data.resize(size); }
	 virtual ~Pool() = default;

	 const bool Empty() const { return Data.empty(); }
	 const int Size() const { return Data.size(); }
	 void Resize(const unsigned int Size) { Data.resize(Size); }
	 void Clear() { Data.clear(); }
	 void Push(T Object) { Data.push_back(Object); }
	 void Insert(const int Idx, T& Object) { Data.insert(Data.begin() + Idx, Object); }
	 void Erase(const unsigned int Idx) { Data.erase(Idx); }
	 T& Get(const int Idx) { return Data[Idx]; }
	 void operator[](unsigned int Idx) { return Data[Idx]; }

private:
	std::vector<T> Data;
};

/**
 * ECSManager singleton. Manages the life cycles of all ECS objects.
 * The data structures here represent the relationships between entities, components,
 * and systems (e.g. which entities have which components and signatures)
 */
class ECSManager
{
public:
	ECSManager() = default;
	~ECSManager();

	/** Entity stuff */
	Entity CreateEntity();
	void DestroyEntity(const Entity InEntity);

	template <typename T, typename ...TArgs>
	void AddComponent(Entity InEntity, TArgs&& ...Args);

	unsigned int NumEntities = 0;

private:
	void Update();

	/**
	 * Index indicates ComponentID, value is that component's pool (of entities with that component)
	 */
	std::vector<IPool*> ComponentPools;

	/**
	 * Index indicates EntityID, value is that entity's signature
	 */
	std::vector<Signature> EntityComponentSignatures;

	/**
	 * System map doesn't need to be ordered, since systems have no ID.
	 */
	std::unordered_map<std::type_index, System*> Systems;

	/** Entities flagged to be added or removed in the next Update() call */
	std::set<unsigned int> EntitiesToBeAdded;
	std::set<unsigned int> EntitiesToBeRemoved;
};

template <typename T, typename ...TArgs>
void ECSManager::AddComponent(Entity InEntity, TArgs&& ...Args)
{
	const auto ComponentID = Component<T>::GetID();
	const auto EntityID = InEntity.GetID();

	if (ComponentID >= ComponentPools.size())
	{
		ComponentPools.resize(ComponentID + 1, nullptr);
	}

	if (ComponentPools[ComponentID] == nullptr)
	{
		ComponentPools[ComponentID] = new Pool<T>();
	}

	Pool<T>* ComponentPool = static_cast<Pool<T>*>(ComponentPools[ComponentID]);

	if (EntityID >= ComponentPool->Size())
	{
		ComponentPool->Resize(NumEntities);
	}

	T NewComponent(std::forward<TArgs>(Args)...);

	if (EntityID >= ComponentPool->Size())
	{
		ComponentPool->Resize(EntityID + 1);
	}
	ComponentPool->Insert(EntityID, NewComponent);

	EntityComponentSignatures[EntityID].set(ComponentID);
}

template <typename T>
void System::RequireComponent()
{
	ComponentSignature.set(Component<T>::GetID());
}