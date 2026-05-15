#pragma once
#include "application.hpp"
#include "core.hpp"
#include <entt/entt.hpp>
#include <memory>

#include "cvox/rendering/render_context.hpp"
#include "cvox/window/window.hpp"

namespace cvox
{
class CVOX_API Engine
{
public:
    explicit Engine() = default;

    Engine(const Engine&) = delete;
    Engine operator=(const Engine&) = delete;
    Engine(Engine&&) = delete;
    Engine operator=(Engine&&) = delete;

    void init(std::unique_ptr<Application> application);

    void run();

    Window*
    window() const noexcept
    {
        return m_window.get();
    }
    RenderContext*
    render_context() const noexcept
    {
        return m_render_context.get();
    }

    inline entt::registry&
    registry() noexcept
    {
        return m_registry;
    }

private:
    entt::registry m_registry;

    std::unique_ptr<Window> m_window;
    std::unique_ptr<RenderContext> m_render_context;
    std::unique_ptr<Application> m_application;
    bool m_running = false;
};
} // namespace cvox