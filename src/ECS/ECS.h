#pragma once

#include "Logger/Logger.h"
#include "Util/CoreStatics.h"

// Unfortunately all these things have to be included here as we are using typedefs and
// smart pointers and things, and this file contains template implementations
#include <bitset>
#include <vector>
#include <assert.h>
#include <unordered_map>
#include <typeindex>
#include <set>
#include <memory>

/**
 * Using a bitset of MaxComponents size allows us to flag the presence of the component
 * with ID matching the index of that bit in the signature.
 * 
 * TODO: rather than having a maximum number of components restricted by the size of
 * a single data type, could refactor to use data structures as signatures instead. 
 * That way we don't have a restriction on the number of components we can write.
 */
typedef std::bitset<CoreStatics::MaxNumComponents> Signature;

/**
 * Entity class. Basically just an ID.
 * We will pass systems COPIES of this class to store rather than pointers, since
 * the class itself is just a wrapper around plain old data.
 * Holds the current number of entities as well as the max number (size of uint32)
 * which also serves as the NullID.
 */
class Entity
{
public:
	Entity(class ECSManager* Owner) : EntityID(NumEntities++), Owner(std::make_shared<ECSManager>(*Owner))
	{ 
		assert(NumEntities < CoreStatics::MaxNumEntities); 
	}

	const unsigned int GetID() const { return EntityID; }

	Entity& operator =(const Entity& Other) = default;
	bool operator==(const Entity& Other) const { return EntityID == Other.GetID(); }
	bool operator<(const Entity& Other) const { return EntityID < Other.GetID(); }
	bool operator>(const Entity& Other) const { return EntityID > Other.GetID(); }

	// Adding these wrappers to simply forward to the Owner for readability in game code
	template <typename TComponent, typename ...TArgs>
	void AddComponent(TArgs&& ...Args);

	template <typename TComponent>
	void RemoveComponent();

	template <typename TComponent>
	const bool HasComponent() const;

	template <typename TComponent>
	TComponent& GetComponent() const;

protected:
	static unsigned int NumEntities; // = 0;

private:
	unsigned int EntityID;
	std::shared_ptr<ECSManager> Owner;
	//ECSManager* Owner; // Does this need to be a C ptr??
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
 * NumComponents must live in IComponent because Component is templated.
 * 
 * This is why we have to have this weird GetID declaration of the ComponentID
 * for each templated TComponent. Putting the ID assignment in the constructor
 * throws unresolved external symbol.
 * 
 * We need templated Components in order that each type can naively have an ID
 * (whereas we don't need to do this for entities since there are not multiple
 * types of Entities, or for Systems because we are using type_index as keys there)
 * 
 * Possibly this could be refactored to work similarly to Systems in the future,
 * but this implementation is taken from the course and so it's what I'm going with 
 * for now.
 */
 template <typename TComponent>
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
	template <typename TComponent>
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
 * Pool of some object. Can be Entity, Component, etc.
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
	ECSManager();
	~ECSManager();

	void Update(const double DeltaTime);

	////////////////////////////////////////////////////////////////////////////////
	// Entity Management

	Entity CreateEntity();
	void DestroyEntity(const Entity InEntity);

	unsigned int NumEntities = 0;

	////////////////////////////////////////////////////////////////////////////////
	// Component Management

	/**
	 * Optional params after InEntity are forwarded to the constructor of TComponent.
	 * Make sure they are present and correct if TComponent's constructor
	 * does not have defaults!
	 */
	template <typename TComponent, typename ...TArgs>
	void AddComponent(Entity InEntity, TArgs&& ...Args);

	template <typename TComponent>
	const bool HasComponent(const Entity InEntity) const;
	
	template <typename TComponent>
	void RemoveComponent(Entity InEntity);

	template <typename TComponent>
	TComponent& GetComponent(const Entity InEntity);

	////////////////////////////////////////////////////////////////////////////////
	// System Management

	/**
	 * Optional params are forwarded to the constructor of TSystem.
	 * Make sure they are present and correct if TSystem's constructor 
	 * does not have defaults!
	 */
	template <typename TSystem, typename ...TArgs>
	void AddSystem(TArgs&& ...Args);

	template <typename TSystem>
	void RemoveSystem();

	template <typename TSystem>
	const bool HasSystem() const;

	template <typename TSystem>
	TSystem& GetSystem() const;

private:
	void AddEntityToSystems(const Entity InEntity);

	/**
	 * Index indicates ComponentID, value is that component's pool (of entities with that component)
	 */
	std::vector<std::shared_ptr<IPool>> ComponentPools;

	/**
	 * Index indicates EntityID, value is that entity's signature
	 */
	std::vector<Signature> EntityComponentSignatures;

	/**
	 * System map doesn't need to be ordered, since systems have no ID.
	 */
	 std::unordered_map<std::type_index, std::shared_ptr<System>> Systems;

	/** 
	 * Entities flagged to be added or removed in the next Update() call 
	 */
	std::set<Entity> EntitiesToBeAdded;
	std::set<Entity> EntitiesToBeRemoved;
};

template <typename TComponent, typename ...TArgs>
void ECSManager::AddComponent(Entity InEntity, TArgs&& ...Args)
{
	const auto entityID = InEntity.GetID();
	const auto componentID = Component<TComponent>::GetID();

	// Bounds check on the array of pools, allocate nullptrs as needed
	if (componentID >= ComponentPools.size())
	{
		ComponentPools.resize(componentID + 1, nullptr);
	}

	// If we needed to add nullptrs, allocate a new Pool and store it
	if (ComponentPools[componentID] == nullptr)
	{
		ComponentPools[componentID] = std::make_shared<Pool<TComponent>>();
	}

	std::shared_ptr<Pool<TComponent>> componentPool = static_pointer_cast<Pool<TComponent>>(
		ComponentPools[componentID]
	);

	// Bounds check on this particular pool
	if (entityID >= (unsigned)componentPool->Size()) // blah warning blah
	{
		componentPool->Resize(entityID + 1);
	}

	// Make a new component to assign to the entity, forwarding constructor args if they are present
	TComponent newComponent(std::forward<TArgs>(Args)...);

	// Assign it to the entity (in this component type's pool, at this entity ID's index)
	componentPool->Insert(entityID, newComponent);

	if (entityID >= EntityComponentSignatures.size())
	{
		EntityComponentSignatures.resize(entityID + 1);
	}

	// Update the entity's signature to indicate that this component is assigned to this entity
	EntityComponentSignatures[entityID].set(componentID);

	Logger::LogMessage(
		"Component " + std::to_string(componentID) + 
		" added to entity " + std::to_string(entityID));
}

template <typename TComponent>
const bool ECSManager::HasComponent(const Entity InEntity) const
{
	assert(InEntity.GetID() < EntityComponentSignatures.size());
	return EntityComponentSignatures[InEntity.GetID()].test(Component<TComponent>::GetID());
}

template <typename TComponent>
void ECSManager::RemoveComponent(Entity InEntity)
{
	assert(InEntity.GetID() < EntityComponentSignatures.size());
	EntityComponentSignatures[InEntity.GetID()].set(Component<TComponent>::GetID(), false);
}

template <typename TComponent>
TComponent& ECSManager::GetComponent(const Entity InEntity)
{
	assert(HasComponent<TComponent>(InEntity));
	const auto entityId = InEntity.GetID();
	const auto* componentPool = ComponentPools[Component<TComponent>::GetID()];
	return *componentPool[entityId];
}

template <typename TSystem, typename ...TArgs>
void ECSManager::AddSystem(TArgs&& ...Args)
{
	const auto systemIdx = std::type_index(typeid(TSystem));

	if (Systems.count(systemIdx) == 0)
	{
		std::shared_ptr<TSystem> newSystem = std::make_unique<TSystem>(std::forward<TArgs>(Args)...);
		Systems[systemIdx] = newSystem;
	}
}

template <typename TSystem>
void ECSManager::RemoveSystem()
{
	const auto systemIdx = std::type_index(typeid(TSystem));
	
	if (Systems.count(systemIdx))
	{
		Systems.erase(systemIdx);
	}
}

template <typename TSystem>
const bool ECSManager::HasSystem() const
{
	const auto systemIdx = std::type_index(typeid(TSystem));
	return Systems.count(systemIdx);
}

template <typename TSystem>
TSystem& ECSManager::GetSystem() const
{
	const auto systemIdx = std::type_index(typeid(TSystem));

	if (Systems.count(systemIdx) && Systems[systemIdx] != nullptr)
	{
		return *static_ptr_cast<TSystem>(Systems[systemIdx]);
	}
}

template <typename TComponent>
void System::RequireComponent()
{
	ComponentSignature.set(Component<TComponent>::GetID());
}


template <typename TComponent, typename ...TArgs>
void Entity::AddComponent(TArgs&& ...Args)
{
	Owner->AddComponent<TComponent>(*this, std::forward<TArgs>(Args)...);
}

template <typename TComponent>
void Entity::RemoveComponent()
{
	Owner->RemoveComponent<TComponent>(*this);
}

template <typename TComponent>
const bool Entity::HasComponent() const
{
	return Owner->HasComponent<TComponent>(*this);
}

template <typename TComponent>
TComponent& Entity::GetComponent() const
{
	return Owner->GetComponent<TComponent>(*this);
}