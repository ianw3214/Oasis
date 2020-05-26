#pragma once

#include <vector>

struct ImGuiInputTextCallbackData;
namespace Oasis
{
    class Console
    {
    public:
        static void Init();
        static void ToggleShow();

        static void AddLog(const char* fmt, ...);
    private:
        static void Draw();
        static void ClearLog();
        static void ExecCommand(const char* command);

        static bool s_show;
        static char m_inputBuf[256];
        static std::vector<char *> m_items;
    public:
        static int TextEditCallback(ImGuiInputTextCallbackData* data);
    };
}