#include <Amethyst/Amethyst.hpp>
#include <Amethyst/Prelude/json.hpp>
#include <httplib/httplib.h>
#include <Logging/log.hpp>

#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>
#include <SDL3/SDL.h>

// #include <Amethyst/Network/BiliBili-API/Client.hpp>
#include <Amethyst/Module/BiliHelper/Ui/BiliLive.hpp>

namespace ame {
    void Amethyst::Initialize() {

    }

    void Amethyst::Terminate() {

    }

    void Amethyst::Process() {
        const auto logger = std::make_shared<logging::Logger>("Window");
        logger->info("Window => Start...");
        if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
            logger->error("SDL_Init(): {}", SDL_GetError());
        }

        float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
        SDL_WindowFlags window_flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN | SDL_WINDOW_HIGH_PIXEL_DENSITY;
        SDL_Window* window = SDL_CreateWindow("Amethyst", static_cast<int>(1280 * main_scale), static_cast<int>(720 * main_scale), window_flags);
        if (window == nullptr)
        {
            logger->error("Error: SDL_CreateWindow(): {}", SDL_GetError());
            return;
        }
        SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
        SDL_SetRenderVSync(renderer, 1);
        if (renderer == nullptr)
        {
            logger->error("Error: SDL_CreateRenderer(): {}", SDL_GetError());
            return;
        }
        SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
        SDL_ShowWindow(window);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        // Setup Dear ImGui style
        // ImGui::StyleColorsDark();
        ImGui::StyleColorsLight();


        // Setup scaling
        ImGuiStyle& style = ImGui::GetStyle();
        style.ScaleAllSizes(main_scale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
        style.FontScaleDpi = main_scale;        // Set initial font scale. (using io.ConfigDpiScaleFonts=true makes this unnecessary. We leave both here for documentation purpose)
        style.WindowRounding = 12.0f;
        style.ChildRounding = 0.0f;
        style.FrameRounding = 5.0f;
        style.ScrollbarRounding = 12.0f;
        style.GrabRounding = 5.0f;

        // Setup Platform/Renderer backends
        ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
        ImGui_ImplSDLRenderer3_Init(renderer);

        io.Fonts->AddFontFromFileTTF("./AlimamaShuHeiTi-Bold.ttf", 20.0f, nullptr, io.Fonts->GetGlyphRangesChineseFull());
        // ImFont* font = io.Fonts->AddFontFromFileTTF("./AlimamaShuHeiTi-Blod");

        auto clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        bool isRunning = true;
        const auto bl = std::make_unique<module::bilihelper::BiliLive>();
        while (isRunning) {
            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                ImGui_ImplSDL3_ProcessEvent(&event);
                if (event.type == SDL_EVENT_QUIT)
                    isRunning = false;
                if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(window))
                    isRunning = false;
            }

            if (SDL_GetWindowFlags(window) & SDL_WINDOW_MINIMIZED)
            {
                SDL_Delay(10);
                continue;
            }

            ImGui_ImplSDLRenderer3_NewFrame();
            ImGui_ImplSDL3_NewFrame();
            ImGui::NewFrame();

            ImGui::ShowDemoWindow();
            ImGui::Begin("Hello");
            ImGui::Text("Hello, Amethyst!");
            ImGui::Text("你好，阿美斯特！");
            ImGui::End();

            bl->Show();

            {
                static float f = 0.0f;
                static int counter = 0;

                ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

                ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

                ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
                ImGui::ColorEdit3("clear color", reinterpret_cast<float *>(&clear_color)); // Edit 3 floats representing a color

                if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                    counter++;
                ImGui::SameLine();
                ImGui::Text("counter = %d", counter);

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
                ImGui::End();
            }

            // Rendering
            ImGui::Render();
            SDL_SetRenderScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
            SDL_SetRenderDrawColorFloat(renderer, clear_color.x, clear_color.y, clear_color.z, clear_color.w);
            SDL_RenderClear(renderer);
            ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
            SDL_RenderPresent(renderer);
        }

        ImGui_ImplSDLRenderer3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
}

