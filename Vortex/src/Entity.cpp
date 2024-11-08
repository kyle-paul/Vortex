#include "Vortex/PreHeaders.h"
#include "Vortex/Scene/Entity.h"
#include "Vortex/Scene/Component.h"

namespace Vortex
{
    Entity::Entity(entt::entity handle, Scene *scene)
        :m_EntityHandle(handle), m_Scene(scene)
    {
        
    }

    UUID Entity::GetUUID()
    {
        return GetComponent<IDComponent>().ID;
    }

    const std::string& Entity::GetName()
    {
        return GetComponent<TagComponent>().Tag;
    }
}
