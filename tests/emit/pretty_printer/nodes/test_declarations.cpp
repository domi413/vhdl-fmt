#include "ast/nodes/declarations.hpp"
#include "ast/nodes/expressions.hpp"
#include "emit/pretty_printer.hpp"

#include <catch2/catch_test_macros.hpp>

// TODO(user): Should declarations ever be split into multiple lines in VHDL?

TEST_CASE("GenericParam with single name", "[pretty_printer][declarations]")
{
    ast::GenericParam param;
    param.names = { "WIDTH" };
    param.type_name = "integer";

    emit::PrettyPrinter printer;
    const auto doc = printer(param);
    const auto result = doc.render(80);

    REQUIRE(result == "WIDTH : integer");
}

TEST_CASE("GenericParam with multiple names", "[pretty_printer][declarations]")
{
    ast::GenericParam param;
    param.names = { "WIDTH", "HEIGHT", "DEPTH" };
    param.type_name = "positive";

    emit::PrettyPrinter printer;
    const auto doc = printer(param);
    const auto result = doc.render(80);

    REQUIRE(result == "WIDTH, HEIGHT, DEPTH : positive");
}

TEST_CASE("GenericParam with default value", "[pretty_printer][declarations]")
{
    ast::GenericParam param;
    param.names = { "WIDTH" };
    param.type_name = "integer";

    ast::TokenExpr default_val;
    default_val.text = "8";
    param.default_expr = default_val;

    emit::PrettyPrinter printer;
    const auto doc = printer(param);
    const auto result = doc.render(80);

    REQUIRE(result == "WIDTH : integer := 8");
}

TEST_CASE("GenericParam with multiple names and default", "[pretty_printer][declarations]")
{
    ast::GenericParam param;
    param.names = { "A", "B" };
    param.type_name = "natural";

    ast::TokenExpr default_val;
    default_val.text = "0";
    param.default_expr = default_val;

    emit::PrettyPrinter printer;
    const auto doc = printer(param);
    const auto result = doc.render(80);

    REQUIRE(result == "A, B : natural := 0");
}

TEST_CASE("Port with single name and mode", "[pretty_printer][declarations]")
{
    ast::Port port;
    port.names = { "clk" };
    port.mode = "in";
    port.type_name = "std_logic";

    emit::PrettyPrinter printer;
    const auto doc = printer(port);
    const auto result = doc.render(80);

    REQUIRE(result == "clk : in std_logic");
}

TEST_CASE("Port with multiple names", "[pretty_printer][declarations]")
{
    ast::Port port;
    port.names = { "data_in", "data_out" };
    port.mode = "inout";
    port.type_name = "std_logic_vector";

    emit::PrettyPrinter printer;
    const auto doc = printer(port);
    const auto result = doc.render(80);

    REQUIRE(result == "data_in, data_out : inout std_logic_vector");
}

TEST_CASE("Port with constraints", "[pretty_printer][declarations]")
{
    ast::Port port;
    port.names = { "data" };
    port.mode = "in";
    port.type_name = "std_logic_vector";

    // Create constraint: 7 downto 0
    ast::BinaryExpr constraint;
    ast::TokenExpr left_token;
    left_token.text = "7";
    ast::TokenExpr right_token;
    right_token.text = "0";
    auto left = std::make_unique<ast::Expr>(left_token);
    auto right = std::make_unique<ast::Expr>(right_token);
    constraint.left = std::move(left);
    constraint.op = "downto";
    constraint.right = std::move(right);
    port.constraints.push_back(std::move(constraint));

    emit::PrettyPrinter printer;
    const auto doc = printer(port);
    const auto result = doc.render(80);

    REQUIRE(result == "data : in std_logic_vector(7 downto 0)");
}

TEST_CASE("Port with multiple constraints", "[pretty_printer][declarations]")
{
    ast::Port port;
    port.names = { "matrix" };
    port.mode = "out";
    port.type_name = "matrix_type";

    // First constraint: 7 downto 0
    ast::BinaryExpr constraint1;
    ast::TokenExpr left1_token;
    left1_token.text = "7";
    ast::TokenExpr right1_token;
    right1_token.text = "0";
    auto left1 = std::make_unique<ast::Expr>(left1_token);
    auto right1 = std::make_unique<ast::Expr>(right1_token);
    constraint1.left = std::move(left1);
    constraint1.op = "downto";
    constraint1.right = std::move(right1);

    // Second constraint: 3 downto 0
    ast::BinaryExpr constraint2;
    ast::TokenExpr left2_token;
    left2_token.text = "3";
    ast::TokenExpr right2_token;
    right2_token.text = "0";
    auto left2 = std::make_unique<ast::Expr>(left2_token);
    auto right2 = std::make_unique<ast::Expr>(right2_token);
    constraint2.left = std::move(left2);
    constraint2.op = "downto";
    constraint2.right = std::move(right2);

    port.constraints.push_back(std::move(constraint1));
    port.constraints.push_back(std::move(constraint2));

    emit::PrettyPrinter printer;
    const auto doc = printer(port);
    const auto result = doc.render(80);

    REQUIRE(result == "matrix : out matrix_type(7 downto 0, 3 downto 0)");
}

TEST_CASE("Port with default value", "[pretty_printer][declarations]")
{
    ast::Port port;
    port.names = { "enable" };
    port.mode = "in";
    port.type_name = "std_logic";

    ast::TokenExpr default_val;
    default_val.text = "'0'";
    port.default_expr = default_val;

    emit::PrettyPrinter printer;
    const auto doc = printer(port);
    const auto result = doc.render(80);

    REQUIRE(result == "enable : in std_logic := '0'");
}

TEST_CASE("Port with constraints and default value", "[pretty_printer][declarations]")
{
    ast::Port port;
    port.names = { "data" };
    port.mode = "in";
    port.type_name = "std_logic_vector";

    // Constraint
    ast::BinaryExpr constraint;
    ast::TokenExpr left_token;
    left_token.text = "7";
    ast::TokenExpr right_token;
    right_token.text = "0";
    auto left = std::make_unique<ast::Expr>(left_token);
    auto right = std::make_unique<ast::Expr>(right_token);
    constraint.left = std::move(left);
    constraint.op = "downto";
    constraint.right = std::move(right);
    port.constraints.push_back(std::move(constraint));

    // Default value
    ast::TokenExpr default_val;
    default_val.text = "X\"00\"";
    port.default_expr = default_val;

    emit::PrettyPrinter printer;
    const auto doc = printer(port);
    const auto result = doc.render(80);

    REQUIRE(result == "data : in std_logic_vector(7 downto 0) := X\"00\"");
}
