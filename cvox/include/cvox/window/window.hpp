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

    inline daxa::NativeWindowInfo
    get_native_window_info()
    {
#if defined(_WIN32)
        return daxa::NativeWindowInfoWin32{
            .hwnd = glfwGetWin32Window(m_glfw_window_ptr),
        };
#elif defined(__linux__)
        return daxa::NativeWindowInfoXlib{
            .window = reinterpret_cast<void*>(glfwGetX11Window(m_glfw_window_ptr))};
#endif
    }

    inline GLFWwindow*
    get_glfw_window() const
    {
        return m_glfw_window_ptr;
    }

    inline uint32_t
    width() const noexcept
    {
        return m_width;
    }

    inline uint32_t
    height() const noexcept
    {
        return m_height;
    }

private:
    GLFWwindow* m_glfw_window_ptr;
    uint32_t m_width, m_height;
    bool m_minimized = false;
    bool m_swapchain_out_of_date = false;
};
} // namespace cvox