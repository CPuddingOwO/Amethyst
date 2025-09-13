#pragma once

#include <Amethyst/Prelude/json.hpp>

namespace ame::module::bilihelper {
    struct SimpleSectionChild {
        std::string id;
        std::string name;
        NLOHMANN_JSONIFY_ALL_THINGS(SimpleSectionChild, id, name)
    };
    struct SimpleSectionParent {
        std::string id;
        std::string name;
        std::vector<SimpleSectionChild> list;
        NLOHMANN_JSONIFY_ALL_THINGS(SimpleSectionParent, id, name, list)
    };

    struct SimpleRtmp {
        std::string addr;
        std::string token;
        NLOHMANN_JSONIFY_ALL_THINGS(SimpleRtmp, addr, token)
    };
    struct SimpleCookie {
        std::string SESSDATA;
        std::string bili_jct;
        NLOHMANN_JSONIFY_ALL_THINGS(SimpleCookie, SESSDATA, bili_jct)
    };

    struct SimpleUserSection {
        unsigned int id;
        unsigned int index;
        std::string name;
        NLOHMANN_JSONIFY_ALL_THINGS(SimpleUserSection, id, index, name)
    };

    struct SimpleUser {
        std::string uid;    // UID
        std::string rid;    // Room ID
        std::string srid;   // Short Room ID
        std::string name;   // Name
        std::string avatar_link; // 头像链接
        bool has_room;      // Created Live Room
        bool is_living;     // 正在直播
        bool is_rounding;   // 正在轮播
        std::string title;    // 直播标题
        SimpleUserSection section;
        SimpleCookie cookie;
        SimpleRtmp rtmp;
        NLOHMANN_JSONIFY_ALL_THINGS(SimpleUser, uid, rid, srid, name, avatar_link, has_room, is_living, is_rounding, title, section, cookie, rtmp)
    };
}