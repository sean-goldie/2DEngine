#include "ECS.h"

unsigned int Entity::NumEntities = 0;
unsigned int IComponent::NumComponents = 0;

void System::AddEntity(Entity InEntity)
{
	Entities.push_back(InEntity);
}

void System::RemoveEntity(Entity InEntity)
{
	Entities.erase(Entities.begin() + InEntity.GetID());
}

ECSManager::ECSManager()
{

}

ECSManager::~ECSManager()
{

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
		if ((system->GetComponentSignature() & entitySignature) == entitySignature)
		{
			system->AddEntity(InEntity);
		}
	}
}

void ECSManager::Update(const double DeltaTime)
{
	for (const Entity& next : EntitiesToBeAdded)
	{
		AddEntityToSystems(next);
	}
	EntitiesToBeAdded.clear();

	for (Entity next : EntitiesToBeRemoved)
	{
		
	}
}
