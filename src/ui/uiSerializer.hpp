#pragma once

#include <string>
#include <unordered_map>

#include "uiCommon.hpp"

#include "util/pointer.hpp"

// The UI Manager will own an instance of this class, so NOT a static class
class UISerializer
{
public:
    UISerializer();
    ~UISerializer();

    // Return in a struct in case there is ever any more data we need to retrieve
    struct Data{
        typedef std::unordered_map<std::string, Ref<UIElement>> UINameMap;
        UINameMap mUIElements;
    };
    // All UI elements of a file must be part of the same group, so only need the root here
    Data Deserialize(const std::string& path, Ref<UIElement> root);
private:
    void DeserializeUIBackgroundLine(const std::string& line, int index, Ref<UIElement> curr);
    void DeserializeUITextLine(const std::string& line, int index, Ref<UIElement> curr);
    void DeserializeUITextureLine(const std::string& line, int index, Ref<UIElement> curr);
    void DeserializeUIDynamicTextLine(const std::string& line, int index, Ref<UIElement> curr);
    void DeserializeUIButtonLine(const std::string& line, int index, Ref<UIElement> curr);
    void DeserializeUIAnimatedTextureLine(const std::string& line, int index, Ref<UIElement> curr);

    void DeserializeColour(const std::string& line, Oasis::Colour& colour);
};