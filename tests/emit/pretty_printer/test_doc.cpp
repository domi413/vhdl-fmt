#include "common/config.hpp"
#include "emit/pretty_printer/doc.hpp"
#include "emit/test_utils.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string>
#include <utility>

using emit::Doc;
using emit::space;
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

TEST_CASE("Space helper creates single space", "[doc]")
{
    const Doc doc = space();
    REQUIRE(doc.render(defaultConfig()) == " ");
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
    std::string expected = "hello\nworld";
    REQUIRE(doc.render(defaultConfig()) == expected);
}

TEST_CASE("Operator<< nests right-hand side", "[doc]")
{
    const Doc doc = Doc::text("begin") << Doc::text("end");
    REQUIRE(doc.render(defaultConfig()) == "begin\n  end");
}

TEST_CASE("Operator>> dedents right-hand side", "[doc]")
{
    const Doc doc = Doc::text("begin") << Doc::text("indented") >> Doc::text("back");
    std::string expected = "begin\n  indented\nback";
    REQUIRE(doc.render(defaultConfig()) == expected);
}

TEST_CASE("Nested indentation accumulates", "[doc]")
{
    const Doc inner = Doc::text("inner");
    const Doc middle = Doc::text("middle");
    const Doc outer = Doc::text("outer");

    const Doc full = outer << (middle << inner);

    std::string expected = "outer\n  middle\n    inner";
    REQUIRE(full.render(defaultConfig()) == expected);
}

TEST_CASE("Soft line becomes space when grouped and fits", "[doc]")
{
    const Doc doc = (Doc::text("hello") / Doc::text("world")).group();
    // Soft line becomes space when it fits
    REQUIRE(doc.render(defaultConfig()) == "hello world");
}

TEST_CASE("Hard line never becomes space", "[doc]")
{
    const Doc doc = (Doc::text("hello") | Doc::text("world")).group();
    // Hard line always breaks, even when grouped
    REQUIRE(doc.render(defaultConfig()) == "hello\nworld");
}

TEST_CASE("Empty documents don't affect output", "[doc]")
{
    const Doc doc = Doc::text("hello") + Doc::empty() + space() + Doc::text("world");
    REQUIRE(doc.render(defaultConfig()) == "hello world");
}

TEST_CASE("Complex nested structure", "[doc]")
{
    const Doc header = Doc::text("if") & Doc::text("condition") & Doc::text("then");
    const Doc body = Doc::text("statement;");
    const Doc footer = Doc::text("end if;");
    const Doc full = header << (body >> footer);

    const std::string expected = "if condition then\n  statement;\nend if;";
    REQUIRE(full.render(defaultConfig()) == expected);
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
    const Doc doc = (Doc::text("short") + Doc::line() + Doc::text("text")).group();

    // Wide enough - should fit on one line
    REQUIRE(doc.render(defaultConfig()) == "short text");

    // Too narrow - should break
    constexpr int TOO_NARROW = 5;
    auto config = defaultConfig();
    config.line_config.line_length = TOO_NARROW;
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
    const Doc doc = Doc::text("begin").hardIndent(Doc::text("end")).group();
    // Hard line never becomes space, even in a group
    REQUIRE(doc.render(defaultConfig()) == "begin\n  end");
}

TEST_CASE("hardDedent reduces indentation with hard break", "[doc]")
{
    const Doc doc
      = Doc::text("begin").hardIndent(Doc::text("indented")).hardDedent(Doc::text("back"));
    const std::string expected = "begin\n  indented\nback";
    REQUIRE(doc.render(defaultConfig()) == expected);
}

TEST_CASE("Soft vs hard indent comparison", "[doc]")
{
    const Doc soft = (Doc::text("x") << Doc::text("y")).group();
    const Doc hard = Doc::text("x").hardIndent(Doc::text("y")).group();

    // Soft can collapse when grouped and fits
    REQUIRE(soft.render(defaultConfig()) == "x y");

    // Hard always breaks, even when grouped
    REQUIRE(hard.render(defaultConfig()) == "x\n  y");
}
