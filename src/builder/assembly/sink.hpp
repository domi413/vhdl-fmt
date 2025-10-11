#ifndef BUILDER_ASSEMBLY_SINK_HPP
#define BUILDER_ASSEMBLY_SINK_HPP

#include "ast/node.hpp"

#include <cassert>
#include <memory>
#include <vector>

namespace builder {

struct ISink
{
    using Node = ast::Node;
    using NodePtr = std::unique_ptr<Node>;

    ISink() = default;
    virtual ~ISink() = default;
    ISink(const ISink &) = delete;
    auto operator=(const ISink &) -> ISink & = delete;
    ISink(ISink &&) = delete;
    auto operator=(ISink &&) -> ISink & = delete;

    virtual void push(NodePtr n) = 0;
};

template<typename T>
struct SinkImpl : ISink
{
    explicit SinkImpl(std::vector<std::unique_ptr<T>> &v) : vec_(v) {}

    ~SinkImpl() override = default;
    SinkImpl(const SinkImpl &) = delete;
    auto operator=(const SinkImpl &) -> SinkImpl & = delete;
    SinkImpl(SinkImpl &&) = delete;
    auto operator=(SinkImpl &&) -> SinkImpl & = delete;

    void push(NodePtr n) override
    {
        auto *casted{ dynamic_cast<T *>(n.release()) };
        assert(casted && "Wrong node type pushed into sink!");
        this->vec_.emplace_back(casted);
    }

  private:
    std::vector<std::unique_ptr<T>> &vec_;
};

} // namespace builder

#endif /* BUILDER_ASSEMBLY_SINK_HPP */
