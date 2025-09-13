#pragma once

#include <array>
#include <string>
#include <optional>
#include <chrono>
#include <httplib.h>
#include <sstream>
#include <iomanip>
#include <openssl/md5.h>

#include <Amethyst/Prelude/json.hpp>

namespace ame::module::bilihelper::network {

    // class WbiHelper {
    // private:
    //     constexpr static std::array<uint8_t, 64> MIXIN_KEY_ENC_TABLE = {
    //             46, 47, 18, 2, 53, 8, 23, 32, 15, 50, 10, 31, 58, 3, 45, 35,
    //             27, 43, 5, 49, 33, 9, 42, 19, 29, 28, 14, 39, 12, 38, 41, 13,
    //             37, 48, 7, 16, 24, 55, 40, 61, 26, 17, 0, 1, 60, 51, 30, 4,
    //             22, 25, 54, 21, 56, 59, 6, 63, 57, 62, 11, 36, 20, 34, 44, 52
    //     };
    //
    //     std::optional<std::pair<std::string, std::string> > cachedWbiKey;
    //     std::optional<std::string> cachedMixinKey;
    //
    //     /* 获取 MD5 哈希值 */
    //     std::string computeMd5(const std::string &input) const {
    //         unsigned char digest[MD5_DIGEST_LENGTH];
    //         MD5(reinterpret_cast<const unsigned char *>(input.c_str()), input.size(), digest);
    //
    //         std::ostringstream oss;
    //         for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
    //             oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(digest[i]);
    //         }
    //         return oss.str();
    //     }
    //
    //     /* 将 JSON 转换为 URL 编码字符串 */
    //     std::string jsonToUrlEncodedString(const nlohmann::json &json) const {
    //         std::string encodedString;
    //         for (const auto &[key, value] : json.items()) {
    //             encodedString.append(key).append("=")
    //                          .append(httplib::detail::encode_url(
    //                                  value.is_string() ? value.get<std::string>() : value.dump()))
    //                          .append("&");
    //         }
    //         encodedString.pop_back(); // 移除最后的 '&'
    //         return encodedString;
    //     }
    //
    //     /* 获取 WBI Key */
    //     std::pair<std::string, std::string> fetchWbiKey() {
    //         httplib::Client client("https://api.bilibili.com");
    //         httplib::Headers headers = {
    //                 {"User-Agent",
    //                  "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.3"},
    //                 {"Referer", "https://www.bilibili.com/"}
    //         };
    //         auto res = client.Get("/x/web-interface/nav", headers);
    //         if (!res || res->status != 200) {
    //             throw std::runtime_error("Failed to fetch WBI key");
    //         }
    //
    //         nlohmann::json json = nlohmann::json::parse(res->body);
    //         const std::string imgUrl = json["data"]["wbi_img"]["img_url"];
    //         const std::string subUrl = json["data"]["wbi_img"]["sub_url"];
    //
    //         std::string imgKey = imgUrl.substr(imgUrl.find("wbi/") + 4, imgUrl.find(".png") - imgUrl.find("wbi/") - 4);
    //         std::string subKey = subUrl.substr(subUrl.find("wbi/") + 4, subUrl.find(".png") - subUrl.find("wbi/") - 4);
    //
    //         return {imgKey, subKey};
    //     }
    //
    //     /* 获取 Mixin Key */
    //     std::string computeMixinKey(const std::string &imgKey, const std::string &subKey) {
    //         std::string rawKey = imgKey + subKey;
    //         std::string mixinKey;
    //         for (const auto &x: MIXIN_KEY_ENC_TABLE) {
    //             mixinKey.push_back(rawKey.at(x));
    //         }
    //         return mixinKey.substr(0, 32);
    //     }
    //
    // public:
    //     /* 对外：获取 w_rid */
    //     std::string generateWRid(nlohmann::json &params) {
    //         if (!cachedWbiKey.has_value()) {
    //             cachedWbiKey = fetchWbiKey();
    //         }
    //
    //         if (!cachedMixinKey.has_value()) {
    //             const auto &[imgKey, subKey] = cachedWbiKey.value();
    //             cachedMixinKey = computeMixinKey(imgKey, subKey);
    //         }
    //
    //         params["wts"] = std::chrono::duration_cast<std::chrono::seconds>(
    //                         std::chrono::system_clock::now().time_since_epoch())
    //                 .count();
    //
    //         const std::string encodedParams = jsonToUrlEncodedString(params) + cachedMixinKey.value();
    //         return computeMd5(encodedParams);
    //
    //     }
    // };
    // inline const static auto wbihelper = std::make_unique<WbiHelper>();
} // namespace ame::module::bilihelper::network
