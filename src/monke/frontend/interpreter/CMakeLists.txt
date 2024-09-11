add_library(
  monke_interpreter STATIC "Callable.hpp" "Environment.hpp" "Function.cpp"
                           "Function.hpp" "Interpreter.hpp" "Literal.hpp")

target_link_libraries(monke_interpreter PUBLIC monke_parser)
