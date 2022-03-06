// This header provides a base class that the other systems in the E-C System
// inherit from. This essentially just provides a standardized way to provide entities.
#pragma once

// std includes
#include <set>

// Project includes
#include "ge_component_manager.hpp"

namespace ge
{

class GeSystem
{
    public:
        std::set<Entity> mEntities;
};

}