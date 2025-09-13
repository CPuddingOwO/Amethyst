#pragma once

namespace ame::module::bilihelper::api {
    static auto LiveHost = "https://api.live.bilibili.com";
    static auto GeneralHost = "https://api.bilibili.com";
    static auto GetSectionList = "/room/v1/Area/getList";
    static auto StartLive = "/room/v1/Room/startLive";
    static auto StopLive = "/room/v1/Room/stopLive";
    static auto GetRoomInfo = "/room/v1/Room/get_info";
    static auto GetUserRoomInfo = "/room/v1/Room/getRoomInfoOld";
    static auto GetLiverInfo = "/live_user/v1/Master/info";
    static auto CreateLiveRoom = "/xlive/app-blink/v1/preLive/CreateRoom";
    // static auto GetMyInfo =     "/x/space/myinfo";
    static auto GetMyInfo =     "/x/space/v2/myinfo";
    static auto Nav = "/x/web-interface/nav";
}

namespace ame::module::bilihelper::file {
    static std::filesystem::path SimpleSectionJson = "./data/sections.json";
    static std::filesystem::path UsersJson = "./data/users.json";
}

