#include "emit/pretty_printer/doc.hpp"
#include <catch2/catch_test_macros.hpp>
#include <string>

using emit::Doc;
using emit::space;

TEST_CASE("Doc::empty creates empty document", "[doc]")
{
    Doc doc = Doc::empty();
    REQUIRE(doc.render(80) == "");
}

TEST_CASE("Doc::text creates text document", "[doc]")
{
    Doc doc = Doc::text("hello");
    REQUIRE(doc.render(80) == "hello");
}

TEST_CASE("Doc::text with multiple words", "[doc]")
{
    Doc doc = Doc::text("hello world");
    REQUIRE(doc.render(80) == "hello world");
}

TEST_CASE("space helper creates single space", "[doc]")
{
    Doc doc = space();
    REQUIRE(doc.render(80) == " ");
}

TEST_CASE("Concatenation with operator+", "[doc]")
{
    Doc doc = Doc::text("hello") + Doc::text("world");
    REQUIRE(doc.render(80) == "helloworld");
}

TEST_CASE("Concatenation with spaces", "[doc]")
{
    Doc doc = Doc::text("hello") + space() + Doc::text("world");
    REQUIRE(doc.render(80) == "hello world");
}

TEST_CASE("Multiple concatenations", "[doc]")
{
    Doc doc = Doc::text("a") + space() + Doc::text("b") + space() + Doc::text("c");
    REQUIRE(doc.render(80) == "a b c");
}

TEST_CASE("Line break renders as newline", "[doc]")
{
    Doc doc = Doc::text("hello") + Doc::line() + Doc::text("world");
    REQUIRE(doc.render(80) == "hello\nworld");
}

TEST_CASE("Line break with indentation", "[doc]")
{
    Doc doc = Doc::text("hello") + Doc::line() + Doc::text("world").nest(2);
    // Note: nest should affect the indentation after line breaks
    std::string expected = "hello\n  world";
    REQUIRE(doc.render(80) == expected);
}

TEST_CASE("Nest increases indentation", "[doc]")
{
    Doc doc = (Doc::text("begin") + Doc::line() + Doc::text("end")).nest(2);
    REQUIRE(doc.render(80) == "begin\n  end");
}

TEST_CASE("Nested indentation accumulates", "[doc]")
{
    Doc inner = Doc::text("inner");
    Doc middle = (Doc::text("middle") + Doc::line() + inner.nest(2)).nest(2);
    Doc doc = Doc::text("outer") + Doc::line() + middle;

    std::string expected = "outer\n  middle\n    inner";
    REQUIRE(doc.render(80) == expected);
}TEST_CASE("Group allows line to be flattened when it fits", "[doc]")
{
    Doc doc = (Doc::text("a") + Doc::line() + Doc::text("b")).group();
    // Should fit on one line, so line becomes space
    REQUIRE(doc.render(80) == "a b");
}

TEST_CASE("Group forces break when content doesn't fit", "[doc]")
{
    // Create content that's too wide for the line
    Doc doc = (Doc::text("very_long_identifier_that_exceeds") +
               Doc::line() +
               Doc::text("the_width_limit")).group();

    const int width = 20;

    std::string result = doc.render(width);
    // Should break because it doesn't fit in 20 characters
    REQUIRE(result == "very_long_identifier_that_exceeds\nthe_width_limit");
}

TEST_CASE("Empty documents don't affect output", "[doc]")
{
    Doc doc = Doc::text("hello") + Doc::empty() + space() + Doc::text("world");
    REQUIRE(doc.render(80) == "hello world");
}

TEST_CASE("Complex nested structure", "[doc]")
{
    Doc header = Doc::text("if") + space() + Doc::text("condition");
    Doc body = Doc::text("then") + Doc::line() + Doc::text("statement;");
    Doc full = header + Doc::line() + body.nest(2);

    std::string expected = "if condition\nthen\n  statement;";
    REQUIRE(full.render(80) == expected);
}

TEST_CASE("Doc is copyable", "[doc]")
{
    Doc doc1 = Doc::text("hello");
    Doc doc2 = doc1;  // NOLINT (performance-unnecessary-copy-initialization)

    REQUIRE(doc1.render(80) == "hello");
    REQUIRE(doc2.render(80) == "hello");
}

TEST_CASE("Doc is movable", "[doc]")
{
    Doc doc1 = Doc::text("hello");
    Doc doc2 = std::move(doc1);

    REQUIRE(doc2.render(80) == "hello");
}

TEST_CASE("Structural sharing works correctly", "[doc]")
{
    Doc shared = Doc::text("shared") + space() + Doc::text("part");
    Doc doc1 = shared + space() + Doc::text("A");
    Doc doc2 = shared + space() + Doc::text("B");

    REQUIRE(doc1.render(80) == "shared part A");
    REQUIRE(doc2.render(80) == "shared part B");
}

TEST_CASE("Render with different widths", "[doc]")
{
    Doc doc = (Doc::text("short") + Doc::line() + Doc::text("text")).group();

    // Wide enough - should fit on one line
    REQUIRE(doc.render(80) == "short text");

    // Too narrow - should break
    REQUIRE(doc.render(5) == "short\ntext");
}
