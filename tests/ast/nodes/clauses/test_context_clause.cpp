#include "ast/nodes/design_file.hpp"
#include "ast/nodes/design_units.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

TEST_CASE("ContextDeclaration: Simple context with library clause", "[clauses][context_clause]")
{
    constexpr std::string_view VHDL_FILE = R"(
        context my_context is
            library ieee;
        end context my_context;
    )";

    const auto design = builder::buildFromString(VHDL_FILE);
    REQUIRE(design.units.size() == 1);

    const auto *context = std::get_if<ast::ContextDeclaration>(&design.units[0]);
    REQUIRE(context != nullptr);
    REQUIRE(context->name == "my_context");
    REQUIRE(context->items.size() == 1); // Changed from decls to items

    const auto *lib_clause
      = std::get_if<ast::LibraryClause>(&context->items[0]); // Changed from decls to items
    REQUIRE(lib_clause != nullptr);
    REQUIRE(lib_clause->logical_names.size() == 1);
    REQUIRE(lib_clause->logical_names[0] == "ieee");
}

// TEST_CASE("ContextDeclaration: Context with library and use clauses",
// "[clauses][context_clause]")
// {
//     constexpr std::string_view VHDL_FILE = R"(
//         context my_context is
//             library ieee;
//             use ieee.std_logic_1164.all;
//         end context my_context;
//     )";

//     const auto design = builder::buildFromString(VHDL_FILE);
//     REQUIRE(design.units.size() == 1);

//     const auto *context = std::get_if<ast::ContextDeclaration>(&design.units[0]);
//     REQUIRE(context != nullptr);
//     REQUIRE(context->name == "my_context");
//     REQUIRE(context->context_clause.items.size() == 2);

//     const auto *lib_clause = std::get_if<ast::LibraryClause>(&context->context_clause.items[0]);
//     REQUIRE(lib_clause != nullptr);
//     REQUIRE(lib_clause->logical_names.size() == 1);
//     REQUIRE(lib_clause->logical_names[0] == "ieee");

//     const auto *use_clause = std::get_if<ast::UseClause>(&context->context_clause.items[1]);
//     REQUIRE(use_clause != nullptr);
//     REQUIRE(use_clause->selected_names.size() == 1);
//     REQUIRE(use_clause->selected_names[0] == "ieee.std_logic_1164.all");
// }

// TEST_CASE("ContextDeclaration: Complex context with multiple clauses",
// "[clauses][context_clause]")
// {
//     constexpr std::string_view VHDL_FILE = R"(
//         context complex_context is
//             library ieee, std;
//             use ieee.std_logic_1164.all;
//             use std.textio.all;
//         end context complex_context;
//     )";

//     const auto design = builder::buildFromString(VHDL_FILE);
//     REQUIRE(design.units.size() == 1);

//     const auto *context = std::get_if<ast::ContextDeclaration>(&design.units[0]);
//     REQUIRE(context != nullptr);
//     REQUIRE(context->name == "complex_context");
//     REQUIRE(context->context_clause.items.size() == 3);

//     const auto *lib_clause = std::get_if<ast::LibraryClause>(&context->context_clause.items[0]);
//     REQUIRE(lib_clause != nullptr);
//     REQUIRE(lib_clause->logical_names.size() == 2);
//     REQUIRE(lib_clause->logical_names[0] == "ieee");
//     REQUIRE(lib_clause->logical_names[1] == "std");

//     const auto *use_clause1 = std::get_if<ast::UseClause>(&context->context_clause.items[1]);
//     REQUIRE(use_clause1 != nullptr);
//     REQUIRE(use_clause1->selected_names.size() == 1);
//     REQUIRE(use_clause1->selected_names[0] == "ieee.std_logic_1164.all");

//     const auto *use_clause2 = std::get_if<ast::UseClause>(&context->context_clause.items[2]);
//     REQUIRE(use_clause2 != nullptr);
//     REQUIRE(use_clause2->selected_names.size() == 1);
//     REQUIRE(use_clause2->selected_names[0] == "std.textio.all");
// }
