add_library(monke_interpreter STATIC "Interpreter.hpp" "Literal.hpp"
                                     "Environment.hpp")

target_link_libraries(monke_interpreter PUBLIC monke_parser)
