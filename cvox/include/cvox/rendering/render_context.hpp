#pragma once
#include <cvox/core/context.hpp>
#include <daxa/daxa.hpp>
#include <daxa/utils/pipeline_manager.hpp>
#include <memory>

namespace cvox
{

class CVOX_API RenderContext : public Context
{

public:
    RenderContext(Engine& engine);
    ~RenderContext();

    std::shared_ptr<daxa::RasterPipeline>
    add_raster_pipeline(const daxa::RasterPipelineCompileInfo2& info);

    daxa::Swapchain&
    swapchain() noexcept
    {
        return m_swapchain;
    }

    daxa::BufferId create_buffer(const daxa::BufferInfo& info);

    template<class T> T* host_map_buffer(daxa::BufferId buffer);

    daxa::Device&
    device() noexcept
    {
        return m_device;
    }
    void on_resize();

    void reload_pipelines();

private:
    daxa::Instance m_instance;
    daxa::Device m_device;
    daxa::Swapchain m_swapchain;
    daxa::PipelineManager m_pipeline_manager;
};

template<class T>
T*
RenderContext::host_map_buffer(daxa::BufferId buffer)
{
    return m_device.buffer_host_address_as<T>(buffer).value();
}

} // namespace cvox