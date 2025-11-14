#include "ast/node.hpp"
#include "ast/nodes/declarations.hpp" // Using GenericParam as our test node
#include "emit/test_utils.hpp"

#include <catch2/catch_test_macros.hpp>
#include <optional>
#include <string_view>

TEST_CASE("withTrivia: No trivia", "[pretty_printer][trivia]")
{
    // A node with trivia = std::nullopt
    const ast::GenericParam param{ .names = { "WIDTH" }, .type_name = "integer" };

    const auto result = emit::test::render(param);
    // Should just render the core doc
    REQUIRE(result == "WIDTH : integer");
}

TEST_CASE("withTrivia: Empty trivia", "[pretty_printer][trivia]")
{
    // A node with trivia object, but empty lists
    ast::GenericParam param{ .names = { "WIDTH" }, .type_name = "integer" };
    param.trivia = ast::NodeTrivia{};

    const auto result = emit::test::render(param);
    // Should also just render the core doc
    REQUIRE(result == "WIDTH : integer");
}

TEST_CASE("withTrivia: Single leading comment", "[pretty_printer][trivia]")
{
    ast::GenericParam param{ .names = { "WIDTH" }, .type_name = "integer" };
    param.trivia
      = ast::NodeTrivia{ .leading = { ast::Comments{ .text = "-- a leading comment" } } };

    const auto result = emit::test::render(param);
    // printTrivia(Comment) adds a hardline after the text
    REQUIRE(result == "-- a leading comment\nWIDTH : integer");
}

TEST_CASE("withTrivia: Multiple leading comments", "[pretty_printer][trivia]")
{
    ast::GenericParam param{ .names = { "WIDTH" }, .type_name = "integer" };
    param.trivia = ast::NodeTrivia{
        .leading = { ast::Comments{ .text = "-- line 1" }, ast::Comments{ .text = "-- line 2" } }
    };

    const auto result = emit::test::render(param);
    REQUIRE(result == "-- line 1\n-- line 2\nWIDTH : integer");
}

TEST_CASE("withTrivia: Leading paragraph break (1 blank line)", "[pretty_printer][trivia]")
{
    ast::GenericParam param{ .names = { "WIDTH" }, .type_name = "integer" };
    param.trivia = ast::NodeTrivia{ .leading = { ast::ParagraphBreak{ .blank_lines = 1 } } };

    const auto result = emit::test::render(param);
    // 1 blank line = 1 hardline
    REQUIRE(result == "\nWIDTH : integer");
}

TEST_CASE("withTrivia: Leading paragraph break (2 blank lines)", "[pretty_printer][trivia]")
{
    ast::GenericParam param{ .names = { "WIDTH" }, .type_name = "integer" };
    param.trivia = ast::NodeTrivia{ .leading = { ast::ParagraphBreak{ .blank_lines = 2 } } };

    const auto result = emit::test::render(param);
    // 2 blank lines = 2 hardlines
    REQUIRE(result == "\n\nWIDTH : integer");
}

TEST_CASE("withTrivia: Leading comments and newlines", "[pretty_printer][trivia]")
{
    ast::GenericParam param{ .names = { "WIDTH" }, .type_name = "integer" };
    param.trivia = ast::NodeTrivia{
        .leading = { ast::Comments{ .text = "-- header" },
                    ast::ParagraphBreak{ .blank_lines = 1 },
                    ast::Comments{ .text = "-- description" } }
    };

    const auto result = emit::test::render(param);
    // Renders as: (comment + \n) + (\n) + (comment + \n) + core
    constexpr std::string_view EXPECTED = "-- header\n"
                                          "\n"
                                          "-- description\n"
                                          "WIDTH : integer";
    REQUIRE(result == EXPECTED);
}

TEST_CASE("withTrivia: Single trailing comment", "[pretty_printer][trivia]")
{
    ast::GenericParam param{ .names = { "WIDTH" }, .type_name = "integer" };
    param.trivia = ast::NodeTrivia{ .trailing = ast::Comments{ .text = "-- inline comment" } };

    const auto result = emit::test::render(param);
    // Logic is `core_doc & foot`, which adds a space
    REQUIRE(result == "WIDTH : integer -- inline comment");
}

TEST_CASE("withTrivia: Leading and trailing comments", "[pretty_printer][trivia]")
{
    ast::GenericParam param{ .names = { "WIDTH" }, .type_name = "integer" };
    param.trivia = ast::NodeTrivia{ .leading = { ast::Comments{ .text = "-- leading" } },
                                    .trailing = ast::Comments{ .text = "-- trailing" } };

    const auto result = emit::test::render(param);
    REQUIRE(result == "-- leading\nWIDTH : integer -- trailing");
}

TEST_CASE("withTrivia: Complex mix", "[pretty_printer][trivia]")
{
    ast::GenericParam param{ .names = { "WIDTH" }, .type_name = "integer" };
    param.trivia = ast::NodeTrivia{
        .leading = { ast::Comments{ .text = "-- header comment" },
                    ast::ParagraphBreak{ .blank_lines = 2 },
                    ast::Comments{ .text = "-- description" } },
        .trailing = ast::Comments{ .text = "-- end of line" }
    };

    const auto result = emit::test::render(param);
    // (comment + \n) + (\n + \n) + (comment + \n) + core + " " + (comment)
    constexpr std::string_view EXPECTED = "-- header comment\n"
                                          "\n"
                                          "\n"
                                          "-- description\n"
                                          "WIDTH : integer -- end of line";
    REQUIRE(result == EXPECTED);
}
