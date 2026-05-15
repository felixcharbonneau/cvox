#pragma once
#include "cvox/core/context.hpp"

#include <daxa/daxa.hpp>

#include <GLFW/glfw3.h>

#if defined(_WIN32)
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_NATIVE_INCLUDE_NONE
using HWND = void*;
#elif defined(__linux__)
#define GLFW_EXPOSE_NATIVE_X11
#endif
#include <GLFW/glfw3native.h>

namespace cvox
{
struct CVOX_API WindowSpecification
{
    std::string name = "cvox engine";
    uint32_t width = 500, height = 500;
};
class CVOX_API Window : public Context
{
public:
    explicit Window(Engine& engine, WindowSpecification& specification);
    ~Window();

    inline bool
    should_close()
    {
        return glfwWindowShouldClose(m_glfw_window_ptr);
    }
    inline void
    update()
    {
        glfwPollEvents();
        glfwSwapBuffers(m_glfw_window_ptr);
    }

    static inline daxa::NativeWindowPlatform
    get_native_platform()
    {
        switch (glfwGetPlatform())
        {
        case GLFW_PLATFORM_WIN32:
            return daxa::NativeWindowPlatform::WIN32_API;
        case GLFW_PLATFORM_X11:
            return daxa::NativeWindowPlatform::XLIB_API;
        case GLFW_PLATFORM_WAYLAND:
            return daxa::NativeWindowPlatform::WAYLAND_API;
        default:
            return daxa::NativeWindowPlatform::UNKNOWN;
        }
    }

    inline daxa::NativeWindowHandle
    get_native_handle() const
    {
#if defined(_WIN32)
        return glfwGetWin32Window(m_glfw_window_ptr);
#elif defined(__linux__)
        switch (get_native_platform())
        {
        case daxa::NativeWindowPlatform::XLIB_API:
        default:
            return reinterpret_cast<daxa::NativeWindowHandle>(glfwGetX11Window(m_glfw_window_ptr));
        }
#endif
    }

    inline GLFWwindow*
    get_glfw_window() const
    {
        return m_glfw_window_ptr;
    }

private:
    GLFWwindow* m_glfw_window_ptr;
    uint32_t m_width, m_height;
    bool m_minimized = false;
    bool m_swapchain_out_of_date = false;
};
} // namespace cvox