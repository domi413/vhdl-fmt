#ifndef BUILDER_ASSEMBLY_SCOPE_HPP
#define BUILDER_ASSEMBLY_SCOPE_HPP

#include "builder/assembly/sink.hpp"

#include <memory>
#include <vector>

namespace builder {

/// RAII helper for redirecting builder output into a different vector
template<typename Vec>
struct SlotGuard
{
    explicit SlotGuard(std::vector<std::unique_ptr<ISink>> &s, Vec &vec) : sinks_(s)
    {
        using NodeT = typename Vec::value_type::element_type;
        this->sinks_.push_back(std::make_unique<SinkImpl<NodeT>>(vec));
    }

    ~SlotGuard() noexcept { this->sinks_.pop_back(); }

    SlotGuard(const SlotGuard &) = delete;
    auto operator=(const SlotGuard &) -> SlotGuard & = delete;
    SlotGuard(SlotGuard &&) = delete;
    auto operator=(SlotGuard &&) -> SlotGuard & = delete;

  private:
    std::vector<std::unique_ptr<ISink>> &sinks_;
};

} // namespace builder

#endif /* BUILDER_ASSEMBLY_SCOPE_HPP */
