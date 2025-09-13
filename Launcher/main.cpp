#include <iostream>
#include <Amethyst/Amethyst.hpp>

auto main(const int argc, char* argv[] ) -> int {
    for (int i = 0; i < argc; ++i) {
        std::cout << "Argument " << i << ": " << argv[i] << std::endl;
    }

    auto app = std::make_unique<ame::Amethyst>();
    app->Process();
    return 0;
}
