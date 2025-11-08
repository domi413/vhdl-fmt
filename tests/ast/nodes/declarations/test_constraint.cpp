#include "ast/nodes/declarations.hpp"
#include "ast/nodes/design_file.hpp"
#include "ast/nodes/design_units.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

TEST_CASE("Signal with index constraint (array)", "[declarations][constraints]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal data : std_logic_vector(7 downto 0);
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::ArchitectureBody>(&design.units[1]);
    REQUIRE(arch != nullptr);
    REQUIRE(arch->decls.size() == 1);

    auto *signal = std::get_if<ast::SignalDecl>(arch->decls.data());
    REQUIRE(signal != nullptr);
    REQUIRE(signal->names[0] == "data");
    REQUIRE(signal->type_name == "std_logic_vector");
    REQUIRE(signal->constraints.size() == 1);

    // Check the constraint is "7 downto 0"
    const auto &constraint = signal->constraints[0];
    REQUIRE(constraint.op == "downto");
}

TEST_CASE("Port with index constraint", "[declarations][constraints]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is
            port (
                data_in : in std_logic_vector(15 downto 0)
            );
        end E;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *entity = std::get_if<ast::EntityDecl>(design.units.data());
    REQUIRE(entity != nullptr);
    REQUIRE(entity->port_clause.ports.size() == 1);

    const auto &port = entity->port_clause.ports[0];
    REQUIRE(port.names[0] == "data_in");
    REQUIRE(port.type_name == "std_logic_vector");
    REQUIRE(port.constraints.size() == 1);

    // Check the constraint is "15 downto 0"
    const auto &constraint = port.constraints[0];
    REQUIRE(constraint.op == "downto");
}

TEST_CASE("Signal with 'to' direction constraint", "[declarations][constraints]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal vec : bit_vector(0 to 7);
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::ArchitectureBody>(&design.units[1]);
    REQUIRE(arch != nullptr);
    REQUIRE(arch->decls.size() == 1);

    auto *signal = std::get_if<ast::SignalDecl>(arch->decls.data());
    REQUIRE(signal != nullptr);
    REQUIRE(signal->type_name == "bit_vector");
    REQUIRE(signal->constraints.size() == 1);

    // Check the constraint is "0 to 7"
    const auto &constraint = signal->constraints[0];
    REQUIRE(constraint.op == "to");
}
