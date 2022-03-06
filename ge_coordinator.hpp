// This is the header for the coordinator implementation for the E-C System
// This handles the interactions of the different managers implemented
#pragma once
// std includes

// Project includes
#include "ge_system_manager.hpp"

namespace ge
{

class GeCoordinator
{
    public:
        void Init();

        // Entity Methods
        Entity createEntity();
        void destroyEntity(Entity entity);

        // Component Methods
        template<typename T> void registerComponent();
        template<typename T> void addComponent(Entity entity, T component);
        template<typename T> void removeComponent(Entity entity);
        template<typename T> T& getComponent(Entity entity);
        template<typename T> ComponentType getComponentType();

        // System Methods
        template<typename T> std::shared_ptr<T> registerSystem();
        template<typename T> void setSystemSignature(Signature signature);
    private:
        std::unique_ptr<GeComponentManager> mComponentManager;
        std::unique_ptr<GeEntityManager> mEntityManager;
        std::unique_ptr<GeSystemManager> mSystemManager;
};

}
