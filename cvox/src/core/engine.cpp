#include "cvox/core/engine.hpp"

namespace cvox
{
void
Engine::init(std::unique_ptr<Application> application)
{
    application->set_engine(this);

    m_application = std::move(application);
    WindowSpecification spec = m_application->window_specification();
    m_window = std::make_unique<Window>(*this, spec);
    m_render_context = std::make_unique<RenderContext>(*this);

    m_application->on_start();
}

void
Engine::run()
{
    m_running = true;
    while (m_running)
    {
        if (m_window->should_close())
            m_running = false;

        m_window->update();

        m_application->on_update();

#ifndef NDEBUG
        m_render_context->reload_pipelines();
#endif
    }
}

} // namespace cvox