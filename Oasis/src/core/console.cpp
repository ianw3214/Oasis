#include "console.hpp"
#include <imgui/imgui.h>

#include "imgui/imguiWrapper.hpp"

#include <ctype.h>

bool Oasis::Console::s_show = false;
char Oasis::Console::s_inputBuf[256];
std::vector<char *> Oasis::Console::s_items;
std::unordered_map<std::string, std::function<void()>> Oasis::Console::s_commands;

static char* Strdup(const char* s)
{ 
    size_t len = strlen(s) + 1; 
    void* buf = malloc(len); 
    IM_ASSERT(buf); 
    return (char*)memcpy(buf, (const void*)s, len); 
}

void Oasis::Console::Init()
{
    Oasis::ImGuiWrapper::AddWindowFunction(&Draw);

    // Default console commands
    Oasis::Console::SetCommand("clear", [](){
        ClearLog();
    });
}

void Oasis::Console::ToggleShow()
{
    s_show = !s_show;
}

void Oasis::Console::SetCommand(const std::string& command, std::function<void()> func)
{
    s_commands[command] = func;
}

void Oasis::Console::AddLog(const char* fmt, ...)
{
    // FIXME-OPT
    char buf[1024];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
    buf[IM_ARRAYSIZE(buf)-1] = 0;
    va_end(args);
    s_items.push_back(Strdup(buf));
}

void Oasis::Console::Draw()
{
    if (!s_show) return;
    ImGui::Begin("console");

    // Reserve enough left-over height for 1 separator + 1 input text
    const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
    ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar);
    if (ImGui::BeginPopupContextWindow())
    {
        if (ImGui::Selectable("Clear")) ClearLog();
        ImGui::EndPopup();
    }

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4,1)); // Tighten spacing
    for (unsigned int i = 0; i < s_items.size(); i++)
    {
        const char* item = s_items[i];
        
        // Normally you would store more information in your item than just a string.
        // (e.g. make Items[] an array of structure, store color/type etc.)
        ImVec4 color;
        bool has_color = false;
        if (strstr(item, "[error]"))          { color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f); has_color = true; }
        else if (strncmp(item, "# ", 2) == 0) { color = ImVec4(1.0f, 0.8f, 0.6f, 1.0f); has_color = true; }
        if (has_color)
            ImGui::PushStyleColor(ImGuiCol_Text, color);
        ImGui::TextUnformatted(item);
        if (has_color)
            ImGui::PopStyleColor();
    }
    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
    {
        ImGui::SetScrollHereY(1.0f);
    }

    ImGui::PopStyleVar();
    ImGui::EndChild();
    ImGui::Separator();

    bool reclaim_focus = false;
    ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory;
    if (ImGui::InputText("Input", s_inputBuf, IM_ARRAYSIZE(s_inputBuf), input_text_flags, &TextEditCallback))
    {
        char* s = s_inputBuf;
        // Strtrim(s);
        if (s[0])
            ExecCommand(s);
        strcpy_s(s, 256, "");
        reclaim_focus = true;
    }

    // Auto-focus on window apparition
    ImGui::SetItemDefaultFocus();
    if (reclaim_focus)
        ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget

    ImGui::End();
}

void Oasis::Console::ClearLog()
{
    for (unsigned int i = 0; i < s_items.size(); i++)
        free(s_items[i]);
    s_items.clear();
}

void Oasis::Console::ExecCommand(const char* command)
{
    AddLog("# %s\n", command);

    auto it = s_commands.find(command);
    if (it != s_commands.end())
    {
        (it->second)();
    }
    else
    {
        AddLog("Unknown command: '%s'\n", command);
    }
}

int Oasis::Console::TextEditCallback(ImGuiInputTextCallbackData* data)
{
    //AddLog("cursor: %d, selection: %d-%d", data->CursorPos, data->SelectionStart, data->SelectionEnd);
    switch (data->EventFlag)
    {
    case ImGuiInputTextFlags_CallbackCompletion:
        {
            // Example of TEXT COMPLETION

            // Locate beginning of current word
            const char* word_end = data->Buf + data->CursorPos;
            const char* word_start = word_end;
            while (word_start > data->Buf)
            {
                const char c = word_start[-1];
                if (c == ' ' || c == '\t' || c == ',' || c == ';')
                    break;
                word_start--;
            }

            // Build a list of candidates
            ImVector<const char*> candidates;
            // for (int i = 0; i < Commands.Size; i++)
            //     if (Strnicmp(Commands[i], word_start, (int)(word_end-word_start)) == 0)
            //         candidates.push_back(Commands[i]);

            if (candidates.Size == 0)
            {
                // No match
                AddLog("No match for \"%.*s\"!\n", (int)(word_end-word_start), word_start);
            }
            else if (candidates.Size == 1)
            {
                // Single match. Delete the beginning of the word and replace it entirely so we've got nice casing.
                data->DeleteChars((int)(word_start-data->Buf), (int)(word_end-word_start));
                data->InsertChars(data->CursorPos, candidates[0]);
                data->InsertChars(data->CursorPos, " ");
            }
            else
            {
                // Multiple matches. Complete as much as we can..
                // So inputing "C"+Tab will complete to "CL" then display "CLEAR" and "CLASSIFY" as matches.
                int match_len = (int)(word_end - word_start);
                for (;;)
                {
                    int c = 0;
                    bool all_candidates_matches = true;
                    for (int i = 0; i < candidates.Size && all_candidates_matches; i++)
                        if (i == 0)
                            c = toupper(candidates[i][match_len]);
                        else if (c == 0 || c != toupper(candidates[i][match_len]))
                            all_candidates_matches = false;
                    if (!all_candidates_matches)
                        break;
                    match_len++;
                }

                if (match_len > 0)
                {
                    data->DeleteChars((int)(word_start - data->Buf), (int)(word_end-word_start));
                    data->InsertChars(data->CursorPos, candidates[0], candidates[0] + match_len);
                }

                // List matches
                AddLog("Possible matches:\n");
                for (int i = 0; i < candidates.Size; i++)
                    AddLog("- %s\n", candidates[i]);
            }

            break;
        }
    // case ImGuiInputTextFlags_CallbackHistory:
    //     {
    //         // Example of HISTORY
    //         const int prev_history_pos = HistoryPos;
    //         if (data->EventKey == ImGuiKey_UpArrow)
    //         {
    //             if (HistoryPos == -1)
    //                 HistoryPos = History.Size - 1;
    //             else if (HistoryPos > 0)
    //                 HistoryPos--;
    //         }
    //         else if (data->EventKey == ImGuiKey_DownArrow)
    //         {
    //             if (HistoryPos != -1)
    //                 if (++HistoryPos >= History.Size)
    //                     HistoryPos = -1;
    //         }

    //         // A better implementation would preserve the data on the current input line along with cursor position.
    //         if (prev_history_pos != HistoryPos)
    //         {
    //             const char* history_str = (HistoryPos >= 0) ? History[HistoryPos] : "";
    //             data->DeleteChars(0, data->BufTextLen);
    //             data->InsertChars(0, history_str);
    //         }
    //     }
    }
    return 0;
}