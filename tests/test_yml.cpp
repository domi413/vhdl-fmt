#include "common/config.hpp"
#include "common/factory.hpp"
#include <catch2/catch_test_macros.hpp>
#include <filesystem>

TEST_CASE("Load yml file and check integrity", "[integration][yml]")
{
    // --- Path ---
    const auto path = std::filesystem::path(TEST_DATA_DIR) / "yml_full";

    // --- Setup Config ---
    common::Config cfg = common::ConfigFactory::create(path);

    // --- Verify Config ---
    REQUIRE(cfg.indentWidth() == 8);
    REQUIRE(cfg.lineEnding() == "lf");
    REQUIRE(cfg.useTabs() == true);
}

TEST_CASE("Load partial yml file and check defaults", "[integration][yml]")
{
    // --- Path ---
    const auto path = std::filesystem::path(TEST_DATA_DIR) / "yml_partial";

    // --- Setup Config ---
    common::Config cfg = common::ConfigFactory::create(path);

    // --- Verify Config ---
    REQUIRE(cfg.indentWidth() == 8);
    REQUIRE(cfg.lineEnding() == "lf");
    REQUIRE(cfg.useTabs() == false);
}
