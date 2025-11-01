#include "ast/nodes/declarations.hpp"
#include "ast/nodes/design_file.hpp"
#include "ast/nodes/design_units.hpp"
#include "ast/nodes/expressions.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

// Helper to get expression from signal initialization
namespace {

auto getSignalInitExpr(const ast::DesignFile &design) -> const ast::Expr *
{
    if (design.units.size() < 2) {
        return nullptr;
    }
    const auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    if ((arch == nullptr) || arch->decls.empty()) {
        return nullptr;
    }
    const auto *signal = std::get_if<ast::SignalDecl>(arch->decls.data());
    if ((signal == nullptr) || !signal->init_expr.has_value()) {
        return nullptr;
    }
    return &(*signal->init_expr);
}

} // namespace

TEST_CASE("UnaryExpr: Negation operator", "[expressions][unary]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal x : integer := -42;
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    const auto *expr = getSignalInitExpr(design);
    REQUIRE(expr != nullptr);

    const auto *unary = std::get_if<ast::UnaryExpr>(expr);
    REQUIRE(unary != nullptr);
    REQUIRE(unary->op == "-");

    auto *val = std::get_if<ast::TokenExpr>(unary->value.get());
    REQUIRE(val != nullptr);
    REQUIRE(val->text == "42");
}

TEST_CASE("UnaryExpr: Plus operator", "[expressions][unary]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal x : integer := +42;
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    const auto *expr = getSignalInitExpr(design);
    REQUIRE(expr != nullptr);

    const auto *unary = std::get_if<ast::UnaryExpr>(expr);
    REQUIRE(unary != nullptr);
    REQUIRE(unary->op == "+");

    auto *val = std::get_if<ast::TokenExpr>(unary->value.get());
    REQUIRE(val != nullptr);
    REQUIRE(val->text == "42");
}

TEST_CASE("UnaryExpr: Not operator", "[expressions][unary]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal x : boolean := not true;
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    const auto *expr = getSignalInitExpr(design);
    REQUIRE(expr != nullptr);

    const auto *unary = std::get_if<ast::UnaryExpr>(expr);
    REQUIRE(unary != nullptr);
    REQUIRE(unary->op == "not");

    auto *val = std::get_if<ast::TokenExpr>(unary->value.get());
    REQUIRE(val != nullptr);
    REQUIRE(val->text == "true");
}

TEST_CASE("UnaryExpr: Abs operator", "[expressions][unary]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal x : integer := abs -5;
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    const auto *expr = getSignalInitExpr(design);
    REQUIRE(expr != nullptr);

    const auto *unary = std::get_if<ast::UnaryExpr>(expr);
    REQUIRE(unary != nullptr);
    REQUIRE(unary->op == "abs");

    // The operand should be a UnaryExpr with negation
    auto *inner = std::get_if<ast::UnaryExpr>(unary->value.get());
    REQUIRE(inner != nullptr);
    REQUIRE(inner->op == "-");
}

TEST_CASE("UnaryExpr: Not on identifier", "[expressions][unary]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal x : boolean := not ready;
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    const auto *expr = getSignalInitExpr(design);
    REQUIRE(expr != nullptr);

    const auto *unary = std::get_if<ast::UnaryExpr>(expr);
    REQUIRE(unary != nullptr);
    REQUIRE(unary->op == "not");

    auto *val = std::get_if<ast::TokenExpr>(unary->value.get());
    REQUIRE(val != nullptr);
    REQUIRE(val->text == "ready");
}

TEST_CASE("UnaryExpr: Double negation", "[expressions][unary]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal x : integer := - - 10;
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    const auto *expr = getSignalInitExpr(design);
    REQUIRE(expr != nullptr);

    const auto *unary1 = std::get_if<ast::UnaryExpr>(expr);
    REQUIRE(unary1 != nullptr);
    REQUIRE(unary1->op == "-");

    const auto *unary2 = std::get_if<ast::UnaryExpr>(unary1->value.get());
    REQUIRE(unary2 != nullptr);
    REQUIRE(unary2->op == "-");

    auto *val = std::get_if<ast::TokenExpr>(unary2->value.get());
    REQUIRE(val != nullptr);
    REQUIRE(val->text == "10");
}

// Reduction operators (VHDL-2008)
// TEST_CASE("UnaryExpr: And reduction", "[expressions][unary]")
// {
//     constexpr std::string_view VHDL_FILE = R"(
//         entity E is end E;
//         architecture A of E is
//             signal data : std_logic_vector(7 downto 0);
//             signal result : std_logic := and data;
//         begin
//         end A;
//     )";
//
//     auto design = builder::buildFromString(VHDL_FILE);
//     // TODO: Check reduction operator when implemented
// }

// TEST_CASE("UnaryExpr: Or reduction", "[expressions][unary]")
// {
//     constexpr std::string_view VHDL_FILE = R"(
//         entity E is end E;
//         architecture A of E is
//             signal data : std_logic_vector(7 downto 0);
//             signal result : std_logic := or data;
//         begin
//         end A;
//     )";
//
//     auto design = builder::buildFromString(VHDL_FILE);
//     // TODO: Check reduction operator when implemented
// }

// TEST_CASE("UnaryExpr: Xor reduction", "[expressions][unary]")
// {
//     constexpr std::string_view VHDL_FILE = R"(
//         entity E is end E;
//         architecture A of E is
//             signal data : std_logic_vector(7 downto 0);
//             signal result : std_logic := xor data;
//         begin
//         end A;
//     )";
//
//     auto design = builder::buildFromString(VHDL_FILE);
//     // TODO: Check reduction operator when implemented
// }

// TEST_CASE("UnaryExpr: Nand reduction", "[expressions][unary]")
// {
//     constexpr std::string_view VHDL_FILE = R"(
//         entity E is end E;
//         architecture A of E is
//             signal data : std_logic_vector(7 downto 0);
//             signal result : std_logic := nand data;
//         begin
//         end A;
//     )";
//
//     auto design = builder::buildFromString(VHDL_FILE);
//     // TODO: Check reduction operator when implemented
// }

// TEST_CASE("UnaryExpr: Nor reduction", "[expressions][unary]")
// {
//     constexpr std::string_view VHDL_FILE = R"(
//         entity E is end E;
//         architecture A of E is
//             signal data : std_logic_vector(7 downto 0);
//             signal result : std_logic := nor data;
//         begin
//         end A;
//     )";
//
//     auto design = builder::buildFromString(VHDL_FILE);
//     // TODO: Check reduction operator when implemented
// }

// TEST_CASE("UnaryExpr: Xnor reduction", "[expressions][unary]")
// {
//     constexpr std::string_view VHDL_FILE = R"(
//         entity E is end E;
//         architecture A of E is
//             signal data : std_logic_vector(7 downto 0);
//             signal result : std_logic := xnor data;
//         begin
//         end A;
//     )";
//
//     auto design = builder::buildFromString(VHDL_FILE);
//     // TODO: Check reduction operator when implemented
// }
