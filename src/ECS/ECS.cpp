#include "ECS.h"

unsigned int Entity::NumEntities = 0;
unsigned int IComponent::NumComponentTypes = 0;

void System::AddEntity(const Entity InEntity)
{
	Entities.push_back(InEntity);
}

void System::RemoveEntity(const Entity InEntity)
{
	if (Entities.size() > InEntity.GetID())
	{
		Entities.erase(Entities.begin() + InEntity.GetID());
	}
}

ECSManager::ECSManager()
{

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
		EntityComponentSignatures.resize(NumEntities);
	}

	return entity;
}

void ECSManager::DestroyEntity(Entity InEntity)
{
	EntitiesToBeRemoved.insert(InEntity);
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
		}

		for (const auto& [typeIndex, system] : Systems)
		{
			const Signature& systemSignature = system->GetComponentSignature();
			if ((systemSignature & New) == systemSignature)
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
	}
	EntitiesToBeRemoved.clear();

	for (const auto& system : Systems)
	{
		system.second->Update(DeltaTime);
	}
}
