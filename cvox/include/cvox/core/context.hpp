#pragma once
#include "core.hpp"
#include "namespaced_id.hpp"
#include <entt/entt.hpp>

namespace cvox
{

class Engine;
class CVOX_API Context
{
public:
    Context(Engine& engine, NamespaceId name);

    entt::entity
    entity() const noexcept
    {
        return m_entity;
    }
    NamespaceId
    name() const noexcept
    {
        return m_name;
    }
    Engine&
    engine() const noexcept
    {
        return m_engine;
    }

private:
    entt::entity m_entity;
    NamespaceId m_name;
    Engine& m_engine;
};
} // namespace cvox