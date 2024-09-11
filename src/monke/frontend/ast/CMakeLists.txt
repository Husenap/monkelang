add_library(monke_ast STATIC "AstDumpTree.hpp" "AstPrettyPrint.hpp" "Expr.hpp"
                             "Stmt.hpp")

set_target_properties(monke_ast PROPERTIES LINKER_LANGUAGE CXX)
