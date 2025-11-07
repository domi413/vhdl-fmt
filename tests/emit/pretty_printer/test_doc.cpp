#include "emit/pretty_printer/doc.hpp"
#include "emit/pretty_printer/table.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string>
#include <utility>

using emit::Doc;
using emit::space;

TEST_CASE("Doc::empty creates empty document", "[doc]")
{
    const Doc doc = Doc::empty();
    REQUIRE(doc.render(80).empty());
}

TEST_CASE("Doc::text creates text document", "[doc]")
{
    const Doc doc = Doc::text("hello world");
    REQUIRE(doc.render(80) == "hello world");
}

TEST_CASE("Space helper creates single space", "[doc]")
{
    const Doc doc = space();
    REQUIRE(doc.render(80) == " ");
}

TEST_CASE("Direct concatenation with operator+", "[doc]")
{
    const Doc doc = Doc::text("hello") + Doc::text("world");
    REQUIRE(doc.render(80) == "helloworld");
}

TEST_CASE("Space concatenation with operator&", "[doc]")
{
    const Doc doc = Doc::text("a") & Doc::text("b") & Doc::text("c");
    REQUIRE(doc.render(80) == "a b c");
}

TEST_CASE("Line break with operator/", "[doc]")
{
    const Doc doc = Doc::text("hello") / Doc::text("world");
    std::string expected = "hello\nworld";
    REQUIRE(doc.render(80) == expected);
}

TEST_CASE("Operator<< nests right-hand side by 2", "[doc]")
{
    const Doc doc = Doc::text("begin") << Doc::text("end");
    REQUIRE(doc.render(80) == "begin\n  end");
}

TEST_CASE("Nested indentation accumulates", "[doc]")
{
    const Doc inner = Doc::text("inner");
    const Doc middle = Doc::text("middle");
    const Doc outer = Doc::text("outer");

    const Doc full = outer << (middle << inner);

    std::string expected = "outer\n  middle\n    inner";
    REQUIRE(full.render(80) == expected);
}

TEST_CASE("Operator>> dedents right-hand side by 2", "[doc]")
{
    const Doc doc = Doc::text("begin") << Doc::text("indented") >> Doc::text("back");
    std::string expected = "begin\n  indented\nback";
    REQUIRE(doc.render(80) == expected);
}

TEST_CASE("Soft line becomes space when grouped and fits", "[doc]")
{
    const Doc doc = (Doc::text("hello") / Doc::text("world")).group();
    // Soft line becomes space when it fits
    REQUIRE(doc.render(80) == "hello world");
}

TEST_CASE("Hard line never becomes space", "[doc]")
{
    const Doc doc = (Doc::text("hello") | Doc::text("world")).group();
    // Hard line always breaks, even when grouped
    REQUIRE(doc.render(80) == "hello\nworld");
}

TEST_CASE("Empty documents don't affect output", "[doc]")
{
    const Doc doc = Doc::text("hello") + Doc::empty() + space() + Doc::text("world");
    REQUIRE(doc.render(80) == "hello world");
}

TEST_CASE("Complex nested structure", "[doc]")
{
    const Doc header = Doc::text("if") & Doc::text("condition") & Doc::text("then");
    const Doc body = Doc::text("statement;");
    const Doc footer = Doc::text("end if;");
    const Doc full = header << body >> footer;

    const std::string expected = "if condition then\n  statement;\nend if;";
    REQUIRE(full.render(80) == expected);
}

TEST_CASE("Doc is copyable", "[doc]")
{
    const Doc doc1 = Doc::text("hello");
    const Doc doc2 = doc1; // NOLINT (performance-unnecessary-copy-initialization)

    REQUIRE(doc1.render(80) == "hello");
    REQUIRE(doc2.render(80) == "hello");
}

TEST_CASE("Doc is movable", "[doc]")
{
    Doc doc1 = Doc::text("hello");
    const Doc doc2 = std::move(doc1);

    REQUIRE(doc2.render(80) == "hello");
}

TEST_CASE("Structural sharing works correctly", "[doc]")
{
    const Doc shared = Doc::text("shared") & Doc::text("part");
    const Doc doc1 = shared & Doc::text("A");
    const Doc doc2 = shared & Doc::text("B");

    REQUIRE(doc1.render(80) == "shared part A");
    REQUIRE(doc2.render(80) == "shared part B");
}

TEST_CASE("Render with different widths", "[doc]")
{
    const Doc doc = (Doc::text("short") + Doc::line() + Doc::text("text")).group();

    // Wide enough - should fit on one line
    REQUIRE(doc.render(80) == "short text");

    // Too narrow - should break
    REQUIRE(doc.render(5) == "short\ntext");
}

TEST_CASE("Table with simple alignment", "[doc][table]")
{
    using emit::makeTable;

    std::vector<std::vector<Doc>> rows = {
        { Doc::text("enable"),   Doc::text(":"), Doc::text("in"),  Doc::text("std_logic")        },
        { Doc::text("clk"),      Doc::text(":"), Doc::text("in"),  Doc::text("std_logic")        },
        { Doc::text("data_out"), Doc::text(":"), Doc::text("out"), Doc::text("std_logic_vector") }
    };

    auto table = makeTable(rows);

    const std::string expected = "enable   : in  std_logic\n"
                                 "clk      : in  std_logic\n"
                                 "data_out : out std_logic_vector";
    REQUIRE(table.render(80) == expected);
}

TEST_CASE("Table composable with other Doc operations", "[doc][table]")
{
    using emit::makeTable;

    std::vector<std::vector<Doc>> rows = {
        { Doc::text("port1"), Doc::text(":"), Doc::text("in"),  Doc::text("std_logic") },
        { Doc::text("port2"), Doc::text(":"), Doc::text("out"), Doc::text("std_logic") }
    };

    auto table = makeTable(rows);
    auto doc = Doc::text("entity MyEntity is") / Doc::text("port (") << table >> Doc::text(");");

    const std::string expected = "entity MyEntity is\n"
                                 "port (\n"
                                 "  port1 : in  std_logic\n"
                                 "  port2 : out std_logic\n"
                                 ");";

    REQUIRE(doc.render(80) == expected);
}

TEST_CASE("Empty table", "[doc][table]")
{
    using emit::makeTable;

    std::vector<std::vector<Doc>> rows;
    auto table = makeTable(rows);

    REQUIRE(table.render(80).empty());
}
