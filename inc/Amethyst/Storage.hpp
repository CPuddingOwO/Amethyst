#pragma once

#include <filesystem>
#include <fstream>
#include <memory>

#include <Logging/log.hpp>

namespace ame::storage {
    using path = std::filesystem::path;

    class Storage {
        static std::shared_ptr<logging::Logger> logger;
        public:
        [[nodiscard]] static bool exists(const path& filename) {
            return std::filesystem::exists(filename);
        }
        static bool write(const path& file_path, const std::string& content, const bool append = false) {
            if (!exists(file_path.parent_path())) {
                std::filesystem::create_directories(file_path.parent_path());
            }

            try {
                std::ofstream file;
                if (append) {
                    file.open(file_path, std::ios::app);
                } else {
                    file.open(file_path, std::ios::out);
                }

                if (!file.is_open()) {
                    logger->error("文件打开失败 -> {}", file_path.string()); // 这里
                    return false;
                }

                file << content;
                return true;
            } catch (const std::exception& e) {
                logger->error("文件写入失败 {} -> {}", file_path.string(), e.what());
                return false;
            }
        }
        [[nodiscard]] static std::optional<std::string> read(const path& file_path) {
            if (!exists(file_path)) {
                logger->error("文件不存在 -> {}", file_path.string());
                return std::nullopt;
            }

            try {
                std::ifstream file(file_path);
                if (!file.is_open()) {
                    logger->error("文件打开失败 -> {}", file_path.string());
                    return std::nullopt;
                }
                return std::string((std::istreambuf_iterator<char>(file)),
                                  std::istreambuf_iterator<char>());
            } catch (const std::exception& e) {
                logger->error("文件读取错误 {} -> {}", file_path.string(), e.what());
                return std::nullopt;
            }
        }
    };

    // auto Storage::logger = nullptr;
}
