
#include <Amethyst/Module/BiliHelper/Network/Client.hpp>

#include <Amethyst/Prelude/json.hpp>
#include <Amethyst/Storage.hpp>
#include <Amethyst/Module/BiliHelper/Constants.hpp>

#include <format>

#include "Amethyst/Module/BiliHelper/SimpleTypes.hpp"
#include "Amethyst/Module/BiliHelper/Network/ResponseTypes.hpp"
#include <Amethyst/Module/BiliHelper/Network/WbiCheck.hpp>

#include "Amethyst/Module/BiliHelper/UserManager.hpp"

using json = nlohmann::json;
using namespace ame::storage;

namespace ame::module::bilihelper::network {
    auto UserAgent = "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:143.0) Gecko/20100101 Firefox/143.0";

    bool Client::SyncSectionCollection(const bool force) {
        if (!Storage::exists(file::SimpleSectionJson) || force == true) {
            // 从 网络 同步最新分区列表
            httplib::Client cli(api::LiveHost);
            httplib::Headers headers;
            headers.emplace("UserAgent", UserAgent);
            auto res = cli.Get(api::GetSectionList, headers);
            if (res->status != 200) {
                logger->warn("GET {} {} -> {}", res->status, api::GetSectionList, res->body.c_str());
            }

            std::vector<SimpleSectionParent> simples = {};
            try {
                // 反序列化
                using namespace resp;
                const auto response = json::parse(res->body).get<general::Response<live::section::Section>>();
                for (const auto&[id, name, list]: response.data) {
                    SimpleSectionParent pa;
                    pa.id = std::to_string(id);
                    pa.name = name;
                    for (const auto& child: list) {
                        SimpleSectionChild ch;
                        ch.id = child.id;
                        ch.name = child.name;
                        pa.list.push_back(ch);
                    }
                    simples.push_back(pa);
                }
            } catch (const json::exception& e) {
                logger->error("{}", e.what());
            }
            json j = simples;
            this->area_list = j;

            // logger->trace("正在写入分区列表到 %s {}", file::SimpleSectionJson.c_str()); // TODO: 打印写入分区JSON日志失败
            // logger->info("{}", j.dump(1).c_str());
            return Storage::write(file::SimpleSectionJson, j.dump(1));
            // return false;
        }
        else { // 从本地同步分区列表
            logger->trace("正在从本地加载分区列表...");
            if (auto secs = Storage::read(file::SimpleSectionJson)) {
                this->area_list = json::parse(secs.value());
            }
        return false;
        }

    }

    /// 同步单用户数据，修改引用里的SimpleUser的数据
    /// 根据 Cookie 获取 UID, 用户昵称，直播间数据: 房间号，直播状态，分区数据
    bool Client::SyncSingleUserInformation(SimpleUser& su) {
        if (su.cookie.SESSDATA.empty()) {
            logger->error("User Cookie SESSDATA is Empty! User adding failed.");
            return false;
        }

        { // 初步通过 Cookie 获取 UID 和 用户昵称.
            httplib::Client cli(api::GeneralHost);
            httplib::Headers headers;
            headers.emplace("UserAgent", UserAgent);
            // headers.emplace("Cookie", std::format("SESSDATA={}; bili_jct={}", su.cookie.SESSDATA, su.cookie.bili_jct));
            headers.emplace("Cookie", std::format("SESSDATA={}", su.cookie.SESSDATA));
            logger->trace("Headers: ");
            for (const auto& [key, value] : headers) {
                std::cout << "Key: \t" << key << "\tValue: \t" << value << std::endl;
            }
            httplib::Params params {};
            // auto pa = paramsToJson(params);
            // auto w_rid = wbihelper->generateWRid(pa);
            // params.emplace("w_rid", w_rid);
            // logger->trace("w_rid: {}", w_rid);

            try {
                auto res = cli.Get(api::Nav, params, headers);
                if (res->status != 200) {
                    logger->warn("GET {} {} -> {}", res->status, api::Nav, res->body.c_str());
                }
                json rs = json::parse(res->body);

                logger->trace("Headers: ");
                for (const auto& [key, value] : res->headers) {
                    std::cout << "Key: \t" << key << "\tValue: \t" << value << std::endl;
                }
                logger->trace("\n{}\n", res->body);
                if (!rs.at("code") == 0) return false;

            }catch (const json::exception& e) {

            }

        }


        return true;
    }


    // bool Client::StartFocusUserLive() {
    //     httplib::Client cli(api::Host);
    //     httplib::Headers headers = {
    //         {"Cookie", std::format("SESSDATA={}; bili_jct={}", get_focus_user().cookie.SESSDATA, get_focus_user().cookie.bili_jct)},
    //     };
    //     httplib::Params params;
    //     params.emplace("room_id", get_focus_user().live.room_id);
    //     params.emplace("area_v2", std::to_string(get_focus_user().live.area_id));
    //     params.emplace("csrf", get_focus_user().cookie.bili_jct);
    //     params.emplace("platform", platforms.at(get_focus_user().live.platform_index));
    //     try {
    //         auto res = cli.Post(api::StartLive, headers, params);
    //         if (res && res->status == 200) {
    //             logger->info("开播成功 -> ");
    //             json result = json::parse(res->body);
    //             auto status = result["status"].get<int>();
    //             if (status != 0) {
    //                 logger->error("开播失败(message) -> {}", result["message"].get<std::string>());
    //                 logger->error("开播失败(msg) -> {}", result["msg"].get<std::string>());
    //             }
    //             /*
    //             if (status == 65530) {
    //                 error("开播失败 -> Token已失效");
    //             } // Token 错误
    //             if (status == 60009) {
    //                 error("开播失败 -> 分区不存在");
    //             } // 分区不存在
    //             if (status == 60013) {
    //                 error("开播失败 -> 非常抱歉，您所在的地区受实名认证限制无法开播");
    //             } // 非常抱歉，您所在的地区受实名认证限制无法开播
    //             if (status == 60024) {
    //                 error("开播失败 -> 目标分区需要人脸认证");
    //             } // 目标分区需要人脸认证
    //             if (status == 60037) {
    //                 error("开播失败 -> web 在线开播已下线");
    //             } // web 在线开播已下线 */
    //
    //         } else {
    //             logger->error("开播失败 -> {}; 状态: {}", res ? res->body : httplib::to_string(res.error()),res ? res->status : -1);
    //         }
    //     } catch (const std::exception &e) {
    //         logger->error("开播异常 -> {}", e.what());
    //     }
    //
    //     return false;
    // }

    // bool Client::StopFocusUserLive() {
    //     httplib::Client cli(api::Host);
    //     httplib::Headers headers = {
    //         {"Cookie", std::format("SESSDATA={}; bili_jct={}", get_focus_user().cookie.SESSDATA, get_focus_user().cookie.bili_jct)},
    //     };
    //     httplib::Params params;
    //     params.emplace("room_id", get_focus_user().live.room_id);
    //     params.emplace("platform", platforms.at(get_focus_user().live.platform_index));
    //     params.emplace("csrf", get_focus_user().cookie.bili_jct);
    //     try {
    //         auto res = cli.Post(api::StartLive, headers, params);
    //         if (res && res->status == 200) {
    //             logger->info("关播成功");
    //             json result = json::parse(res->body);
    //             if (!result["data"]["change"].get<bool>()) {
    //                 auto status = result["data"]["status"].get<std::string>();
    //                 if (status == "ROUND") get_focus_user().live.is_round = true;
    //                 if (status == "PREPARING") get_focus_user().live.is_live = false;
    //
    //                 logger->warn("关播失败，直播状态不变，当前状态 -> {}", status);
    //             }
    //
    //         } else {
    //             logger->error("关播失败 -> {}; 当前状态: {}", res ? res->body : httplib::to_string(res.error()),res ? res->status : -1);
    //         }
    //     } catch (const std::exception &e) {
    //         logger->error("关播异常 -> {}", e.what());
    //     }
    //
    //     return false;
    // }

}
