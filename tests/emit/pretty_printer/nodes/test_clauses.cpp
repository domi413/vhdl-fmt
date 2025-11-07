#include "ast/nodes/design_units.hpp"
#include "ast/nodes/expressions.hpp"
#include "emit/pretty_printer.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Empty GenericClause", "[pretty_printer][clauses]")
{
    ast::GenericClause clause;
    // Empty generics list

    emit::PrettyPrinter printer;
    const auto doc = printer(clause);
    const auto result = doc.render(80);

    REQUIRE(result.empty());
}

TEST_CASE("GenericClause with single parameter", "[pretty_printer][clauses]")
{
    ast::GenericClause clause;

    ast::GenericParam param;
    param.names = { "WIDTH" };
    param.type_name = "integer";
    ast::TokenExpr default_val;
    default_val.text = "8";
    param.default_expr = default_val;

    clause.generics.push_back(std::move(param));

    emit::PrettyPrinter printer;
    const auto doc = printer(clause);
    const auto result = doc.render(80);

    // When it fits on one line, it should be compact
    REQUIRE(result == "generic ( WIDTH : integer := 8 );");
}

TEST_CASE("GenericClause with multiple parameters", "[pretty_printer][clauses]")
{
    ast::GenericClause clause;

    ast::GenericParam param1;
    param1.names = { "WIDTH" };
    param1.type_name = "positive";
    ast::TokenExpr default1;
    default1.text = "8";
    param1.default_expr = default1;

    ast::GenericParam param2;
    param2.names = { "HEIGHT" };
    param2.type_name = "positive";
    ast::TokenExpr default2;
    default2.text = "16";
    param2.default_expr = default2;

    clause.generics.push_back(std::move(param1));
    clause.generics.push_back(std::move(param2));

    emit::PrettyPrinter printer;
    const auto doc = printer(clause);
    const auto result = doc.render(80);

    REQUIRE(result == "generic ( WIDTH : positive := 8; HEIGHT : positive := 16 );");
}

TEST_CASE("Empty PortClause", "[pretty_printer][clauses]")
{
    ast::PortClause clause;
    // Empty ports list

    emit::PrettyPrinter printer;
    const auto doc = printer(clause);
    const auto result = doc.render(80);

    REQUIRE(result.empty());
}

TEST_CASE("PortClause with single port", "[pretty_printer][clauses]")
{
    ast::PortClause clause;

    ast::Port port;
    port.names = { "clk" };
    port.mode = "in";
    port.type_name = "std_logic";

    clause.ports.push_back(std::move(port));

    emit::PrettyPrinter printer;
    const auto doc = printer(clause);
    const auto result = doc.render(80);

    REQUIRE(result == "port ( clk : in std_logic );");
}
TEST_CASE("PortClause with multiple ports", "[pretty_printer][clauses]")
{
    ast::PortClause clause;

    ast::Port port1;
    port1.names = { "clk" };
    port1.mode = "in";
    port1.type_name = "std_logic";

    ast::Port port2;
    port2.names = { "reset" };
    port2.mode = "in";
    port2.type_name = "std_logic";

    ast::Port port3;
    port3.names = { "data_out" };
    port3.mode = "out";
    port3.type_name = "std_logic_vector";

    // Add constraint to port3
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
    port3.constraints.push_back(std::move(constraint));

    clause.ports.push_back(std::move(port1));
    clause.ports.push_back(std::move(port2));
    clause.ports.push_back(std::move(port3));

    emit::PrettyPrinter printer;
    const auto doc = printer(clause);
    const auto result = doc.render(80);

    const std::string expected = "port (\n"
                                 "  clk : in std_logic;\n"
                                 "  reset : in std_logic;\n"
                                 "  data_out : out std_logic_vector(7 downto 0)\n"
                                 ");";
    REQUIRE(result == expected);
}
