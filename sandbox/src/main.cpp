#include "shaders/shared.inl"
#include <cvox/cvox.hpp>

#include <daxa/utils/task_graph.hpp>

class SandboxApplication : public cvox::Application
{
public:
    cvox::ApplicationSpecification
    application_specification() override
    {
        return {.app_name = "Cvox Sandbox"};
    }

    void
    register_graph()
    {
        task_render_image = daxa::ExternalTaskImage(
            daxa::ExternalTaskImageInfo{.image = render_image, .name = "task_render_image"});

        task_swapchain_image = daxa::ExternalTaskImage((daxa::ExternalTaskImageInfo){
            .is_swapchain_image = true, .name = "cvox swapchain image task"});

        graph =
            daxa::TaskGraph(daxa::TaskGraphInfo{.device = engine().render_context()->device(),
                                                .swapchain = engine().render_context()->swapchain(),
                                                .name = "loop"});
        graph.register_image(task_swapchain_image);
        graph.register_image(task_render_image);

        graph.add_task(
            daxa::InlineTask::Compute("Draw (Compute)")
                .writes(task_render_image)
                .executes(
                    [this](daxa::TaskInterface ti)
                    {
                        ti.recorder.set_pipeline(*compute_pipeline);
                        ti.recorder.push_constant(ComputePush{
                            .image = render_image.default_view(),
                            .frame_dim = {engine().window()->width(), engine().window()->height()},
                        });
                        ti.recorder.dispatch({(engine().window()->width() + 7) / 8,
                                              (engine().window()->height() + 7) / 8});
                    }));
        graph.add_task(
            daxa::InlineTask::Transfer("Blit (render to swapchain)")
                .reads(task_render_image)
                .writes(task_swapchain_image)
                .executes(
                    [this](daxa::TaskInterface ti)
                    {
                        ti.recorder.blit_image_to_image({
                            .src_image = ti.get(task_render_image).id,
                            .dst_image = ti.get(task_swapchain_image).id,
                            .src_offsets = {{{0, 0, 0},
                                             {static_cast<int32_t>(engine().window()->width()),
                                              static_cast<int32_t>(engine().window()->height()),
                                              1}}},
                            .dst_offsets = {{{0, 0, 0},
                                             {static_cast<int32_t>(engine().window()->width()),
                                              static_cast<int32_t>(engine().window()->height()),
                                              1}}},
                        });
                    }));

        graph.submit({});
        graph.present({});
        graph.complete({});
    }

    void
    on_start() override
    {
        compute_pipeline = engine().render_context()->add_compute_pipeline(
            daxa::ComputePipelineCompileInfo2{.source = daxa::ShaderFile("compute.glsl"),
                                              .push_constant_size = sizeof(ComputePush),
                                              .name = "voxel pipeline"});
        render_image = engine().render_context()->device().create_image(
            daxa::ImageInfo{.format = daxa::Format::R8G8B8A8_UNORM,
                            .size = {engine().window()->width(), engine().window()->height(), 1},
                            .usage = daxa::ImageUsageFlagBits::SHADER_STORAGE |
                                     daxa::ImageUsageFlagBits::TRANSFER_SRC,
                            .name = "render image"});
        register_graph();
    }

    void
    on_update() override
    {
        auto swapchain_image = engine().render_context()->swapchain().acquire_next_image();
        if (swapchain_image.is_empty())
        {
            return;
        }

        task_swapchain_image.set_image(swapchain_image);

        graph.execute({});
        engine().render_context()->device().collect_garbage();
    }

private:
    std::shared_ptr<daxa::ComputePipeline> compute_pipeline;

    daxa::TaskGraph graph;
    daxa::ExternalTaskImage task_swapchain_image;
    daxa::ImageId render_image;
    daxa::ExternalTaskImage task_render_image;
};

auto
cvox::create_application(int argc, char** argv) -> std::unique_ptr<cvox::Application>
{
    return std::make_unique<SandboxApplication>();
}