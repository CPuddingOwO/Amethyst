#pragma once

namespace ame::ui {
    class SimpleUI {
    public:
        virtual ~SimpleUI() = default;
        virtual bool Show() = 0;

    private:
    };
}