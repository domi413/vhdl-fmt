#ifndef BUILDER_ASSEMBLY_BUILDER_SCOPE_HPP
#define BUILDER_ASSEMBLY_BUILDER_SCOPE_HPP

#include "builder/assembly/sink.hpp"

#include <memory>
#include <type_traits>
#include <vector>

namespace builder {

/// RAII helper for redirecting builder output into a different vector
template<typename Vec>
struct SlotGuard
{
    explicit SlotGuard(std::vector<std::unique_ptr<ISink>> &s, Vec &vec) : sinks(s)
    {
        using Elem = typename std::remove_cvref_t<Vec>::value_type;
        using NodeT = typename Elem::element_type;
        this->sinks.push_back(std::make_unique<SinkImpl<NodeT>>(vec));
    }

    ~SlotGuard() noexcept { this->sinks.pop_back(); }

    SlotGuard(const SlotGuard &) = delete;
    auto operator=(const SlotGuard &) -> SlotGuard & = delete;
    SlotGuard(SlotGuard &&) = delete;
    auto operator=(SlotGuard &&) -> SlotGuard & = delete;

  private:
    std::vector<std::unique_ptr<ISink>> &sinks;
};

} // namespace builder

#endif /* BUILDER_ASSEMBLY_BUILDER_SCOPE_HPP */
