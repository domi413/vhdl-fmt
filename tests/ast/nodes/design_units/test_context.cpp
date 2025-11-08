#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>

TEST_CASE("makeContextDecl: Basic context declaration (VHDL-2008)", "[design_unit][context]")
{
    constexpr std::string_view VHDL_FILE = R"(
        context MyContext is
            library IEEE;
            use IEEE.std_logic_1164.all;
        end context MyContext;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone):
    // REQUIRE(design.units.size() == 1);
    // auto *ctx = std::get_if<ast::Context>(&design.units[0]);
    // REQUIRE(ctx != nullptr);
    // REQUIRE(ctx->name == "MyContext");
}

TEST_CASE("makeContextDecl: With multiple libraries and use clauses", "[design_unit][context]")
{
    constexpr std::string_view VHDL_FILE = R"(
        context IEEEContext is
            library IEEE;
            use IEEE.std_logic_1164.all;
            use IEEE.numeric_std.all;
            use IEEE.math_real.all;
        end context IEEEContext;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone):
    // auto *ctx = std::get_if<ast::Context>(&design.units[0]);
    // REQUIRE(ctx != nullptr);
}

TEST_CASE("makeContextDecl: Referencing other contexts", "[design_unit][context]")
{
    constexpr std::string_view VHDL_FILE = R"(
        context ExtendedContext is
            context work.BaseContext;
            library IEEE;
            use IEEE.numeric_std.all;
        end context ExtendedContext;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone):
    // auto *ctx = std::get_if<ast::Context>(&design.units[0]);
    // REQUIRE(ctx != nullptr);
}
