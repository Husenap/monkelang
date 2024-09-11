find_package(BISON 3.8.2 REQUIRED)
find_package(FLEX 2.6.4 REQUIRED)

bison_target(monke_grammar "Grammar.yy"
             "${CMAKE_CURRENT_BINARY_DIR}/Parser.cpp")
flex_target(monke_lexer "Lex.l" "${CMAKE_CURRENT_BINARY_DIR}/Lexer.cpp")
add_flex_bison_dependency(monke_lexer monke_grammar)

add_library(monke_parser ${BISON_monke_grammar_OUTPUTS}
                         ${FLEX_monke_lexer_OUTPUTS})

target_link_libraries(monke_parser PUBLIC monke_ast)

target_include_directories(monke_parser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(monke_parser PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
