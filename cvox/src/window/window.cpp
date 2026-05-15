#include "cvox/window/window.hpp"
#include "cvox/core/engine.hpp"

namespace cvox
{
Window::Window(Engine& engine, WindowSpecification& specification)
    : Context(engine, NamespaceId::parse("cvox::window").value()), m_width(specification.width),
      m_height(specification.height)
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    m_glfw_window_ptr = glfwCreateWindow(static_cast<int32_t>(m_width),
                                         static_cast<int32_t>(m_height),
                                         specification.name.c_str(),
                                         nullptr,
                                         nullptr);
    glfwSetWindowUserPointer(m_glfw_window_ptr, this);

    glfwSetWindowSizeCallback(m_glfw_window_ptr,
                              [](GLFWwindow* window, int size_x, int size_y)
                              {
                                  auto* win =
                                      static_cast<Window*>(glfwGetWindowUserPointer(window));
                                  win->m_width = static_cast<uint32_t>(size_x);
                                  win->m_height = static_cast<uint32_t>(size_y);
                                  win->m_swapchain_out_of_date = true;

                                  win->engine().render_context()->on_resize();
                              });
}
Window::~Window()
{
    glfwDestroyWindow(m_glfw_window_ptr);
    glfwTerminate();
}

} // namespace cvox