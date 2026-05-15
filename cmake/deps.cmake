include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/CPM.cmake")


# glfw for windowing
CPMAddPackage(
    NAME            glfw
    GIT_REPOSITORY  https://github.com/glfw/glfw
    GIT_TAG         3.4
    GIT_SHALLOW     TRUE
    GIT_PROGRESS    TRUE
    OPTIONS         "BUILD_SHARED_LIBS OFF"
    OPTIONS         "GLFW_BUILD_EXAMPLES OFF"
    OPTIONS         "GLFW_BUILD_TESTS OFF"
    OPTIONS         "GLFW_BUILD_DOCS OFF"
    OPTIONS         "GLFW_VULKAN_STATIC OFF"
    OPTIONS         "GLFW_INSTALL OFF"
    OPTIONS         "GLFW_INCLUDE_NONE ON"
    SYSTEM          TRUE
)

CPMAddPackage(
    NAME            entt
    GIT_REPOSITORY  https://github.com/skypjack/entt
    GIT_TAG         v3.16.0
    GIT_SHALLOW     TRUE
    GIT_PROGRESS    TRUE
)

CPMAddPackage(
    NAME            daxa
    GIT_REPOSITORY  https://github.com/expenses/Daxa
    GIT_TAG         linux-fixes
    GIT_SHALLOW     TRUE
    GIT_PROGRESS    TRUE
    OPTIONS
        "BUILD_SHARED_LIBS OFF"
        "DAXA_ENABLE_UTILS_TASK_GRAPH ON"
        "DAXA_ENABLE_UTILS_MEM ON"
        "DAXA_ENABLE_UTILS_IMGUI ON"
        "DAXA_ENABLE_UTILS_PIPELINE_MANAGER_GLSLANG ON"
    SYSTEM          TRUE
)

if(TARGET daxa)
    target_compile_options(daxa PRIVATE
        -Wno-deprecated-declarations
        -Wno-shadow
        -Wno-conversion
        -Wno-sign-conversion
        -Wno-missing-field-initializers
        -Wno-dangling-reference
        -Wno-unused-parameter
        -Wno-pragmas
        -Wno-tautological-compare
        -Wno-useless-cast
        -Wno-unused-result
    )
endif()