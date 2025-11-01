#include "ast/nodes/design_units.hpp"
#include "ast/nodes/statements.hpp"
#include "builder/ast_builder.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <variant>

TEST_CASE("WhileLoop: Simple while loop", "[statements][while_loop]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                while count < 10 loop
                    count := count + 1;
                end loop;
            end process;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);

    auto *proc = std::get_if<ast::Process>(arch->stmts.data());
    REQUIRE(proc != nullptr);
    REQUIRE_FALSE(proc->body.empty());

    auto *while_loop = std::get_if<ast::WhileLoop>(proc->body.data());
    REQUIRE(while_loop != nullptr);
}

TEST_CASE("WhileLoop: While loop with comparison condition", "[statements][while_loop]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                while index <= max_value loop
                    data(index) := '0';
                    index := index + 1;
                end loop;
            end process;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);

    auto *proc = std::get_if<ast::Process>(arch->stmts.data());
    REQUIRE(proc != nullptr);
    REQUIRE_FALSE(proc->body.empty());

    auto *while_loop = std::get_if<ast::WhileLoop>(proc->body.data());
    REQUIRE(while_loop != nullptr);
}

TEST_CASE("WhileLoop: While loop with boolean condition", "[statements][while_loop]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                while not done loop
                    process_data;
                    check_status;
                end loop;
            end process;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);

    auto *proc = std::get_if<ast::Process>(arch->stmts.data());
    REQUIRE(proc != nullptr);
    REQUIRE_FALSE(proc->body.empty());

    auto *while_loop = std::get_if<ast::WhileLoop>(proc->body.data());
    REQUIRE(while_loop != nullptr);
}

TEST_CASE("WhileLoop: While loop with logical operators", "[statements][while_loop]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                while enable = '1' and ready = '1' loop
                    transfer_data;
                end loop;
            end process;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);

    auto *proc = std::get_if<ast::Process>(arch->stmts.data());
    REQUIRE(proc != nullptr);
    REQUIRE_FALSE(proc->body.empty());

    auto *while_loop = std::get_if<ast::WhileLoop>(proc->body.data());
    REQUIRE(while_loop != nullptr);
}

// TEST_CASE("WhileLoop: While loop with multiple statements", "[statements][while_loop]")
// {
//     constexpr std::string_view VHDL_FILE = R"(
//         entity Test is end Test;
//         architecture RTL of Test is
//         begin
//             process
//             begin
//                 while counter < limit loop
//                     temp := data_in;
//                     result := temp + offset;
//                     counter := counter + 1;
//                     valid := '1';
//                 end loop;
//             end process;
//         end RTL;
//     )";

//     auto design = builder::buildFromString(VHDL_FILE);
//     auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
//     REQUIRE(arch != nullptr);

//     auto *proc = std::get_if<ast::Process>(arch->stmts.data());
//     REQUIRE(proc != nullptr);
//     REQUIRE_FALSE(proc->body.empty());

//     auto *while_loop = std::get_if<ast::WhileLoop>(proc->body.data());
//     REQUIRE(while_loop != nullptr);
//     REQUIRE_FALSE(while_loop->body.empty());
// }

TEST_CASE("WhileLoop: Nested while loops", "[statements][while_loop]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                while outer_count < 10 loop
                    while inner_count < 5 loop
                        result := result + 1;
                        inner_count := inner_count + 1;
                    end loop;
                    inner_count := 0;
                    outer_count := outer_count + 1;
                end loop;
            end process;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);

    auto *proc = std::get_if<ast::Process>(arch->stmts.data());
    REQUIRE(proc != nullptr);
    REQUIRE_FALSE(proc->body.empty());

    auto *outer_loop = std::get_if<ast::WhileLoop>(proc->body.data());
    REQUIRE(outer_loop != nullptr);
    REQUIRE_FALSE(outer_loop->body.empty());

    auto *inner_loop = std::get_if<ast::WhileLoop>(outer_loop->body.data());
    REQUIRE(inner_loop != nullptr);
}

TEST_CASE("WhileLoop: While loop with if statement inside", "[statements][while_loop]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                while index < max_index loop
                    if data(index) = target then
                        found := true;
                        position := index;
                    end if;
                    index := index + 1;
                end loop;
            end process;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);

    auto *proc = std::get_if<ast::Process>(arch->stmts.data());
    REQUIRE(proc != nullptr);
    REQUIRE_FALSE(proc->body.empty());

    auto *while_loop = std::get_if<ast::WhileLoop>(proc->body.data());
    REQUIRE(while_loop != nullptr);
    REQUIRE_FALSE(while_loop->body.empty());
}

TEST_CASE("WhileLoop: While loop with inequality", "[statements][while_loop]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                while value /= target_value loop
                    value := compute_next(value);
                end loop;
            end process;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    auto *arch = std::get_if<ast::Architecture>(&design.units[1]);
    REQUIRE(arch != nullptr);

    auto *proc = std::get_if<ast::Process>(arch->stmts.data());
    REQUIRE(proc != nullptr);
    REQUIRE_FALSE(proc->body.empty());

    auto *while_loop = std::get_if<ast::WhileLoop>(proc->body.data());
    REQUIRE(while_loop != nullptr);
}

// Labeled while loop (not yet verified if supported)
TEST_CASE("WhileLoop: Labeled while loop", "[statements][while_loop]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                search_loop: while not found loop
                    check_next_item;
                end loop search_loop;
            end process;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Verify labeled loop support when implemented
}

// Exit statement in while loop (not yet verified if Exit node exists)
TEST_CASE("WhileLoop: While loop with exit statement", "[statements][while_loop]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                while true loop
                    if exit_condition then
                        exit;
                    end if;
                    do_work;
                end loop;
            end process;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Verify exit statement when implemented
}

// Next statement in while loop (not yet verified if Next node exists)
TEST_CASE("WhileLoop: While loop with next statement", "[statements][while_loop]")
{
    constexpr std::string_view VHDL_FILE = R"(
        entity Test is end Test;
        architecture RTL of Test is
        begin
            process
            begin
                while counter < max_count loop
                    counter := counter + 1;
                    if skip_condition then
                        next;
                    end if;
                    process_item;
                end loop;
            end process;
        end RTL;
    )";

    auto design = builder::buildFromString(VHDL_FILE);
    // TODO(someone): Verify next statement when implemented
}
