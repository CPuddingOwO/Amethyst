#pragma once

#include <httplib/httplib.h>
#include <Amethyst/Prelude/json.hpp>
#include <Logging/log.hpp>

#include <Amethyst/Storage.hpp>

#include "Amethyst/Module/BiliHelper/SimpleTypes.hpp"


namespace ame::module::bilihelper::network {
    class Client {
        std::shared_ptr<logging::Logger> logger = nullptr;
        // web_link 已废弃
        std::array<const char*, 4> platforms = {"pc_link", "web_link", "android_link", "ios_link"};
        unsigned int focus_user_index = 0;
        nlohmann::json area_list;

    public:
        Client() {
            this->logger = std::make_shared<logging::Logger>("Bili");
        }
        ~Client() = default;

        bool SyncSectionCollection(bool force = false);
        bool SyncSingleUserInformation(SimpleUser& su);
    };
}
