#include <shared.inl>
DAXA_DECL_PUSH_CONSTANT(ComputePush, push)


layout(local_size_x = 8, local_size_y = 8, local_size_z = 1) in;
void main()
{
    uvec3 pixel_i = gl_GlobalInvocationID.xyz;
    if (pixel_i.x >= push.frame_dim.x || pixel_i.y >= push.frame_dim.y)
        return;

    imageStore(daxa_image2D(push.image), ivec2(pixel_i.xy), vec4(1.2, 0.0, 1.0, 1));
}