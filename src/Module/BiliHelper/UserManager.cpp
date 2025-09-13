#include <Amethyst/Module/BiliHelper/UserManager.hpp>

namespace ame::module::bilihelper {
    std::shared_ptr<logging::Logger> SimpleUserManager::logger = nullptr;
    std::vector<SimpleUser> SimpleUserManager::users = {};
    std::vector<std::pair<int, std::string>> SimpleUserManager::us_name = {};
    SimpleUserManagerStatus SimpleUserManager::status = {};
    std::unique_ptr<network::Client> SimpleUserManager::client = std::make_unique<network::Client>();
}