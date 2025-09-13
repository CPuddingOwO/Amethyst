#pragma once
#include <memory>
#include <vector>

#include <Logging/log.hpp>

#include <Amethyst/Storage.hpp>
#include "Constants.hpp"
#include "SimpleTypes.hpp"
#include "Network/Client.hpp"

namespace ame::module::bilihelper {
    inline json paramsToJson(const httplib::Params& params) {
        json j;
        for (const auto& [key, value] : params) {
            // 如果 key 已经存在，将 value 转换为数组
            if (j.contains(key)) {
                if (j[key].is_array()) {
                    // 如果已经是数组，直接添加
                    j[key].push_back(value);
                } else {
                    // 如果不是数组，则将其转换为数组
                    j[key] = json::array({j[key], value});
                }
            } else {
                // 如果 key 不存在，直接赋值
                j[key] = value;
            }
        }
        return j;
    }

    struct SimpleUserManagerStatus {
        bool users_changed = false;
    };
    class SimpleUserManager {
        static std::shared_ptr<logging::Logger> logger;
        static std::vector<SimpleUser> users;
        static std::vector<std::pair<int, std::string>> us_name;
        static SimpleUserManagerStatus status;
        static std::unique_ptr<network::Client> client;
        public:
        explicit SimpleUserManager(const std::shared_ptr<logging::Logger> &logger_) {
            logger = logger_;
        }

        struct AddUserSpec {
            SimpleCookie cookie;
            NLOHMANN_JSONIFY_ALL_THINGS(AddUserSpec, cookie)
        };
        static bool AddUser(const AddUserSpec &spec, bool sync = true) {
            SimpleUser user{};
            user.cookie = spec.cookie;
            // TODO: 同步其他数据 例如 Name HasRoom Rid UID 之类的
            if (sync) {
                if (!client->SyncSingleUserInformation(user)) {
                    logger->error("SyncSingleUserInformation failed");
                    return true;
                }
            }

            users.emplace_back(user);
            logger->trace("Added User");
            status.users_changed = true;
            logger->debug("Status -> Users Changed");
            return false;
        }

        static std::vector<SimpleUser> GetUsers() {
            return users;
        }

        static std::vector<std::pair<int, std::string>> GetUsersNameWithIndexCached() {
            if (status.users_changed) {
                logger->debug("Username Cache Updating..");
                us_name = GetUsersNameWithIndex();
                status.users_changed = false;
            }
            return us_name;
        }

        static std::vector<std::pair<int, std::string>> GetUsersNameWithIndex() {
            std::vector<std::pair<int, std::string>> us;
            for (int i = 0; i < users.size(); i++) {
                us.emplace_back(i, users.at(i).name);
            }
            return us;
        }
    };
}
