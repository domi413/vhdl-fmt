add_library(
    builder
    STATIC
    ast_builder.cpp
    translators/translator_constraint.cpp
    translators/translator_declaration.cpp
    translators/translator_expression.cpp
    translators/translator_statement.cpp
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
