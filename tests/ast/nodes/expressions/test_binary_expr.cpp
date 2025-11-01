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

TEST_CASE("BinaryExpr: Addition operator", "[expressions][binary]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal x : integer := 10 + 20;
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    const auto *expr = getSignalInitExpr(design);
    REQUIRE(expr != nullptr);

    const auto *binary = std::get_if<ast::BinaryExpr>(expr);
    REQUIRE(binary != nullptr);
    REQUIRE(binary->op == "+");

    auto *left = std::get_if<ast::TokenExpr>(binary->left.get());
    REQUIRE(left != nullptr);
    REQUIRE(left->text == "10");

    auto *right = std::get_if<ast::TokenExpr>(binary->right.get());
    REQUIRE(right != nullptr);
    REQUIRE(right->text == "20");
}

TEST_CASE("BinaryExpr: Subtraction operator", "[expressions][binary]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal x : integer := 50 - 30;
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    const auto *expr = getSignalInitExpr(design);
    REQUIRE(expr != nullptr);

    const auto *binary = std::get_if<ast::BinaryExpr>(expr);
    REQUIRE(binary != nullptr);
    REQUIRE(binary->op == "-");
}

TEST_CASE("BinaryExpr: Multiplication operator", "[expressions][binary]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal x : integer := 5 * 6;
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    const auto *expr = getSignalInitExpr(design);
    REQUIRE(expr != nullptr);

    const auto *binary = std::get_if<ast::BinaryExpr>(expr);
    REQUIRE(binary != nullptr);
    REQUIRE(binary->op == "*");
}

TEST_CASE("BinaryExpr: Division operator", "[expressions][binary]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal x : integer := 100 / 4;
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    const auto *expr = getSignalInitExpr(design);
    REQUIRE(expr != nullptr);

    const auto *binary = std::get_if<ast::BinaryExpr>(expr);
    REQUIRE(binary != nullptr);
    REQUIRE(binary->op == "/");
}

TEST_CASE("BinaryExpr: Modulo operator", "[expressions][binary]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal x : integer := 10 mod 3;
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    const auto *expr = getSignalInitExpr(design);
    REQUIRE(expr != nullptr);

    const auto *binary = std::get_if<ast::BinaryExpr>(expr);
    REQUIRE(binary != nullptr);
    REQUIRE(binary->op == "mod");
}

TEST_CASE("BinaryExpr: Remainder operator", "[expressions][binary]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal x : integer := 10 rem 3;
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    const auto *expr = getSignalInitExpr(design);
    REQUIRE(expr != nullptr);

    const auto *binary = std::get_if<ast::BinaryExpr>(expr);
    REQUIRE(binary != nullptr);
    REQUIRE(binary->op == "rem");
}

TEST_CASE("BinaryExpr: Exponentiation operator", "[expressions][binary]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal x : integer := 2 ** 8;
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    const auto *expr = getSignalInitExpr(design);
    REQUIRE(expr != nullptr);

    const auto *binary = std::get_if<ast::BinaryExpr>(expr);
    REQUIRE(binary != nullptr);
    REQUIRE(binary->op == "**");
}

TEST_CASE("BinaryExpr: Logical AND operator", "[expressions][binary]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal x : boolean := true and false;
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    const auto *expr = getSignalInitExpr(design);
    REQUIRE(expr != nullptr);

    const auto *binary = std::get_if<ast::BinaryExpr>(expr);
    REQUIRE(binary != nullptr);
    REQUIRE(binary->op == "and");
}

TEST_CASE("BinaryExpr: Logical OR operator", "[expressions][binary]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal x : boolean := true or false;
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    const auto *expr = getSignalInitExpr(design);
    REQUIRE(expr != nullptr);

    const auto *binary = std::get_if<ast::BinaryExpr>(expr);
    REQUIRE(binary != nullptr);
    REQUIRE(binary->op == "or");
}

TEST_CASE("BinaryExpr: Logical XOR operator", "[expressions][binary]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal x : std_logic := a xor b;
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    const auto *expr = getSignalInitExpr(design);
    REQUIRE(expr != nullptr);

    const auto *binary = std::get_if<ast::BinaryExpr>(expr);
    REQUIRE(binary != nullptr);
    REQUIRE(binary->op == "xor");
}

TEST_CASE("BinaryExpr: Logical NAND operator", "[expressions][binary]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal x : std_logic := a nand b;
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    const auto *expr = getSignalInitExpr(design);
    REQUIRE(expr != nullptr);

    const auto *binary = std::get_if<ast::BinaryExpr>(expr);
    REQUIRE(binary != nullptr);
    REQUIRE(binary->op == "nand");
}

TEST_CASE("BinaryExpr: Logical NOR operator", "[expressions][binary]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal x : std_logic := a nor b;
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    const auto *expr = getSignalInitExpr(design);
    REQUIRE(expr != nullptr);

    const auto *binary = std::get_if<ast::BinaryExpr>(expr);
    REQUIRE(binary != nullptr);
    REQUIRE(binary->op == "nor");
}

TEST_CASE("BinaryExpr: Logical XNOR operator", "[expressions][binary]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal x : std_logic := a xnor b;
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    const auto *expr = getSignalInitExpr(design);
    REQUIRE(expr != nullptr);

    const auto *binary = std::get_if<ast::BinaryExpr>(expr);
    REQUIRE(binary != nullptr);
    REQUIRE(binary->op == "xnor");
}

TEST_CASE("BinaryExpr: Equality operator", "[expressions][binary]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal x : boolean := a = b;
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    const auto *expr = getSignalInitExpr(design);
    REQUIRE(expr != nullptr);

    const auto *binary = std::get_if<ast::BinaryExpr>(expr);
    REQUIRE(binary != nullptr);
    REQUIRE(binary->op == "=");
}

TEST_CASE("BinaryExpr: Inequality operator", "[expressions][binary]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal x : boolean := a /= b;
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    const auto *expr = getSignalInitExpr(design);
    REQUIRE(expr != nullptr);

    const auto *binary = std::get_if<ast::BinaryExpr>(expr);
    REQUIRE(binary != nullptr);
    REQUIRE(binary->op == "/=");
}

TEST_CASE("BinaryExpr: Less than operator", "[expressions][binary]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal x : boolean := a < b;
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    const auto *expr = getSignalInitExpr(design);
    REQUIRE(expr != nullptr);

    const auto *binary = std::get_if<ast::BinaryExpr>(expr);
    REQUIRE(binary != nullptr);
    REQUIRE(binary->op == "<");
}

TEST_CASE("BinaryExpr: Less than or equal operator", "[expressions][binary]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal x : boolean := a <= b;
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    const auto *expr = getSignalInitExpr(design);
    REQUIRE(expr != nullptr);

    const auto *binary = std::get_if<ast::BinaryExpr>(expr);
    REQUIRE(binary != nullptr);
    REQUIRE(binary->op == "<=");
}

TEST_CASE("BinaryExpr: Greater than operator", "[expressions][binary]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal x : boolean := a > b;
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    const auto *expr = getSignalInitExpr(design);
    REQUIRE(expr != nullptr);

    const auto *binary = std::get_if<ast::BinaryExpr>(expr);
    REQUIRE(binary != nullptr);
    REQUIRE(binary->op == ">");
}

TEST_CASE("BinaryExpr: Greater than or equal operator", "[expressions][binary]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal x : boolean := a >= b;
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    const auto *expr = getSignalInitExpr(design);
    REQUIRE(expr != nullptr);

    const auto *binary = std::get_if<ast::BinaryExpr>(expr);
    REQUIRE(binary != nullptr);
    REQUIRE(binary->op == ">=");
}

TEST_CASE("BinaryExpr: Range with to", "[expressions][binary]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
            port (data : in std_logic_vector(0 to 7));
        end E;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() == 1);

    auto *entity = std::get_if<ast::Entity>(design.units.data());
    REQUIRE(entity != nullptr);
    REQUIRE(entity->port_clause.ports.size() == 1);

    auto &port = entity->port_clause.ports[0];
    REQUIRE(port.constraints.size() == 1);
    REQUIRE(port.constraints[0].op == "to");
}

TEST_CASE("BinaryExpr: Range with downto", "[expressions][binary]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
            port (data : in std_logic_vector(7 downto 0));
        end E;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() == 1);

    auto *entity = std::get_if<ast::Entity>(design.units.data());
    REQUIRE(entity != nullptr);
    REQUIRE(entity->port_clause.ports.size() == 1);

    auto &port = entity->port_clause.ports[0];
    REQUIRE(port.constraints.size() == 1);
    REQUIRE(port.constraints[0].op == "downto");
}

TEST_CASE("BinaryExpr: Concatenation operator", "[expressions][binary]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal x : std_logic_vector(15 downto 0) := a & b;
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    const auto *expr = getSignalInitExpr(design);
    REQUIRE(expr != nullptr);

    const auto *binary = std::get_if<ast::BinaryExpr>(expr);
    REQUIRE(binary != nullptr);
    REQUIRE(binary->op == "&");
}

TEST_CASE("BinaryExpr: Shift left logical", "[expressions][binary]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal x : std_logic_vector(7 downto 0) := data sll 2;
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    const auto *expr = getSignalInitExpr(design);
    REQUIRE(expr != nullptr);

    const auto *binary = std::get_if<ast::BinaryExpr>(expr);
    REQUIRE(binary != nullptr);
    REQUIRE(binary->op == "sll");
}

TEST_CASE("BinaryExpr: Shift right logical", "[expressions][binary]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal x : std_logic_vector(7 downto 0) := data srl 2;
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    const auto *expr = getSignalInitExpr(design);
    REQUIRE(expr != nullptr);

    const auto *binary = std::get_if<ast::BinaryExpr>(expr);
    REQUIRE(binary != nullptr);
    REQUIRE(binary->op == "srl");
}

TEST_CASE("BinaryExpr: Shift left arithmetic", "[expressions][binary]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal x : std_logic_vector(7 downto 0) := data sla 2;
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    const auto *expr = getSignalInitExpr(design);
    REQUIRE(expr != nullptr);

    const auto *binary = std::get_if<ast::BinaryExpr>(expr);
    REQUIRE(binary != nullptr);
    REQUIRE(binary->op == "sla");
}

TEST_CASE("BinaryExpr: Shift right arithmetic", "[expressions][binary]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal x : std_logic_vector(7 downto 0) := data sra 2;
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    const auto *expr = getSignalInitExpr(design);
    REQUIRE(expr != nullptr);

    const auto *binary = std::get_if<ast::BinaryExpr>(expr);
    REQUIRE(binary != nullptr);
    REQUIRE(binary->op == "sra");
}

TEST_CASE("BinaryExpr: Rotate left", "[expressions][binary]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal x : std_logic_vector(7 downto 0) := data rol 2;
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    const auto *expr = getSignalInitExpr(design);
    REQUIRE(expr != nullptr);

    const auto *binary = std::get_if<ast::BinaryExpr>(expr);
    REQUIRE(binary != nullptr);
    REQUIRE(binary->op == "rol");
}

TEST_CASE("BinaryExpr: Rotate right", "[expressions][binary]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal x : std_logic_vector(7 downto 0) := data ror 2;
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    const auto *expr = getSignalInitExpr(design);
    REQUIRE(expr != nullptr);

    const auto *binary = std::get_if<ast::BinaryExpr>(expr);
    REQUIRE(binary != nullptr);
    REQUIRE(binary->op == "ror");
}

// TEST_CASE("BinaryExpr: Chained binary expressions", "[expressions][binary]")
// {
//     constexpr std::string_view VHDL_FILE = R"(
//         entity E is end E;
//         architecture A of E is
//             signal x : integer := a + b + c;
//         begin
//         end A;
//     )";

//     auto design = builder::buildFromString(VHDL_FILE);
//     const auto *expr = getSignalInitExpr(design);
//     REQUIRE(expr != nullptr);

//     // The outer expression should be a binary expression
//     const auto *binary = std::get_if<ast::BinaryExpr>(expr);
//     REQUIRE(binary != nullptr);
//     REQUIRE(binary->op == "+");

//     // The left side should also be a binary expression
//     const auto *left_binary = std::get_if<ast::BinaryExpr>(binary->left.get());
//     REQUIRE(left_binary != nullptr);
//     REQUIRE(left_binary->op == "+");
// }

TEST_CASE("BinaryExpr: Mixed operators with precedence", "[expressions][binary]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal x : integer := a + b * c;
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    const auto *expr = getSignalInitExpr(design);
    REQUIRE(expr != nullptr);

    // The outer expression should be addition
    const auto *binary = std::get_if<ast::BinaryExpr>(expr);
    REQUIRE(binary != nullptr);
    REQUIRE(binary->op == "+");

    // The right side should be multiplication (higher precedence)
    const auto *right_binary = std::get_if<ast::BinaryExpr>(binary->right.get());
    REQUIRE(right_binary != nullptr);
    REQUIRE(right_binary->op == "*");
}
