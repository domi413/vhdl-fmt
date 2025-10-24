add_library(
    builder
    STATIC
    ast_builder.cpp
    translators/translator_decl.cpp
    translators/translator_expr.cpp
    translators/translator_unit.cpp
    trivia/trivia_binder.cpp
)

target_include_directories(builder PUBLIC ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(
    builder
    PUBLIC
        ast
        vhdl_generated
)
