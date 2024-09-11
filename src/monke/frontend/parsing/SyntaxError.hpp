#include <sstream>
#include <stdexcept>
#include <string_view>

#include "SourceLoc.h"

namespace monke {

class SyntaxError : public std::runtime_error {
  static std::string makeMessage(location loc, std::string_view reason) {
    std::stringstream ss;
    ss << loc << ": Error: " << reason << std::endl;
    return ss.str();
  }

public:
  SyntaxError(location loc, std::string_view reason) : runtime_error(makeMessage(loc, reason)) {}
};

} // namespace monke
