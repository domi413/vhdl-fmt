#include "ast/nodes/design_units.hpp"
#include "builder/ast_builder.hpp"
#include "common/test_utils.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

// -----------------------------------------------------------------------------
// Basic Context Declaration
// -----------------------------------------------------------------------------
TEST_CASE("Context: Groups library and use clauses for reuse (VHDL-2008)",
          "[context][design_unit][vhdl2008][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        context MyContext is
            library IEEE;
            use IEEE.std_logic_1164.all;
        end context MyContext;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() == 1);

    // TODO: Uncomment when Context design unit is implemented
    // auto *ctx = std::get_if<ast::Context>(&design.units[0]);
    // REQUIRE(ctx != nullptr);
    // REQUIRE(ctx->name == "MyContext");
    // REQUIRE(ctx->end_label.has_value());
    // REQUIRE(ctx->end_label.value() == "MyContext");
}

// -----------------------------------------------------------------------------
// Context with Multiple Library Clauses
// -----------------------------------------------------------------------------
TEST_CASE("Context: Captures multiple library clauses", "[context][libraries][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        context StandardContext is
            library IEEE;
            library STD;
            library WORK;
        end context StandardContext;
    )";

    auto design = builder::buildFromString(VHDL_FILE);

    // TODO: Uncomment when Context design unit is implemented
    // auto *ctx = std::get_if<ast::Context>(&design.units[0]);
    // REQUIRE(ctx != nullptr);
    // REQUIRE(ctx->name == "StandardContext");
    // REQUIRE(ctx->library_clauses.size() >= 3);
}

// -----------------------------------------------------------------------------
// Context with Use Clauses
// -----------------------------------------------------------------------------
TEST_CASE("Context: Captures use clauses", "[context][use_clauses][.future]")
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

    // TODO: Uncomment when Context design unit is implemented
    // auto *ctx = std::get_if<ast::Context>(&design.units[0]);
    // REQUIRE(ctx != nullptr);
    // REQUIRE(ctx->name == "IEEEContext");
    // REQUIRE(ctx->use_clauses.size() >= 3);
}

// -----------------------------------------------------------------------------
// Context with Mixed Clauses
// -----------------------------------------------------------------------------
TEST_CASE("Context: Handles mixed library and use clauses", "[context][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        context MixedContext is
            library IEEE;
            use IEEE.std_logic_1164.all;
            library work;
            use work.my_package.all;
            library custom_lib;
            use custom_lib.types.all;
        end context MixedContext;
    )";

    auto design = builder::buildFromString(VHDL_FILE);

    // TODO: Uncomment when Context design unit is implemented
    // auto *ctx = std::get_if<ast::Context>(&design.units[0]);
    // REQUIRE(ctx != nullptr);
    // REQUIRE(ctx->name == "MixedContext");
}

// -----------------------------------------------------------------------------
// Context without End Label
// -----------------------------------------------------------------------------
TEST_CASE("Context: Can omit end label", "[context][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        context SimpleContext is
            library IEEE;
            use IEEE.std_logic_1164.all;
        end context;
    )";

    auto design = builder::buildFromString(VHDL_FILE);

    // TODO: Uncomment when Context design unit is implemented
    // auto *ctx = std::get_if<ast::Context>(&design.units[0]);
    // REQUIRE(ctx != nullptr);
    // REQUIRE(ctx->name == "SimpleContext");
}

// -----------------------------------------------------------------------------
// Context without 'context' keyword at end
// -----------------------------------------------------------------------------
TEST_CASE("Context: Can omit 'context' keyword at end", "[context][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        context AltContext is
            library IEEE;
            use IEEE.std_logic_1164.all;
        end AltContext;
    )";

    auto design = builder::buildFromString(VHDL_FILE);

    // TODO: Uncomment when Context design unit is implemented
    // auto *ctx = std::get_if<ast::Context>(&design.units[0]);
    // REQUIRE(ctx != nullptr);
    // REQUIRE(ctx->name == "AltContext");
}

// -----------------------------------------------------------------------------
// Minimal Context
// -----------------------------------------------------------------------------
TEST_CASE("Context: Minimal context with single use clause", "[context][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        context MinContext is
            use work.types.all;
        end context MinContext;
    )";

    auto design = builder::buildFromString(VHDL_FILE);

    // TODO: Uncomment when Context design unit is implemented
    // auto *ctx = std::get_if<ast::Context>(&design.units[0]);
    // REQUIRE(ctx != nullptr);
    // REQUIRE(ctx->name == "MinContext");
}

// -----------------------------------------------------------------------------
// Context with Specific Imports
// -----------------------------------------------------------------------------
TEST_CASE("Context: Supports specific (non-all) imports", "[context][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        context SpecificContext is
            library IEEE;
            use IEEE.std_logic_1164.std_logic;
            use IEEE.std_logic_1164.std_logic_vector;
            use work.my_package.my_type;
        end context SpecificContext;
    )";

    auto design = builder::buildFromString(VHDL_FILE);

    // TODO: Uncomment when Context design unit is implemented
    // auto *ctx = std::get_if<ast::Context>(&design.units[0]);
    // REQUIRE(ctx != nullptr);
    // REQUIRE(ctx->name == "SpecificContext");
}

// -----------------------------------------------------------------------------
// Context Referencing Other Contexts
// -----------------------------------------------------------------------------
TEST_CASE("Context: Can reference other contexts", "[context][vhdl2008][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        context ExtendedContext is
            context work.BaseContext;
            library IEEE;
            use IEEE.numeric_std.all;
        end context ExtendedContext;
    )";

    auto design = builder::buildFromString(VHDL_FILE);

    // TODO: Uncomment when Context design unit is implemented
    // auto *ctx = std::get_if<ast::Context>(&design.units[0]);
    // REQUIRE(ctx != nullptr);
    // REQUIRE(ctx->name == "ExtendedContext");
}

// -----------------------------------------------------------------------------
// Complex Context with Many Clauses
// -----------------------------------------------------------------------------
TEST_CASE("Context: Handles complex context with many clauses", "[context][.future]")
{
    constexpr std::string_view VHDL_FILE = R"(
        context ComplexContext is
            library IEEE;
            use IEEE.std_logic_1164.all;
            use IEEE.numeric_std.all;
            use IEEE.math_real.all;
            library work;
            use work.constants.all;
            use work.types.all;
            use work.functions.all;
            library custom;
            use custom.utilities.all;
        end context ComplexContext;
    )";

    auto design = builder::buildFromString(VHDL_FILE);

    // TODO: Uncomment when Context design unit is implemented
    // auto *ctx = std::get_if<ast::Context>(&design.units[0]);
    // REQUIRE(ctx != nullptr);
    // REQUIRE(ctx->name == "ComplexContext");
}
