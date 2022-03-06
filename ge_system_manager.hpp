// Header for the implementation of the System Manager for the E-C System
#pragma once
// std includes

// Project includes
#include "ge_system_base.hpp"

namespace ge
{

class GeSystemManager
{
    public:
        template<typename T> std::shared_ptr<T> registerSystem();

        template<typename T> void setSignature(Signature signature);

        void entityDestroyed(Entity entity);

        void entitySignatureChanged(Entity entity, Signature entitySignature);
    private:
        // Map from system type string pointer to a signature
        std::unordered_map<const char*, Signature> mSignatures{};

        // Map from system type string pointer to a system pointer
        std::unordered_map<const char*, std::shared_ptr<GeSystem>> mSystems{};

};

}