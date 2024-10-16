/**
 * Copyright (C) 2024 Sean Goldie. All rights reserved.
 * Contact: sean.writes.code@gmail.com
 */

#include "ECS.h"

Entity::Entity(ECSManager* Owner /*= nullptr*/) : Owner(Owner)
{
    assert(++NumEntities < CoreStatics::MaxNumEntities);

    if (Owner != nullptr)
    {
        auto& freeIDs = Owner->GetFreeEntityIDs();
        if (freeIDs.empty() == false)
        {
            EntityID = freeIDs.front();
            freeIDs.pop();
        }
        else
        {
            EntityID = NumEntities;
        }
    }
}

void Entity::Kill() const
{
    if (Owner != nullptr)
    {
        Owner->DestroyEntity(*this);
    }
}

unsigned int Entity::NumEntities = 0;
unsigned int IComponent::NumComponentTypes = 0;

void System::AddEntity(const Entity InEntity)
{
    const auto entityID = InEntity.GetID();

    if (EntityIDs.count(entityID) == 0)
    {
        EntityIDs.insert(entityID);
        Entities.push_back(InEntity);
    }
}

void System::RemoveEntity(const Entity InEntity)
{
    const auto entityID = InEntity.GetID();

    if (EntityIDs.count(entityID))
    {
        EntityIDs.erase(entityID);
        Entities.erase(std::find(Entities.begin(), Entities.end(), InEntity));
    }
}

ECSManager::~ECSManager()
{
    for (IPool* pool : ComponentPools)
    {
        delete pool;
    }

    for (auto& pair : Systems)
    {
        delete pair.second;
    }
}

Entity ECSManager::CreateEntity()
{
    Entity entity(this);
    EntitiesToBeAdded.insert(entity);
    ++NumEntities;

    if (entity.GetID() >= EntityComponentSignatures.size())
    {
        EntityComponentSignatures.resize(entity.GetID() * 2);
    }

    return entity;
}

void ECSManager::DestroyEntity(Entity InEntity)
{
    EntitiesToBeRemoved.insert(InEntity);
    FreeEntityIDs.push(InEntity.GetID());
}

void ECSManager::AddEntityToSystems(const Entity InEntity)
{
    assert(InEntity.GetID() < EntityComponentSignatures.size());
    
    const Signature& entitySignature = EntityComponentSignatures[InEntity.GetID()];

    for (const auto& [typeIndex, system] : Systems)
    {
        const Signature& systemSignature = system->GetComponentSignature();
        if ((systemSignature & entitySignature) == systemSignature)
        {
            system->AddEntity(InEntity);
        }
    }
}

void ECSManager::RemoveEntityFromSystems(const Entity InEntity)
{
    const Signature& entitySignature = EntityComponentSignatures[InEntity.GetID()];

    for (const auto& [typeIndex, system] : Systems)
    {
        const Signature& systemSignature = system->GetComponentSignature();
        if ((systemSignature & entitySignature) == systemSignature)
        {
            system->RemoveEntity(InEntity);
        }
    }
}

void ECSManager::UpdateEntityInSystems(const Entity InEntity, const Signature& Old, const Signature& New)
{
    if (Old != New)
    {
        for (const auto& [typeIndex, system] : Systems)
        {
            const Signature& systemSignature = system->GetComponentSignature();
            if ((systemSignature & Old) == systemSignature)
            {
                system->RemoveEntity(InEntity);
            }
            else if ((systemSignature & New) == systemSignature)
            {
                system->AddEntity(InEntity);
            }
        }
    }
}

void ECSManager::Update(const float DeltaTime)
{
    for (const Entity& entity : EntitiesToBeAdded)
    {
        AddEntityToSystems(entity);
    }
    EntitiesToBeAdded.clear();

    for (const Entity& entity : EntitiesToBeRemoved)
    {
        RemoveEntityFromSystems(entity);
        EntityComponentSignatures[entity.GetID()].reset();
    }
    EntitiesToBeRemoved.clear();

    for (const auto& system : Systems)
    {
        system.second->Update(DeltaTime);
    }
}
