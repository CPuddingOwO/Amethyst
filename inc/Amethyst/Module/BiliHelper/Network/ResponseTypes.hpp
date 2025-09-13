#pragma once

#include <Amethyst/Prelude/json.hpp>
#include <string>
#include <vector>

namespace ame::module::bilihelper::resp {
    namespace live::section {
        struct Chilld {
            std::string id;
            std::string parent_id;
            std::string old_area_id;
            std::string name;
            std::string act_id;
            std::string pk_status;
            int hot_status{};
            std::string lock_status;
            std::string pic;
            std::string parent_name;
            int area_type{};
            std::optional<std::string> cate_id; // Optional field

            NLOHMANN_JSONIFY_ALL_THINGS(
                Chilld,
                id,
                parent_id,
                old_area_id,
                name,
                act_id,
                pk_status,
                hot_status,
                lock_status,
                pic,
                parent_name,
                area_type,
                cate_id
            )
        };

        struct Section {
            int id{};
            std::string name;
            std::vector<Chilld> list;
            NLOHMANN_JSONIFY_ALL_THINGS(Section, id, name, list)
        };

    }
    namespace live::info {

    }

    namespace live::create {
        struct Data {
            std::string roomID;
            NLOHMANN_JSONIFY_ALL_THINGS(Data, roomID)
        };
    }
    namespace general {
        template <typename T>
        struct Response {
            int code{};
            std::optional<std::string> ttl;
            std::optional<std::string> msg;
            std::string message;
            std::vector<T> data;
            NLOHMANN_JSONIFY_ALL_THINGS(Response, ttl, msg, message, data)
        };
    }
}