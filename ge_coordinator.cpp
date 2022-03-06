// Implementation for the Coordinator for the E-C System
// std includes

// Project includes
#include "ge_coordinator.hpp"

namespace ge
{

void GeCoordinator::Init()
{
    // Create pointers to each of the managers for the E-C System
    mComponentManager = std::make_unique<GeComponentManager>();
    mEntityManager = std::make_unique<GeEntityManager>();
    mSystemManager = std::make_unique<GeSystemManager>();
}

// Entity Methods
Entity GeCoordinator::createEntity()
{
    return mEntityManager->createEntity();
}

void GeCoordinator::destroyEntity(Entity entity)
{
    mEntityManager->destroyEntity(entity);
    mComponentManager->entityDestroyed(entity);
    mSystemManager->entityDestroyed(entity);
}

// Component Methods
template<typename T> void GeCoordinator::registerComponent()
{
    mComponentManager->registerComponent<T>();
}

template<typename T> void GeCoordinator::addComponent(Entity entity, T component)
{
    mComponentManager->addComponent<T>(entity, component);

    auto signature = mEntityManager->getSignature(entity);
    signature.set(mComponentManager->getComponentType<T>(), true);
    mEntityManager->setSignature(entity, signature);
}

template<typename T> void GeCoordinator::removeComponent(Entity entity)
{
    mComponentManager->removeComponent<T>(entity);

    auto signature = mEntityManager->getSignature(entity);
    signature.set(mComponentManager->getComponentType<T>(), false);
    mEntityManager->setSignature(entity, signature);

    mSystemManager->entitySignatureChanged(entity, signature);
}

template<typename T> T& GeCoordinator::getComponent(Entity entity)
{
    return mComponentManager->getComponent<T>(entity);
}

template<typename T> ComponentType GeCoordinator::getComponentType()
{
    return mComponentManager->getComponentType<T>();
}

// System Methods
template<typename T> std::shared_ptr<T> GeCoordinator::registerSystem()
{
    return mSystemManager->registerSystem<T>();
}

template<typename T> void GeCoordinator::setSystemSignature(Signature signature)
{
    mSystemManager->setSignature<T>(signature);
}

}