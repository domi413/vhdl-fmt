#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>

TEST_CASE("Context: Basic context declaration (VHDL-2008)", "[context][design_unit][vhdl2008]")
{
    constexpr std::string_view VHDL_FILE = R"(
        context MyContext is
            library IEEE;
            use IEEE.std_logic_1164.all;
        end context MyContext;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // REQUIRE(design.units.size() == 1);
    // auto *ctx = std::get_if<ast::Context>(&design.units[0]);
    // REQUIRE(ctx != nullptr);
    // REQUIRE(ctx->name == "MyContext");
}

TEST_CASE("Context: With multiple libraries and use clauses", "[context][vhdl2008]")
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
    // auto *ctx = std::get_if<ast::Context>(&design.units[0]);
    // REQUIRE(ctx != nullptr);
}

TEST_CASE("Context: Referencing other contexts", "[context][vhdl2008]")
{
    constexpr std::string_view VHDL_FILE = R"(
        context ExtendedContext is
            context work.BaseContext;
            library IEEE;
            use IEEE.numeric_std.all;
        end context ExtendedContext;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // auto *ctx = std::get_if<ast::Context>(&design.units[0]);
    // REQUIRE(ctx != nullptr);
}
