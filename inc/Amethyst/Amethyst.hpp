#pragma once

namespace ame {

    class Amethyst {
        void Initialize();
        void Terminate();

        public:
        Amethyst() {
            Initialize();
        }
        ~Amethyst() {
            Terminate();
        }

        void Process();
    };
}
