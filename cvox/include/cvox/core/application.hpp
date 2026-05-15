#pragma once
#include "core.hpp"
#include "cvox/window/window.hpp"
#include <string>

namespace cvox
{
class Engine;
struct CVOX_API ApplicationSpecification
{
    std::string app_name = "cvox";
};

class CVOX_API Application
{
public:
    Application() = default;
    virtual ~Application(){};

    virtual WindowSpecification
    window_specification()
    {
        return {};
    }

    virtual ApplicationSpecification
    application_specification()
    {
        return {};
    }

    virtual void
    on_start()
    {
    }
    virtual void
    on_update()
    {
    }

    inline Engine&
    engine() const noexcept
    {
        return *m_engine;
    }
    inline void
    set_engine(Engine* engine)
    {
        m_engine = engine;
    }

private:
    Engine* m_engine;
};

} // namespace cvox