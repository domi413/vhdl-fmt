#include "ast/nodes/design_file.hpp"
#include "ast/nodes/design_units.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

TEST_CASE("AssociationList: Positional associations in function call",
          "[lists_collections][association_list]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            function add(a, b : integer) return integer is
            begin
                return a + b;
            end function;
        begin
            process
                variable result : integer;
            begin
                result := add(5, 10);
            end process;
        end A;
    )";

    const auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() == 2);

    const auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);
    REQUIRE(arch->stmts.size() == 1);

    const auto *proc = std::get_if<ast::Process>(&arch->stmts[0]);
    REQUIRE(proc != nullptr);
    REQUIRE(proc->body.size() == 1);

    const auto *assign = std::get_if<ast::SequentialAssign>(&proc->body[0]);
    REQUIRE(assign != nullptr);

    const auto *name = std::get_if<ast::Name>(&assign->value);
    REQUIRE(name != nullptr);
    REQUIRE(name->parts.size() == 2);
    REQUIRE(name->parts[1].index() == 1); // FunctionCall
    const auto &call = std::get<ast::FunctionCall>(name->parts[1]);
    REQUIRE(call.args.size() == 2);
    // Check positional associations
    REQUIRE(call.args[0].index() == 0); // PositionalAssociation
    const auto &assoc0 = std::get<ast::PositionalAssociation>(call.args[0]);
    REQUIRE(assoc0.expr.has_value());
    const auto *expr0 = std::get_if<ast::TokenExpr>(&assoc0.expr);
    REQUIRE(expr0 != nullptr);
    REQUIRE(expr0->text == "5");

    REQUIRE(call.args[1].index() == 0); // PositionalAssociation
    const auto &assoc1 = std::get<ast::PositionalAssociation>(call.args[1]);
    REQUIRE(assoc1.expr.has_value());
    const auto *expr1 = std::get_if<ast::TokenExpr>(&assoc1.expr);
    REQUIRE(expr1 != nullptr);
    REQUIRE(expr1->text == "10");
}

TEST_CASE("AssociationList: Named associations in port map",
          "[lists_collections][association_list]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            component MyComp is
                port (a : in std_logic; b : out std_logic);
            end component;
        begin
            u1 : MyComp port map (a => '1', b => open);
        end A;
    )";

    const auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() == 2);

    const auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);
    REQUIRE(arch->stmts.size() == 1);

    const auto *inst = std::get_if<ast::ComponentInstantiationStatement>(&arch->stmts[0]);
    REQUIRE(inst != nullptr);
    REQUIRE(inst->port_map.has_value());
    REQUIRE(inst->port_map->associations.size() == 2);
    // Check named associations
    REQUIRE(inst->port_map->associations[0].index() == 1); // NamedAssociation
    const auto &assoc0 = std::get<ast::NamedAssociation>(inst->port_map->associations[0]);
    REQUIRE(assoc0.choices.size() == 1);
    REQUIRE(assoc0.choices[0].index() == 0); // SimpleChoice
    const auto &choice0 = std::get<ast::SimpleChoice>(assoc0.choices[0]);
    REQUIRE(choice0.name == "a");
    REQUIRE(assoc0.expr.has_value());
    const auto *expr0 = std::get_if<ast::TokenExpr>(&assoc0.expr);
    REQUIRE(expr0 != nullptr);
    REQUIRE(expr0->text == "'1'");

    REQUIRE(inst->port_map->associations[1].index() == 1); // NamedAssociation
    const auto &assoc1 = std::get<ast::NamedAssociation>(inst->port_map->associations[1]);
    REQUIRE(assoc1.choices.size() == 1);
    REQUIRE(assoc1.choices[0].index() == 0); // SimpleChoice
    const auto &choice1 = std::get<ast::SimpleChoice>(assoc1.choices[0]);
    REQUIRE(choice1.name == "b");
    REQUIRE(assoc1.expr.has_value());
    const auto *expr1 = std::get_if<ast::TokenExpr>(&assoc1.expr);
    REQUIRE(expr1 != nullptr);
    REQUIRE(expr1->text == "open");
}

TEST_CASE("AssociationList: Mixed positional and named associations",
          "[lists_collections][association_list]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            procedure my_proc(a : in integer; b : out integer; c : in integer) is
            begin
                b := a + c;
            end procedure;
        begin
            process
            begin
                my_proc(1, b => result, c => 2);
            end process;
        end A;
    )";

    const auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() == 2);

    const auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);
    REQUIRE(arch->stmts.size() == 1);

    const auto *proc = std::get_if<ast::Process>(&arch->stmts[0]);
    REQUIRE(proc != nullptr);
    REQUIRE(proc->body.size() == 1);

    const auto *call_stmt = std::get_if<ast::ProcedureCallStatement>(&proc->body[0]);
    REQUIRE(call_stmt != nullptr);
    REQUIRE(call_stmt->args.size() == 3);
    // Check mixed associations
    REQUIRE(call_stmt->args[0].index() == 0); // PositionalAssociation
    const auto &assoc0 = std::get<ast::PositionalAssociation>(call_stmt->args[0]);
    REQUIRE(assoc0.expr.has_value());
    const auto *expr0 = std::get_if<ast::TokenExpr>(&assoc0.expr);
    REQUIRE(expr0 != nullptr);
    REQUIRE(expr0->text == "1");

    REQUIRE(call_stmt->args[1].index() == 1); // NamedAssociation
    const auto &assoc1 = std::get<ast::NamedAssociation>(call_stmt->args[1]);
    REQUIRE(assoc1.choices.size() == 1);
    REQUIRE(assoc1.choices[0].index() == 0); // SimpleChoice
    const auto &choice1 = std::get<ast::SimpleChoice>(assoc1.choices[0]);
    REQUIRE(choice1.name == "b");
    REQUIRE(assoc1.expr.has_value());
    const auto *expr1 = std::get_if<ast::Name>(&assoc1.expr);
    REQUIRE(expr1 != nullptr);

    REQUIRE(call_stmt->args[2].index() == 1); // NamedAssociation
    const auto &assoc2 = std::get<ast::NamedAssociation>(call_stmt->args[2]);
    REQUIRE(assoc2.choices.size() == 1);
    REQUIRE(assoc2.choices[0].index() == 0); // SimpleChoice
    const auto &choice2 = std::get<ast::SimpleChoice>(assoc2.choices[0]);
    REQUIRE(choice2.name == "c");
    REQUIRE(assoc2.expr.has_value());
    const auto *expr2 = std::get_if<ast::TokenExpr>(&assoc2.expr);
    REQUIRE(expr2 != nullptr);
    REQUIRE(expr2->text == "2");
}
