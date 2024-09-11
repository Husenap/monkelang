#pragma once

#include <filesystem>
#include <fstream>
#include <vector>

inline auto GetTestFiles(const std::string_view directory, const std::string_view extension) {
  std::vector<std::filesystem::path> files;
  for (const auto& entry : std::filesystem::directory_iterator(directory)) {
    const auto path = entry.path();
    if (entry.is_regular_file() && path.extension() == extension) {
      files.push_back(path);
    }
  }
  return files;
}

inline std::string ReadFile(const std::filesystem::path& filepath) {
  std::ifstream file(filepath);
  if (!file.is_open()) {
    return "";
  }
  return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}
