#pragma once
#include "imgui.h"
#include "../../../SimpleUI.hpp"
#include "Amethyst/Module/BiliHelper/UserManager.hpp"

namespace ame::module::bilihelper {
    class BiliLive final : public ui::SimpleUI{
        std::unique_ptr<SimpleUserManager> UM;
        int UserIndex = -1;
        std::vector<const char*> users = { };

        bool ShowToolPanelWindow = true;

        SimpleCookie cookie{};

        // 回调函数：动态调整 std::string 大小
        // static int ResizeCallback(ImGuiInputTextCallbackData* data) {
        //     if (data->EventFlag == ImGuiInputTextFlags_CallbackResize) {
        //         auto* str = static_cast<std::string*>(data->UserData);
        //         str->resize(data->BufTextLen); // 更新长度
        //         data->Buf = str->data(); // 更新缓冲区
        //         data->BufSize = str->capacity() + 1;
        //     }
        //     return 0;
        // }
        static int ResizeCallback(ImGuiInputTextCallbackData* data) {
            if (data->EventFlag == ImGuiInputTextFlags_CallbackResize) {
                auto* str = static_cast<std::string*>(data->UserData);
                str->resize(data->BufTextLen); // 更新字符串长度
                data->Buf = const_cast<char*>(str->c_str()); // 使用 c_str() 确保空终止
                data->BufSize = static_cast<int>(str->capacity()) + 1;
            }
            return 0;
        }
    public:
        BiliLive() {
            auto lg = std::make_shared<logging::Logger>("UserManager");
            UM = std::make_unique<SimpleUserManager>(lg);

            RefreshUserNames();
        }
        ~BiliLive() override = default;
        bool RefreshUserNames() {
            const auto us = UM->GetUsersNameWithIndexCached();
            this->users.clear();
            for (int i = 0; i < us.size(); i++) {
                this->users.at(us.at(i).first) = us.at(i).second.c_str();
            }
            return true;
        }

        bool Show() override {
            enum class Status {
                Offline = 0,    // RED
                Living = 1,     // Green
                Rounding = 2,   // Yellow
            };
            auto StatusToString = [](const Status ts) {
                switch (ts) {
                    case Status::Offline: return "Offline";
                    case Status::Living: return "Living";
                    case Status::Rounding: return "Rounding";
                }
                return "None";
            };

            auto ImColorRed = ImColor(249, 123, 176, 255);
            auto ImColorYellow = ImColor(176, 179, 119, 255);
            auto ImColorGreen = ImColor(120, 179, 119, 255);

            auto from_status = [&ImColorRed, &ImColorGreen, &ImColorYellow](const Status ts) {
                switch (ts) {
                    case Status::Offline: return ImColorRed;
                    case Status::Living: return ImColorGreen;
                    case Status::Rounding: return ImColorYellow;
                }
                return ImColorRed;
            };
            auto refresh  = [this]() {
                //TODO: 重新同步用户列表中当前用户的数据，如：测试Cookie是否有效，重新获取用户名，直播状态，直播间标题。
                RefreshUserNames();
            };
            auto start_live = []() {};
            auto stop_live = []() {};
            auto set_section = []() {};

            std::string StreamingAddress    = "https://example.com/very/long/stream/address/that/needs/truncation";
            std::string StreamingToken      = "TokenTokenTokenToken";
            std::string IdentityCode        = "XXXXXXXXXXXXX";
            auto UserStatus                 = Status::Living;
            std::string Section             = "CS:GO";

            // ImGui::SetNextWindowSizeConstraints(ImVec2(1090, 350), ImVec2(FLT_MAX, FLT_MAX));
            ImGui::Begin("AME#Tool", &this->ShowToolPanelWindow, ImGuiWindowFlags_AlwaysAutoResize);
            {
                // auto sz = ImGui::GetWindowSize();
                // auto io = ImGui::GetIO();
            }


            if (ImGui::BeginTable("AME#Bili-Main-Table", 3, ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersInner | ImGuiTableFlags_SizingStretchProp))
            {
                // Set column widths with ratio 1:3:1
                // float min_column_width = 10.0f; // Ensure MinColumnWidth is > 0 to avoid the assertion error
                ImGui::TableSetupColumn("Column 1", ImGuiTableColumnFlags_WidthStretch, 1.0f);
                ImGui::TableSetupColumn("Column 2", ImGuiTableColumnFlags_WidthStretch, 3.0f);
                ImGui::TableSetupColumn("Column 3", ImGuiTableColumnFlags_WidthStretch, 1.0f);

                auto TruncateText = [](const std::string& text, const float max_width, const ImVec4& color) {
                    // 文本 ID 用于检测点击和双击
                    const char* text_id = text.c_str();
                    const ImGuiID id = ImGui::GetID(text_id);
                    static ImGuiID last_clicked_id = 0;
                    static float last_clicked_time = 0;
                    const auto current_time = static_cast<float>(ImGui::GetTime());

                    // 计算文本是否需要截断
                    const char* text_display = text.c_str();
                    std::string truncated;
                    bool is_truncated = false;

                    if (ImGui::CalcTextSize(text_display).x > max_width) {
                        // 尝试截断文本以适应宽度
                        truncated = text;
                        while (!truncated.empty() && ImGui::CalcTextSize((truncated).c_str()).x > max_width) {
                            truncated.pop_back();
                        }
                        truncated += "...";
                        text_display = truncated.c_str();
                        is_truncated = true;
                    }

                    // 显示截断后的文本
                    ImGui::TextColored(color, "%s", text_display);

                    // 如果鼠标悬停在文本上，显示tooltip
                    if (is_truncated && ImGui::IsItemHovered()) {
                        ImGui::BeginTooltip();
                        ImGui::TextUnformatted(text.c_str());
                        ImGui::EndTooltip();
                    }

                    // 检测单击和双击
                    if (ImGui::IsItemClicked()) {
                        // 检测双击
                        if (last_clicked_id == id && (current_time - last_clicked_time) < 0.3f) {
                            // 双击选中所有文本
                            ImGui::SetClipboardText(text.c_str());
                            last_clicked_id = 0; // 重置点击状态
                            // TODO: 快速弹出一个通知，提示已复制到剪贴板
                        } else {
                            // 记录单击状态
                            last_clicked_id = id;
                            last_clicked_time = current_time;
                        }
                    }

                    return ImGui::IsItemHovered(); // 返回是否悬停，方便外部使用
                };

                float col0_width, col1_width, col2_width, col3_width;
                float text_width, text_height;
                float button_width, button_height;
                // Row 1
                ImGui::TableNextRow();

                // Center content for column 0
                ImGui::TableSetColumnIndex(0); {
                    col0_width = ImGui::GetContentRegionAvail().x;
                    text_width = ImGui::CalcTextSize("User").x;
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (col0_width - text_width) * 0.5f);
                    ImGui::AlignTextToFramePadding();
                    ImGui::Text("User");
                }

                // Column 1 with combo
                ImGui::TableSetColumnIndex(1); {
                    ImGui::AlignTextToFramePadding();
                    // std::vector<const char*> users = { };
                    ImGui::PushItemWidth(-FLT_MIN); // Make combo use full width
                    ImGui::Combo("##UserCombo", &UserIndex, this->users.data(), static_cast<int>(this->users.size()));
                    // ImGui::Combo("##UserCombo", &UserIndex, UM->GetUsersNameWithIndexCached(), static_cast<int>(UM->GetUsersNameWithIndexCached().size()));
                    ImGui::PopItemWidth();
                }

                // Center content for column 2
                ImGui::TableSetColumnIndex(2); {
                    float col2_width = ImGui::GetContentRegionAvail().x;
                    float buttons_width = 2 * ImGui::CalcTextSize("Add").x + 2 * ImGui::GetStyle().FramePadding.x * 2 +
                                          ImGui::GetStyle().ItemSpacing.x;
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (col2_width - buttons_width) * 0.5f);
                    ImGui::BeginGroup();
                    if (ImGui::Button("Add")) { ImGui::OpenPopup("BiliHelper#UserAddingPopup"); }
                    if (ImGui::BeginPopupModal("BiliHelper#UserAddingPopup", nullptr)) {
                        if (cookie.SESSDATA.capacity() < 32) {
                            cookie.SESSDATA.reserve(32); // 预分配空间
                        }
                        if (cookie.bili_jct.capacity() < 32) {
                            cookie.bili_jct.reserve(32);
                        }

                        // ImGui::InputText("SESSDATA", cookie.SESSDATA.data(), cookie.SESSDATA.size());
                        // ImGui::InputText("bili_jct", cookie.bili_jct.data(), cookie.bili_jct.size());

                        // SESSDATA 输入框
                        if (ImGui::InputText("SESSDATA", cookie.SESSDATA.data(), cookie.SESSDATA.capacity() + 1,
                                             ImGuiInputTextFlags_CallbackResize, ResizeCallback, &cookie.SESSDATA)) {
                            cookie.SESSDATA.resize(std::strlen(cookie.SESSDATA.data()));
                        }

                        // bili_jct 输入框
                        if (ImGui::InputText("bili_jct", cookie.bili_jct.data(), cookie.bili_jct.capacity() + 1,
                                             ImGuiInputTextFlags_CallbackResize, ResizeCallback, &cookie.bili_jct)) {
                            cookie.bili_jct.resize(std::strlen(cookie.bili_jct.data()));
                        }

                        // 调试信息
                        // ImGui::Text("SESSDATA: %s (size: %zu, capacity: %zu)",
                        // cookie.SESSDATA.c_str(), cookie.SESSDATA.size(), cookie.SESSDATA.capacity());
                        // ImGui::Text("bili_jct: %s (size: %zu, capacity: %zu)",
                        // cookie.bili_jct.c_str(), cookie.bili_jct.size(), cookie.bili_jct.capacity());

                        // 按钮
                        if (ImGui::Button("Submit")) {
                            this->UM->AddUser(
                                    {.cookie = SimpleCookie{.SESSDATA = cookie.SESSDATA, .bili_jct = cookie.bili_jct}});
                            // this->UM->AddUser({.cookie = SimpleCookie{.SESSDATA = cookie.SESSDATA}});
                            ImGui::CloseCurrentPopup();
                            cookie.SESSDATA.clear();
                            cookie.SESSDATA.shrink_to_fit();
                            cookie.bili_jct.clear();
                            cookie.bili_jct.shrink_to_fit();
                        }
                        ImGui::SameLine();
                        if (ImGui::Button("Cancel")) {
                            ImGui::CloseCurrentPopup();
                            cookie.SESSDATA.clear();
                            cookie.SESSDATA.shrink_to_fit();
                            cookie.bili_jct.clear();
                            cookie.bili_jct.shrink_to_fit();
                        }

                        // this->UM->AddUser({"92CjBbR0Xf37XLIAj1DFzS0J85rr14Ba5gkUt9cQnnW0oIdS7QlZ9A4uDlgyRIr01GMZMSVkJhc1J3TV9uamRzeWtLMmxkeUh4R256eEdNY2lINnFVdVhrYjVvbmN6ZHNfTzRnRFdpMHlzd0kxUThMOU9yX05VX2NSd29jYm85WFdjeTlXbGZxckJBIIEC"});
                        ImGui::EndPopup();
                    }


                    ImGui::SameLine();
                    if (ImGui::Button("Del")) {
                        // ImGui::OpenPopup("BiliHelper#UserDeletingPopup");
                    }
                    ImGui::EndGroup();
                }

                // Row 2
                ImGui::TableNextRow();

                ImGui::TableSetColumnIndex(0); {
                    text_width = ImGui::CalcTextSize("Title").x;
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (col0_width - text_width) * 0.5f);
                    ImGui::AlignTextToFramePadding();
                    ImGui::Text("Title");
                    // ImGui::TableSetColumnIndex(1);
                    // ImGui::AlignTextToFramePadding();
                    // ImGui::TextColored(RedTextColor, "....");
                }



                ImGui::TableSetColumnIndex(1); {
                    ImGui::AlignTextToFramePadding();
                    std::string title_text = "这是一个非常长的标题，用来测试文本截断功能";
                    float col1_width = ImGui::GetContentRegionAvail().x;
                    TruncateText(title_text, col1_width * 0.95f, ImColorRed);
                }

                ImGui::TableSetColumnIndex(2); {
                    button_width = ImGui::CalcTextSize("Refresh").x + ImGui::GetStyle().FramePadding.x * 2;
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (col2_width - button_width) * 0.5f);
                    ImGui::AlignTextToFramePadding();
                    if (ImGui::Button("Refresh")) {
                        refresh();
                    }
                }

                // Row 3
                ImGui::TableNextRow();

                ImGui::TableSetColumnIndex(0); {
                    text_width = ImGui::CalcTextSize("Stream Addr").x;
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (col0_width - text_width) * 0.5f);
                    ImGui::AlignTextToFramePadding();
                    ImGui::Text("Stream Addr");
                }

                ImGui::TableSetColumnIndex(1); {
                    ImGui::AlignTextToFramePadding();
                    TruncateText(StreamingAddress, col1_width * 0.95f, ImColorRed);
                }

                ImGui::TableSetColumnIndex(2); {
                    button_width = ImGui::CalcTextSize("Copy#Addr").x + ImGui::GetStyle().FramePadding.x * 2;
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (col2_width - button_width) * 0.5f);
                    ImGui::AlignTextToFramePadding();
                    if (ImGui::Button("Copy#Addr")) {
                        ImGui::SetClipboardText(StreamingAddress.c_str());
                    }
                }

                // Row 4
                ImGui::TableNextRow();

                ImGui::TableSetColumnIndex(0); {
                    text_width = ImGui::CalcTextSize("Stream Token").x;
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (col0_width - text_width) * 0.5f);
                    ImGui::AlignTextToFramePadding();
                    ImGui::Text("Stream Token");
                };

                ImGui::TableSetColumnIndex(1); {
                    ImGui::AlignTextToFramePadding();
                    TruncateText(StreamingToken, col1_width * 0.95f, ImColorRed);
                }


                ImGui::TableSetColumnIndex(2); {
                    button_width = ImGui::CalcTextSize("Copy#Token").x + ImGui::GetStyle().FramePadding.x * 2;
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (col2_width - button_width) * 0.5f);
                    ImGui::AlignTextToFramePadding();
                    if (ImGui::Button("Copy#Token")) {
                        ImGui::SetClipboardText(StreamingToken.c_str());
                    }
                }

                // Row 5
                ImGui::TableNextRow();

                ImGui::TableSetColumnIndex(0); {
                    button_width = ImGui::CalcTextSize("Set Section").x + ImGui::GetStyle().FramePadding.x * 2;
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (col0_width - button_width) * 0.5f);
                    ImGui::AlignTextToFramePadding();
                    if (ImGui::Button("Set Section")) {
                        set_section();
                    }
                }

                ImGui::TableSetColumnIndex(1); {
                    ImGui::AlignTextToFramePadding();
                    ImGui::BeginGroup();

                    // Calculate available width for the entire column content
                    float available_width = ImGui::GetContentRegionAvail().x;

                    // Calculate widths for each element
                    float section_label_width = ImGui::CalcTextSize("Section").x + ImGui::GetStyle().ItemSpacing.x;
                    float section_value_width = ImGui::CalcTextSize(Section.c_str()).x + ImGui::GetStyle().ItemSpacing.x;
                    float identity_label_width = ImGui::CalcTextSize("Identity Code").x + ImGui::GetStyle().ItemSpacing.x;
                    // Reserve remaining space for the identity value
                    float identity_value_width = available_width - section_label_width - section_value_width - identity_label_width;

                    ImGui::Text("Section");
                    ImGui::SameLine();
                    TruncateText(Section, section_value_width, ImColorRed);
                    ImGui::SameLine();

                    ImGui::Text("Identity Code");
                    ImGui::SameLine();

                    ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + identity_value_width);
                    TruncateText(IdentityCode, identity_value_width * 0.8f, ImColorRed);

                    ImGui::PopTextWrapPos();

                    ImGui::EndGroup();
                }

                ImGui::TableSetColumnIndex(2); {
                    button_width = ImGui::CalcTextSize("Copy#Code").x + ImGui::GetStyle().FramePadding.x * 2;
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (col2_width - button_width) * 0.5f);
                    ImGui::AlignTextToFramePadding();
                    if (ImGui::Button("Copy#Code")) {
                        ImGui::SetClipboardText(IdentityCode.c_str());
                    }
                }

                // Row 6 - Double height
                ImGui::TableNextRow(ImGuiTableRowFlags_None, ImGui::GetTextLineHeight() * 2.0f);

                ImGui::TableSetColumnIndex(0); {
                    std::string status_text = StatusToString(UserStatus); // Offline, Round, Live
                    float status_text_width = ImGui::CalcTextSize("Status").x;
                    float status_value_width = ImGui::CalcTextSize(status_text.c_str()).x;
                    float total_width = status_text_width + status_value_width + ImGui::GetStyle().ItemSpacing.x;
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (col0_width - total_width) * 0.5f);
                    // Vertical centering for double-height row
                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetTextLineHeight() * 0.5f);
                    ImGui::AlignTextToFramePadding();
                    ImGui::Text("Status"); ImGui::SameLine();
                    // ImGui::TextColored(RedTextColor, "...");
                    TruncateText(status_text, ImGui::GetContentRegionAvail().x, from_status(UserStatus));
                }

                ImGui::TableSetColumnIndex(1); {
                    // Center the buttons horizontally and vertically
                    float buttons_width2 = 2 * (ImGui::CalcTextSize("Start").x + ImGui::GetStyle().FramePadding.x * 2) + ImGui::GetStyle().ItemSpacing.x;
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (ImGui::GetContentRegionAvail().x - buttons_width2) * 0.5f);
                    // Vertical centering for double-height row
                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetTextLineHeight() * 0.5f);
                    ImGui::BeginGroup();
                    if (ImGui::Button("Start")) {
                        start_live();
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Stop")) {
                        stop_live();
                    }
                    ImGui::EndGroup();
                }

                ImGui::TableSetColumnIndex(2); {
                    float buttons_width2 = 2 * (ImGui::CalcTextSize("Start").x + ImGui::GetStyle().FramePadding.x * 2) + ImGui::GetStyle().ItemSpacing.x;
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (ImGui::GetContentRegionAvail().x - buttons_width2) * 0.5f);
                    // Vertical centering for double-height row
                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetTextLineHeight() * 0.5f);
                    if (ImGui::Button("Share")) {
                        ImGui::SetClipboardText("https://live.bilibili.com/");
                    }
                }

                ImGui::EndTable();
            }
            ImGui::End();
            return true;
        }
    };
}
