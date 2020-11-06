#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

struct ImGuiInputTextCallbackData;
namespace Oasis
{
    class Console
    {
    public:
        static void Init();
        static void ToggleShow();
        static void SetCommand(const std::string& command, std::function<void()> func);

        static void Log(const char* fmt, ...);
        static void Error(const char* fmt, ...);
    private:
        static void Draw();
        static void ClearLog();
        static void ExecCommand(const char* command);

        static bool s_show;
        static char s_inputBuf[256];
        static std::vector<char *> s_items;
        static std::unordered_map<std::string, std::function<void()>> s_commands;
    public:
        static int TextEditCallback(ImGuiInputTextCallbackData* data);
    };
}