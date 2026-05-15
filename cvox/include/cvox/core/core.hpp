#pragma once

#ifdef MSVC
#define CVOX_API __declspec(dllexport)
#else
#define CVOX_API __attribute__((__visibility__("default")))
#endif