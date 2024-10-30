/**
 * Copyright (C) 2024 Sean Goldie. All rights reserved.
 * Contact: sean.writes.code@gmail.com
 */

#pragma once

#include "Logger/Logger.h"
#include "Util/CoreStatics.h"
#include <vector>
#include <cassert>
#include <unordered_map>
#include <typeindex>
#include <set>
#include <bitset>
#include <unordered_set>
#include <queue>

typedef std::bitset<CoreStatics::MaxNumComponentTypes> Signature;

/**
 * Entity class. Basically just an ID.
 * We will generally pass around COPIES of this class to store rather than pointers, since
 * the class itself is just a wrapper around plain old data and it wouldn't be meaningfully
 * more efficient to allocate a pointer or reference to it.
 * Holds the current number of entities as well as the max number (32)
 * which also serves as the NullID.
 */
class Entity
{
public:
    Entity(class ECSManager* Owner = nullptr);

    const unsigned int GetID() const { return EntityID; }

    void Kill() const;

    Entity& operator =(const Entity& Other) = default;
    bool operator==(const Entity& Other) const { return EntityID == Other.GetID(); }
    bool operator<(const Entity& Other) const { return EntityID < Other.GetID(); }
    bool operator>(const Entity& Other) const { return EntityID > Other.GetID(); }

    /**
     * Optional params are forwarded to the constructor of TComponent.
     */
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
    unsigned int EntityID = 0;
    ECSManager* Owner = nullptr;
};

/**
 * Interface for all components that just keeps track of the current
 * NumComponentTypes to be assigned out as IDs. 
 * 
 * Possible TODO: data structure note above
 */
class IComponent
{
protected:
    static unsigned int NumComponentTypes; // = 0;
};

/**
 * Component with a specific ID
 * NumComponents must live in IComponent because Component are templates.
 * 
 * This is why we have to have this weird GetID declaration of the ComponentID
 * for each template TComponent. Putting the ID assignment in the constructor
 * throws unresolved external symbol.
 * 
 * We need template Components in order that each type can naively have an ID
 * (whereas we don't need to do this for entities since there are not multiple
 * types of Entities, or for Systems because we do not need a System ID)
 * 
 * Can't use a type_index map like Systems because we are currently using the ComponentID
 * as a bitset in the signature. Refactoring the whole ECS framework to use an unlimited
 * number of components by having a signature as a data structure rather than a single
 * data type may allow for revisiting all of this in the future.
 */
 template <typename TComponent>
class Component : IComponent
{
public:
    Component() 
    {
        assert(NumComponentTypes + 1 < CoreStatics::MaxNumComponentTypes); 
    }

    static unsigned int GetID()
    { 
        static auto ComponentID = NumComponentTypes++;
        return ComponentID;
    }
};

/**
 * A system to handle a specific component signature
 */
class System
{
public:
    virtual void AddEntity(const Entity EntityToAdd);
    void RemoveEntity(const Entity EntityToRemove);

    std::vector<Entity>& GetEntities() { return Entities; }
    const Signature& GetComponentSignature() const { return ComponentSignature; }

    virtual void Update(const float DeltaTime) = 0;

protected:
    template <typename TComponent>
    void RequireComponent();

    Signature ComponentSignature;
    std::vector<Entity> Entities;
    std::unordered_set<unsigned int> EntityIDs;
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
    ECSManager() = default;
    ~ECSManager();

    void Update(const float DeltaTime);

    ////////////////////////////////////////////////////////////////////////////////
    // Entity Management

    Entity CreateEntity();
    void DestroyEntity(const Entity InEntity);
    std::queue<unsigned int>& GetFreeEntityIDs() { return FreeEntityIDs; }

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
     */
    template <typename TSystem, typename ...TArgs>
    void AddSystem(TArgs&& ...Args);

    template <typename TSystem>
    void RemoveSystem();

    template <typename TSystem>
    const bool HasSystem() const;

    template <typename TSystem>
    TSystem* GetSystem() const;

private:
    void AddEntityToSystems(const Entity InEntity);
    void RemoveEntityFromSystems(const Entity InEntity);
    void UpdateEntityInSystems(const Entity InEntity, const Signature& Old, const Signature& New);

    /**
     * Index indicates ComponentID, value is that component's pool (of entities with that component)
     */
    std::vector<IPool*> ComponentPools;

    /**
     * Index indicates EntityID, value is that entity's signature
     */
    std::vector<Signature> EntityComponentSignatures;

    /**
     * TODO: refactor to use vector, not sure this implementation makes sense
     */
     std::unordered_map<std::type_index, System*> Systems;

    /** 
     * Entities flagged to be added or removed in the next Update() call 
     */
    std::set<Entity> EntitiesToBeAdded;
    std::set<Entity> EntitiesToBeRemoved;

    /**
     * Entity IDs of destroyed entities that can now be reused
     */
     std::queue<unsigned int> FreeEntityIDs;
};

template <typename TComponent, typename ...TArgs>
void ECSManager::AddComponent(Entity InEntity, TArgs&& ...Args)
{
    const auto entityID = InEntity.GetID();
    const auto componentID = Component<TComponent>::GetID();

    // Bounds check on the array of pools, allocate nullptrs as needed
    if (componentID >= ComponentPools.size())
    {
        const auto newSize = (ComponentPools.size() > 0) ? ComponentPools.size() * 2 : 32;
        ComponentPools.resize(newSize, nullptr);
    }

    // If we needed to add nullptrs, allocate a new Pool and store it
    if (ComponentPools[componentID] == nullptr)
    {
        ComponentPools[componentID] = new Pool<TComponent>();
    }

    Pool<TComponent>* componentPool = static_cast<Pool<TComponent>*>((ComponentPools[componentID]));

    // Bounds check on this particular pool
    if (entityID >= static_cast<unsigned int>(componentPool->Size())) // blah warning blah
    {
        if (entityID > 0)
        {
            componentPool->Resize(entityID * 2);
        }
        else
        {
            componentPool->Resize(32);
        }
    }

    // Make a new component to assign to the entity, forwarding constructor args if they are present
    TComponent newComponent(std::forward<TArgs>(Args)...);

    // Assign it to the entity (in this component type's pool, at this entity ID's index)
    componentPool->Insert(entityID, newComponent);

    if (entityID >= EntityComponentSignatures.size())
    {
        if (entityID > 0)
        {
            EntityComponentSignatures.resize(entityID * 2);
        }
        else
        {
            EntityComponentSignatures.resize(32);
        }
    }

    // Capture the old signature
    const Signature oldEntitySignature = EntityComponentSignatures[entityID];

    // Update the entity's signature to indicate that this component is assigned to this entity
    const Signature newEntitySignature = EntityComponentSignatures[entityID].set(componentID);

    UpdateEntityInSystems(InEntity, oldEntitySignature, newEntitySignature);
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

    const auto entityID = InEntity.GetID();
    const Signature oldSignature = EntityComponentSignatures[entityID];
    const Signature newSignature = EntityComponentSignatures[entityID].set(Component<TComponent>::GetID(), false);

    UpdateEntityInSystems(InEntity, oldSignature, newSignature);
}

template <typename TComponent>
TComponent& ECSManager::GetComponent(const Entity InEntity)
{
    assert(HasComponent<TComponent>(InEntity));
    const auto entityId = InEntity.GetID();
    const auto componentId = Component<TComponent>::GetID();

    Pool<TComponent>* componentPool = static_cast<Pool<TComponent>*>((ComponentPools[componentId]));

    return componentPool->Get(entityId);
}

template <typename TSystem, typename ...TArgs>
void ECSManager::AddSystem(TArgs&& ...Args)
{
    const auto systemIdx = std::type_index(typeid(TSystem));

    if (Systems.count(systemIdx) == 0)
    {
        Systems[systemIdx] = new TSystem(std::forward<TArgs>(Args)...);
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
TSystem* ECSManager::GetSystem() const
{
    const auto systemIdx = std::type_index(typeid(TSystem));
    const auto systemItr = Systems.find(systemIdx);

    if (systemItr != Systems.end())
    {
        const auto& pair = *systemItr;

        if (pair.second != nullptr)
        {
            return static_cast<TSystem*>(pair.second);
        }
    }
    
    return nullptr;
}

template <typename TComponent>
void System::RequireComponent()
{
    ComponentSignature.set(Component<TComponent>::GetID());
}


template <typename TComponent, typename ...TArgs>
void Entity::AddComponent(TArgs&& ...Args)
{
    assert(Owner != nullptr);
    Owner->AddComponent<TComponent>(*this, std::forward<TArgs>(Args)...);

}

template <typename TComponent>
void Entity::RemoveComponent()
{
    assert(Owner != nullptr);
    Owner->RemoveComponent<TComponent>(*this);
}

template <typename TComponent>
const bool Entity::HasComponent() const
{
    assert(Owner != nullptr);
    return Owner->HasComponent<TComponent>(*this);
}

template <typename TComponent>
TComponent& Entity::GetComponent() const
{
    assert(Owner != nullptr);
    return Owner->GetComponent<TComponent>(*this);
}