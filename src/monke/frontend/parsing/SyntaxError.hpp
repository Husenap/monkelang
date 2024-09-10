#include <sstream>
#include <stdexcept>
#include <string_view>

#include "SourceLoc.h"

namespace monke {
class SyntaxError : public std::runtime_error {
public:
  SyntaxError(location loc, std::string_view reason) : runtime_error(makeMessage(loc, reason)) {}

private:
  static std::string makeMessage(location loc, std::string_view reason) {
    std::stringstream ss;
    ss << loc << ": Error: " << reason << std::endl;
    return ss.str();
  }
};
} // namespace monke
