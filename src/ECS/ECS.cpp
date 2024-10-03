#include "ECS.h"
#include "Logger/Logger.h"

// TODO: there is a bug here where NumEntities is being initialized with 1 somehow
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
	for (int i = 0; i < ComponentPools.size(); i++)
	{
		delete ComponentPools[i]; // IPool*
	}

	for (const auto& pair : Systems)
	{
		delete pair.second; // System*
	}
}

Entity ECSManager::CreateEntity()
{
	Entity e;
	EntitiesToBeAdded.insert(e.GetID());

	++NumEntities;

	return e;
}

void ECSManager::DestroyEntity(Entity InEntity)
{
	EntitiesToBeRemoved.insert(InEntity.GetID());
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
	for (const unsigned int next : EntitiesToBeAdded)
	{
		
	}
	for (const unsigned int next : EntitiesToBeRemoved)
	{

	}
}
