#include "cvox/rendering/render_context.hpp"
#include "cvox/core/engine.hpp"
#include <print>

namespace cvox
{
RenderContext::RenderContext(Engine& engine)
    : Context(engine, NamespaceId::parse("core:render_context").value())
{
    m_instance = daxa::create_instance({.app_name = "cvox"});
    m_device = m_instance.create_device_2(m_instance.choose_device({}, {}));
    m_swapchain = m_device.create_swapchain((daxa::SwapchainInfo){
        .native_window = engine.window()->get_native_handle(),
        .native_window_platform = engine.window()->get_native_platform(),
        .surface_format_selector =
            [](daxa::Format format)
        {
            switch (format)
            {
            case daxa::Format::R8G8B8A8_UINT:
                return 100;
            default:
                return daxa::default_format_score(format);
            }
        },
        .present_mode = daxa::PresentMode::MAILBOX,
        .image_usage = daxa::ImageUsageFlagBits::TRANSFER_DST,
        .name = "cvox swapchain",
    });

    m_pipeline_manager = daxa::PipelineManager(
        (daxa::PipelineManagerInfo2){.device = m_device,
                                     .root_paths =
                                         {
                                             DAXA_SHADER_INCLUDE_DIR,
                                             "sandbox/src/shaders",
                                         },
                                     .default_language = daxa::ShaderLanguage::GLSL,
                                     .default_enable_debug_info = true,
                                     .name = "cvox pipeline manager"});
}

RenderContext::~RenderContext()
{
    m_device.wait_idle();
    m_device.collect_garbage();
}

std::shared_ptr<daxa::RasterPipeline>
RenderContext::add_raster_pipeline(const daxa::RasterPipelineCompileInfo2& info)
{
    auto result = m_pipeline_manager.add_raster_pipeline2(info);
    if (result.is_err())
    {
        std::println("Failed to create raster pipeline {}", info.name);
        return nullptr;
    }
    return result.value();
}
daxa::BufferId
RenderContext::create_buffer(const daxa::BufferInfo& info)
{
    return m_device.create_buffer(info);
}

void
RenderContext::reload_pipelines()
{
    m_pipeline_manager.reload_all();
}

void
RenderContext::on_resize()
{
    m_swapchain.resize();
}

} // namespace cvox