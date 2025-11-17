#include "ast/node.hpp"
#include "ast/nodes/expressions.hpp"
#include "emit/test_utils.hpp"

#include <catch2/catch_test_macros.hpp>
#include <optional>

// ========================================================================
// NOTE: These tests are integration tests. They test the *entire* pipeline:
// 1. PrettyPrinter creates an unoptimized, "dumb" Doc tree.
// 2. withTrivia() calls doc.optimize().
// 3. optimize() applies peephole optimization rules (e.g., merging text).
// 4. The final, smaller, optimized tree is rendered to a string.
//
// We verify that the final string matches the *optimized* output.
// ========================================================================

TEST_CASE("Optimizer: Rule 1 (Identity) - empty + a", "[pretty_printer][optimizer]")
{
    // A node with only leading trivia.
    // Unoptimized: fold(printTrivia(...)) = (empty + hardlines(1))
    // Optimized: hardlines(1)
    // Final tree: hardlines(1) + core_doc
    ast::TokenExpr token{ .text = "foo" };
    token.trivia = ast::NodeTrivia{ .leading = { ast::ParagraphBreak{ .blank_lines = 1 } } };

    const auto result = emit::test::render(token);
    REQUIRE(result == "\nfoo");
}

TEST_CASE("Optimizer: Rule 1 (Identity) - a + empty", "[pretty_printer][optimizer]")
{
    // A node with 1 blank line trailing (2 newlines).
    // TriviaBinder creates ParagraphBreak{1}.
    // printTrailingTriviaList creates: Doc::line() + hardlines(1 - 1)
    // This becomes: Doc::line() + hardlines(0)
    // Doc::hardlines(0) creates: Doc::empty()
    // Unoptimized tree: core_doc + (Doc::line() + Doc::empty())
    // Optimized tree: core_doc + Doc::line()
    ast::TokenExpr token{ .text = "foo" };
    token.trivia = ast::NodeTrivia{ .trailing = { ast::ParagraphBreak{ .blank_lines = 1 } } };

    const auto result = emit::test::render(token);
    REQUIRE(result == "foo\n"); // Just one newline, not two
}

TEST_CASE("Optimizer: Rule 2 (Text Merge) - Simple", "[pretty_printer][optimizer]")
{
    // ...
    const ast::TokenExpr literal{ .text = "123" };

    // Use std::make_unique<ast::Expr> to create the Box<Expr>
    const ast::UnaryExpr expr{ .op = "-", .value = std::make_unique<ast::Expr>(literal) };

    const auto result = emit::test::render(expr);
    REQUIRE(result == "-123");
}

TEST_CASE("Optimizer: Rule 2 (Text Merge) - Chained", "[pretty_printer][optimizer]")
{
    // ...
    const ast::TokenExpr literal{ .text = "1" };

    // Use std::make_unique<ast::Expr> for both inner and outer
    ast::UnaryExpr inner{ .op = "-", .value = std::make_unique<ast::Expr>(literal) };
    const ast::UnaryExpr outer{ .op = "-", .value = std::make_unique<ast::Expr>(std::move(inner)) };

    const auto result = emit::test::render(outer);
    REQUIRE(result == "--1");
}

TEST_CASE("Optimizer: Rule 3 (HardLines Merge) - Paragraphs", "[pretty_printer][optimizer]")
{
    // Two leading paragraph breaks.
    // printTrivia(P{1}) -> hardlines(1)
    // printTrivia(P{2}) -> hardlines(2)
    // Unoptimized tree: (empty + hardlines(1)) + hardlines(2)
    // Optimized tree: hardlines(3)
    // Final tree: hardlines(3) + core_doc
    ast::TokenExpr token{ .text = "foo" };
    token.trivia = ast::NodeTrivia{
        .leading
        = { ast::ParagraphBreak{ .blank_lines = 1 }, ast::ParagraphBreak{ .blank_lines = 2 } }
    };

    const auto result = emit::test::render(token);
    // 3 hardlines, not 1 then 2
    REQUIRE(result == "\n\n\nfoo");
}

TEST_CASE("Optimizer: Rule 3 (HardLines Merge) - Comment + Paragraph",
          "[pretty_printer][optimizer]")
{
    // A comment followed by a paragraph break.
    // printTrivia(Comment) -> text("--c") + hardline()
    // printTrivia(PBreak)  -> hardlines(1)
    // Unoptimized tree: (empty + (text + hardline)) + hardlines(1)
    // Optimized (bottom-up):
    // 1. (text + hardline) + hardlines(1)
    // 2. hardline + hardlines(1) -> hardlines(2)
    // 3. Final: text("--c") + hardlines(2)
    ast::TokenExpr token{ .text = "foo" };
    token.trivia = ast::NodeTrivia{
        .leading = { ast::Comment{ .text = "--c" }, ast::ParagraphBreak{ .blank_lines = 1 } }
    };

    const auto result = emit::test::render(token);
    // 1 newline from comment, 1 from paragraph, for 2 total
    REQUIRE(result == "--c\n\nfoo");
}
