#include "common/config.hpp"
#include "emit/pretty_printer/doc.hpp"
#include "emit/test_utils.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string>
#include <utility>

using emit::Doc;
using emit::test::defaultConfig;

TEST_CASE("Doc::empty creates empty document", "[doc]")
{
    const Doc doc = Doc::empty();
    REQUIRE(doc.render(defaultConfig()).empty());
}

TEST_CASE("Doc::text creates text document", "[doc]")
{
    const Doc doc = Doc::text("hello world");
    REQUIRE(doc.render(defaultConfig()) == "hello world");
}

TEST_CASE("Direct concatenation with operator+", "[doc]")
{
    const Doc doc = Doc::text("hello") + Doc::text("world");
    REQUIRE(doc.render(defaultConfig()) == "helloworld");
}

TEST_CASE("Space concatenation with operator&", "[doc]")
{
    const Doc doc = Doc::text("a") & Doc::text("b") & Doc::text("c");
    REQUIRE(doc.render(defaultConfig()) == "a b c");
}

TEST_CASE("Line break with operator/", "[doc]")
{
    const Doc doc = Doc::text("hello") / Doc::text("world");
    REQUIRE(doc.render(defaultConfig()) == "hello\nworld");
}

TEST_CASE("Operator<< nests right-hand side", "[doc]")
{
    const Doc doc = Doc::text("begin") << Doc::text("end");
    REQUIRE(doc.render(defaultConfig()) == "begin\n  end");
}

TEST_CASE("Nested indentation accumulates", "[doc]")
{
    const Doc doc = Doc::text("outer") << (Doc::text("middle") << Doc::text("inner"));
    REQUIRE(doc.render(defaultConfig()) == "outer\n  middle\n    inner");
}

TEST_CASE("Soft line becomes space when grouped and fits", "[doc]")
{
    const Doc doc = Doc::group(Doc::text("hello") / Doc::text("world"));
    REQUIRE(doc.render(defaultConfig()) == "hello world");
}

TEST_CASE("Hard line never becomes space", "[doc]")
{
    const Doc doc = Doc::group(Doc::text("hello") | Doc::text("world"));
    REQUIRE(doc.render(defaultConfig()) == "hello\nworld");
}

TEST_CASE("Empty documents don't affect output", "[doc]")
{
    const Doc doc = Doc::text("hello") + Doc::empty() & Doc::text("world");
    REQUIRE(doc.render(defaultConfig()) == "hello world");
}

TEST_CASE("Complex nested structure", "[doc]")
{
    const Doc header = Doc::text("if") & Doc::text("condition") & Doc::text("then");
    const Doc body = Doc::text("statement;");
    const Doc footer = Doc::text("end if;");

    const Doc doc = (header << body) / footer;

    REQUIRE(doc.render(defaultConfig()) == "if condition then\n  statement;\nend if;");
}

TEST_CASE("Doc is copyable", "[doc]")
{
    const Doc doc1 = Doc::text("hello");
    const Doc doc2 = doc1; // NOLINT (performance-unnecessary-copy-initialization)

    REQUIRE(doc1.render(defaultConfig()) == "hello");
    REQUIRE(doc2.render(defaultConfig()) == "hello");
}

TEST_CASE("Doc is movable", "[doc]")
{
    Doc doc1 = Doc::text("hello");
    const Doc doc2 = std::move(doc1);

    REQUIRE(doc2.render(defaultConfig()) == "hello");
}

TEST_CASE("Structural sharing works correctly", "[doc]")
{
    const Doc shared = Doc::text("shared") & Doc::text("part");
    const Doc doc1 = shared & Doc::text("A");
    const Doc doc2 = shared & Doc::text("B");

    REQUIRE(doc1.render(defaultConfig()) == "shared part A");
    REQUIRE(doc2.render(defaultConfig()) == "shared part B");
}

TEST_CASE("Render with different widths", "[doc]")
{
    const Doc doc = Doc::group(Doc::text("short") + Doc::line() + Doc::text("text"));

    // Wide enough - should fit on one line
    REQUIRE(doc.render(defaultConfig()) == "short text");

    constexpr int TOO_NARROW = 5;
    auto config = defaultConfig();
    config.line_config.line_length = TOO_NARROW;

    // Too narrow - should break into two lines
    REQUIRE(doc.render(config) == "short\ntext");
}

TEST_CASE("hardIndent forces line break with indentation", "[doc]")
{
    const Doc doc = Doc::text("begin").hardIndent(Doc::text("end"));

    // Should always break, even if grouped
    REQUIRE(doc.render(defaultConfig()) == "begin\n  end");
}

TEST_CASE("hardIndent always breaks even when grouped", "[doc]")
{
    const Doc doc = Doc::group(Doc::text("begin").hardIndent(Doc::text("end")));

    // Hard line never becomes space, even in a group
    REQUIRE(doc.render(defaultConfig()) == "begin\n  end");
}

TEST_CASE("Soft vs hard indent comparison", "[doc]")
{
    const Doc soft = Doc::group(Doc::text("x") << Doc::text("y"));
    const Doc hard = Doc::group(Doc::text("x").hardIndent(Doc::text("y")));

    // Soft can collapse when grouped and fits
    REQUIRE(soft.render(defaultConfig()) == "x y");

    // Hard always breaks, even when grouped
    REQUIRE(hard.render(defaultConfig()) == "x\n  y");
}

TEST_CASE("Multiple hardlines with hardlines()", "[doc]")
{
    const Doc doc = Doc::text("start") + Doc::hardlines(3) + Doc::text("end");

    REQUIRE(doc.render(defaultConfig()) == "start\n\n\nend");
}

TEST_CASE("hardlines(0) prevents flattening", "[doc]")
{
    const Doc doc
      = Doc::group(Doc::text("this text") / Doc::text("should not flatten") + Doc::hardlines(0));

    // Should always break due to hardlines(0)
    REQUIRE(doc.render(defaultConfig()) == "this text\nshould not flatten");
}

TEST_CASE("AlignText aligns correctly", "[doc]")
{
    const Doc doc
      = Doc::align(Doc::alignText("1", 1) / Doc::alignText("12", 1) / Doc::alignText("123", 1));

    common::Config config = defaultConfig();
    config.port_map.align_signals = true;

    REQUIRE(doc.render(config) == "1  \n12 \n123");
}

TEST_CASE("AlignText with varying alignment columns", "[doc]")
{
    constexpr std::string_view EXPECTED = "a  : bbb :\n"
                                          "aa : bb  :";

    const Doc doc1
      = Doc::alignText("a", 1) & Doc::text(":") & Doc::alignText("bbb", 2) & Doc::text(":");
    const Doc doc2
      = Doc::alignText("aa", 1) & Doc::text(":") & Doc::alignText("bb", 2) & Doc::text(":");

    const Doc doc = Doc::align(doc1 / doc2);

    common::Config config = defaultConfig();
    config.port_map.align_signals = true;

    REQUIRE(doc.render(config) == EXPECTED);
}