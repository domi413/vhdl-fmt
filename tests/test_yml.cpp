#include "common/config.hpp"
#include "common/factory.hpp"
#include <catch2/catch_test_macros.hpp>
#include <string>

TEST_CASE("Load yml file and check integrity", "[integration][yml]")
{
    // --- Path ---
    const std::string yml_config{ std::string(TEST_DATA_DIR) };

    // --- Setup Config ---
    common::Config cfg = common::ConfigFactory::create(yml_config);

    // --- Verify Config ---
    REQUIRE(cfg.indentWidth() == 8);
    REQUIRE(cfg.lineEnding() == "lf");
    REQUIRE(cfg.useTabs() == true);
}
