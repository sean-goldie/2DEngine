#include "ECS.h"
#include "Logger/Logger.h"

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
	Entity e;
	EntitiesToBeAdded.insert(e);
	++NumEntities;

	if (e.GetID() >= EntityComponentSignatures.size())
	{
		EntityComponentSignatures.resize(NumEntities);
	}

	return e;
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

void ECSManager::Update()
{
	for (Entity next : EntitiesToBeAdded)
	{
		AddEntityToSystems(next);
	}
	EntitiesToBeAdded.clear();

	for (Entity next : EntitiesToBeRemoved)
	{
		
	}
}
