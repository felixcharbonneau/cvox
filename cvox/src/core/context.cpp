#include "cvox/core/context.hpp"
#include "cvox/core/engine.hpp"

namespace cvox
{
Context::Context(Engine& engine, NamespaceId name)
    : m_engine(engine), m_entity(engine.registry().create()), m_name(name)
{
}

} // namespace cvox