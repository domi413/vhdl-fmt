#include "ast/nodes/declarations.hpp"
#include "ast/nodes/design_file.hpp"
#include "ast/nodes/design_units.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

TEST_CASE("SignalDecl: Single signal with type", "[declarations][signal]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal temp : std_logic;
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() == 2);

    auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);
    REQUIRE(arch->decls.size() == 1);

    auto *signal = std::get_if<ast::SignalDecl>(arch->decls.data());
    REQUIRE(signal != nullptr);
    REQUIRE(signal->names.size() == 1);
    REQUIRE(signal->names[0] == "temp");
    REQUIRE(signal->type_name == "std_logic");
    REQUIRE_FALSE(signal->init_expr.has_value());
}

TEST_CASE("SignalDecl: Signal with initialization", "[declarations][signal]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal count : integer := 42;
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);
    REQUIRE(arch->decls.size() == 1);

    auto *signal = std::get_if<ast::SignalDecl>(arch->decls.data());
    REQUIRE(signal != nullptr);
    REQUIRE(signal->names.size() == 1);
    REQUIRE(signal->names[0] == "count");
    REQUIRE(signal->type_name == "integer");
    REQUIRE(signal->init_expr.has_value());
}

TEST_CASE("SignalDecl: Multiple signals same declaration", "[declarations][signal]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal clk, rst, enable : std_logic;
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);
    REQUIRE(arch->decls.size() == 1);

    auto *signal = std::get_if<ast::SignalDecl>(arch->decls.data());
    REQUIRE(signal != nullptr);
    REQUIRE(signal->names.size() == 3);
    REQUIRE(signal->names[0] == "clk");
    REQUIRE(signal->names[1] == "rst");
    REQUIRE(signal->names[2] == "enable");
    REQUIRE(signal->type_name == "std_logic");
}

TEST_CASE("SignalDecl: Vector signal with constraints", "[declarations][signal]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal data : std_logic_vector(7 downto 0);
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);
    REQUIRE(arch->decls.size() == 1);

    auto *signal = std::get_if<ast::SignalDecl>(arch->decls.data());
    REQUIRE(signal != nullptr);
    REQUIRE(signal->names[0] == "data");
    REQUIRE(signal->type_name == "std_logic_vector");
    REQUIRE(signal->constraints.size() == 1);
}

TEST_CASE("SignalDecl: Vector with aggregate initialization", "[declarations][signal]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal data : std_logic_vector(7 downto 0) := (others => '0');
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);
    REQUIRE(arch->decls.size() == 1);

    auto *signal = std::get_if<ast::SignalDecl>(arch->decls.data());
    REQUIRE(signal != nullptr);
    REQUIRE(signal->names[0] == "data");
    REQUIRE(signal->type_name == "std_logic_vector");
    REQUIRE(signal->init_expr.has_value());
}

TEST_CASE("SignalDecl: Multiple vector signals", "[declarations][signal]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            signal data_in : std_logic_vector(7 downto 0);
            signal data_out : std_logic_vector(15 downto 0) := (others => '0');
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);
    REQUIRE(arch->decls.size() == 2);

    auto *signal1 = std::get_if<ast::SignalDecl>(arch->decls.data());
    REQUIRE(signal1 != nullptr);
    REQUIRE(signal1->names[0] == "data_in");
    REQUIRE_FALSE(signal1->init_expr.has_value());

    auto *signal2 = std::get_if<ast::SignalDecl>(&arch->decls[1]);
    REQUIRE(signal2 != nullptr);
    REQUIRE(signal2->names[0] == "data_out");
    REQUIRE(signal2->init_expr.has_value());
}
