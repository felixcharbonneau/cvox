#include "shaders/shared.inl"
#include <array>
#include <cvox/cvox.hpp>

#include <daxa/utils/task_graph.hpp>

void
draw_swapchain_task_callback(daxa::TaskInterface ti,
                             daxa::RasterPipeline* pipeline,
                             daxa::TaskImageView color_target,
                             daxa::BufferId vertex_buffer)
{
    // The task interface provides a way to get the attachment info:
    auto image_info = ti.info(color_target).value();
    auto image_id = ti.id(color_target);
    auto image_view_id = ti.view(color_target);
    auto image_layout = ti.layout(color_target);

    // When starting a render pass via a rasterization pipeline, daxa "eats" a generic command
    // recorder and turns it into a RenderCommandRecorder. Only the RenderCommandRecorder can record
    // raster commands. The RenderCommandRecorder can only record commands that are valid within a
    // render pass. This way daxa ensures typesafety for command recording.
    daxa::RenderCommandRecorder render_recorder =
        std::move(ti.recorder)
            .begin_renderpass({
                .color_attachments =
                    std::array{
                        daxa::RenderAttachmentInfo{
                            .image_view = ti.view(color_target),
                            .load_op = daxa::AttachmentLoadOp::CLEAR,
                            .clear_value = std::array<daxa::f32, 4>{0.1f, 0.0f, 0.5f, 1.0f},
                        },
                    },
                .render_area = {.width = image_info.size.x, .height = image_info.size.y},
            });
    // Here, we'll bind the pipeline to be used in the draw call below
    render_recorder.set_pipeline(*pipeline);

    // Very importantly, task graph packs up our attachment shader data into a byte blob.
    // We need to pass this blob to our shader somehow.
    // The typical way to do this is to assign the blob to the push constant.
    render_recorder.push_constant(MyPushConstant{
        .my_vertex_ptr = ti.device.device_address(vertex_buffer).value(),
    });
    // and issue the draw call with the desired number of vertices.
    render_recorder.draw({.vertex_count = 3});

    // VERY IMPORTANT! A renderpass must be ended after finishing!
    // The ending of a render pass returns back the original command recorder.
    // Assign it back to the task interfaces command recorder.
    ti.recorder = std::move(render_recorder).end_renderpass();
};

class SandboxApplication : public cvox::Application
{
public:
    cvox::ApplicationSpecification
    application_specification() override
    {
        return {.app_name = "Cvox Sandbox"};
    }

    void
    on_start() override
    {
        pipeline = engine().render_context()->add_raster_pipeline((
            daxa::RasterPipelineCompileInfo2){
            .vertex_shader_info = daxa::ShaderCompileInfo2{.source = daxa::ShaderFile{"main.glsl"}},
            .fragment_shader_info =
                daxa::ShaderCompileInfo2{.source = daxa::ShaderFile{"main.glsl"}},
            .color_attachments = {{.format = engine().render_context()->swapchain().get_format()}},
            .raster = {},
            .push_constant_size = sizeof(MyPushConstant),
            .name = "my pipeline",
        });

        buffer = engine().render_context()->create_buffer({
            .size = sizeof(MyVertex) * 3,
            .allocate_info = daxa::MemoryFlagBits::HOST_ACCESS_SEQUENTIAL_WRITE,
            .name = "my vertex data",
        });
        buffer_mapped = engine().render_context()->host_map_buffer<std::array<MyVertex, 3>>(buffer);
        *buffer_mapped = {
            MyVertex{.position = {-0.5f, +0.5f, 0.0f}, .color = {1.0f, 0.0f, 0.0f}},
            MyVertex{.position = {+0.5f, +0.5f, 0.0f}, .color = {0.0f, 1.0f, 0.0f}},
            MyVertex{.position = {+0.0f, -0.5f, 0.0f}, .color = {0.0f, 0.0f, 1.0f}},
        };

        task_swapchain_image = daxa::TaskImage(
            (daxa::TaskImageInfo){.swapchain_image = true, .name = "swapchain image"});

        graph = daxa::TaskGraph(
            (daxa::TaskGraphInfo){.device = engine().render_context()->device(),
                                  .swapchain = engine().render_context()->swapchain(),
                                  .name = "loop"});
        graph.use_persistent_image(task_swapchain_image);

        auto draw_swapchain_task =
            daxa::RasterTask("draw triangle")
                .color_attachment
                .reads_writes(daxa::ImageViewType::REGULAR_2D, task_swapchain_image.view())
                .executes(draw_swapchain_task_callback,
                          pipeline.get(),
                          task_swapchain_image.view(),
                          buffer);
        graph.add_task(draw_swapchain_task);
        graph.submit({});
        graph.present({});
        graph.complete({});
    }

    void
    on_update() override
    {
        auto swapchain_image = engine().render_context()->swapchain().acquire_next_image();
        if (swapchain_image.is_empty())
        {
            return;
        }

        task_swapchain_image.set_images({.images = std::span{&swapchain_image, 1}});

        graph.execute({});
        engine().render_context()->device().collect_garbage();
    }

private:
    std::shared_ptr<daxa::RasterPipeline> pipeline;
    daxa::BufferId buffer;
    std::array<MyVertex, 3>* buffer_mapped;

    daxa::TaskGraph graph;
    daxa::TaskImage task_swapchain_image;
};

auto
cvox::create_application(int argc, char** argv) -> std::unique_ptr<cvox::Application>
{
    return std::make_unique<SandboxApplication>();
}