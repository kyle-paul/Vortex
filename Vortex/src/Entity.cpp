#include "Vortex/PreHeaders.h"
#include "Vortex/Scene/Entity.h"

namespace Vortex
{
    Entity::Entity(entt::entity handle, Scene *scene)
        :m_EntityHandle(handle), m_Scene(scene)
    {
        
    }
}