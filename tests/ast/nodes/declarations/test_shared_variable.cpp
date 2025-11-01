#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>

TEST_CASE("SharedVariable: Shared variable declaration", "[declarations][shared_variable]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            shared variable count : integer := 0;
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check shared variable when implemented
}

TEST_CASE("SharedVariable: Shared variable in package", "[declarations][shared_variable]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package P is
            shared variable global_count : integer := 0;
        end P;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check shared variable in package when implemented
}

TEST_CASE("SharedVariable: Shared variable without initialization",
          "[declarations][shared_variable]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity E is end E;
        architecture A of E is
            shared variable temp : std_logic_vector(7 downto 0);
        begin
        end A;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check shared variable without init when implemented
}

TEST_CASE("SharedVariable: Multiple shared variables", "[declarations][shared_variable]")
{
    constexpr std::string_view VHDL_FILE = R"(
        package P is
            shared variable a, b : integer := 0;
        end P;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Check multiple shared variables when implemented
}
