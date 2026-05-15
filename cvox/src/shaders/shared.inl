#pragma once
#include <daxa/daxa.inl>

struct CameraData
{
    daxa_f32mat4x4 matrix;
    daxa_f32vec3 position;
};

struct VoxelBrick
{
    daxa_u64 data[8];
};

struct TraceMainPushConstant
{
    CameraData camera;
    VoxelBrick brick;
};