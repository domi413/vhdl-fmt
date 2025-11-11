#ifndef COMMON_OVERLOAD_HPP
#define COMMON_OVERLOAD_HPP

namespace common {

// Helper for overload pattern
template<typename... Ts>
struct Overload : Ts... // NOLINT(fuchsia-multiple-inheritance)
{
    using Ts::operator()...;
};

} // namespace common

#endif // COMMON_OVERLOAD_HPP
