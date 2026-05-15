#ifndef CVOX_NO_ENTRY
#pragma once
#include <cvox/cvox.hpp>

namespace cvox
{
extern CVOX_API std::unique_ptr<Application> create_application(int argc, char* argv[]);

inline auto
entry(int argc, char* argv[]) -> int
{
    Engine engine = Engine();
    engine.init(create_application(argc, argv));
    engine.run();
    return 0;
}

} // namespace cvox

auto
main(int argc, char* argv[]) -> int
{
    return cvox::entry(argc, argv);
}

#endif