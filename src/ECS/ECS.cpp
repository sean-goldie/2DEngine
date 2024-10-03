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

ECSManager::~ECSManager()
{
	for (unsigned i = 0; i < ComponentPools.size(); i++)
	{
		delete ComponentPools[i];
	}
}

Entity ECSManager::CreateEntity()
{
	Entity E;
	EntitiesToBeAdded.insert(E.GetID());

	++NumEntities;

	Logger::LogMessage("Entity created with id " + std::to_string(E.GetID()));
	Logger::LogMessage("NumEntities is" + std::to_string(NumEntities));

	return E;
}

void ECSManager::DestroyEntity(Entity InEntity)
{
	EntitiesToBeRemoved.insert(InEntity.GetID());
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
