#pragma once

#include <Amethyst/Prelude/json.hpp>

namespace ame::module::bilihelper::req {
    namespace live::create {
        struct Request {
            std::string platform;
            std::string csrf; // == bili_jct
            NLOHMANN_JSONIFY_ALL_THINGS(Request, platform, csrf)
        };
    }
}


