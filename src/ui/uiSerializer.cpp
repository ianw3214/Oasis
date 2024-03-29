#include "uiSerializer.hpp"

#include <fstream>
#include <sstream>
#include <algorithm> 
#include <cctype>
#include <locale>

#include "core/console.hpp"

/////////////////////////////////////////////////////////////////////////////////////
// String trimming
// TODO: Centralize this in some sort of utility file I guess
/////////////////////////////////////////////////////////////////////////////////////
// trim from start (in place)
static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}
/////////////////////////////////////////////////////////////////////////////////////

// TODO: Needs more error checking in general

UISerializer::UISerializer()
{

}

UISerializer::~UISerializer()
{

}

UISerializer::Data UISerializer::Deserialize(const std::string& path, Ref<UIElement> root)
{
    // Keep track of currently added UI elements in case other UI elements are it's children
    Data::UINameMap addedElements;

    std::ifstream file(path);
    if (file.is_open())
    {
        std::string line;
        while(std::getline(file, line))
        {
            // Ignore comments
            if (line[0] == '#') continue;

            // The UI Element we are building
            UIElement * curr = new UIElement();
            curr->mShow = true;
            // If something went wrong and we didn't add it, remove at the end
            bool delete_at_end = false;

            // Parse the comma deliminated representation of a UI element
            std::stringstream sstream(line);
            unsigned int counter = 0;
            // If we want to delete the element at the end, there's no use to continue parsing
            while(sstream.good() && !delete_at_end)
            {
                std::string token;
                std::getline(sstream, token, ',');
                trim(token);
                // Parse the current token
                switch(counter)
                {
                    // name of UI Element
                    case 0: {
                        // Check that we don't already have the name in our added elements
                        if (addedElements.find(token) == addedElements.end())
                        {
                            addedElements[token] = curr;
                        }
                        else
                        {
                            Oasis::Console::Error("UI Element with name %s already defined, skipping current element...", token.c_str());
                            delete_at_end = true;
                        }
                    } break;
                    // width
                    case 1: {
                        curr->mWidth = std::stoi(token);
                    } break;
                    // height
                    case 2: {
                        curr->mHeight = std::stoi(token);
                    } break;
                    // anchor (JUST USE NUMBERS TO REPRESENT)
                    case 3: {
                        int i = std::stoi(token);
                        if (i < static_cast<int>(UIAnchor::COUNT))
                        {
                            curr->mAnchor = static_cast<UIAnchor>(i);
                        }
                        else
                        {
                            Oasis::Console::Error("Invalid UI Anchor: %i, setting to BOTTOM_LEFT", i);
                            curr->mAnchor = UIAnchor::BOTTOM_LEFT;
                        }
                    } break;
                    // x offset
                    case 4: {
                        curr->mXOffset = std::stoi(token);
                    } break;
                    // y offset
                    case 5: {
                        curr->mYOffset = std::stoi(token);
                    } break;
                    // parent
                    case 6: {
                        if (token.size() > 0)
                        {
                            auto it = addedElements.find(token);
                            if (it != addedElements.end())
                            {
                                it->second->mChildren.push_back(curr);
                            }
                            else
                            {
                                Oasis::Console::Error("Could not find UI element: '%s' - appending to root, make sure the parent is defined before the child!", token.c_str());
                                // attach to root for now
                                root->mChildren.push_back(curr);
                            }
                        }
                        else
                        {
                            root->mChildren.push_back(curr);
                        }
                    } break;
                    // Type
                    case 7: {
                        int i = std::stoi(token);
                        if (i < static_cast<int>(UIType::COUNT))
                        {
                            curr->mUIType = static_cast<UIType>(i);
                        }
                        else
                        {
                            Oasis::Console::Error("Invalid UI type: %i, setting to NONE", i);
                            curr->mUIType = UIType::NONE;
                        }
                    } break;
                    // Depending on the type, we will have to handle individually
                    // This is very bad code - will probably need a different solution in the future
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    {
                        int type_index = counter - 8;
                        if (curr->mUIType == UIType::BACKGROUND)
                        {
                            DeserializeUIBackgroundLine(token, type_index, curr);
                        }
                        if (curr->mUIType == UIType::TEXT)
                        {
                            DeserializeUITextLine(token, type_index, curr);
                        }
                        if (curr->mUIType == UIType::TEXTURE)
                        {
                            DeserializeUITextureLine(token, type_index, curr);
                        }
                        if (curr->mUIType == UIType::TEXT_DYNAMIC)
                        {
                            DeserializeUIDynamicTextLine(token, type_index, curr);
                        }
                        if (curr->mUIType == UIType::BUTTON)
                        {
                            DeserializeUIButtonLine(token, type_index, curr);
                        }
                        if (curr->mUIType == UIType::ANIMATED_TEXTURE)
                        {
                            DeserializeUIAnimatedTextureLine(token, type_index, curr);
                        }
                    } break;
                };
                counter++;
            }

            if (delete_at_end)
            {
                delete curr;
            }
        }
    }
    return UISerializer::Data{addedElements};
}

void UISerializer::DeserializeUIBackgroundLine(const std::string& line, int index, Ref<UIElement> curr)
{
    switch(index)
    {

        case 0: {
            curr->mBorderWidth = std::stoi(line);
        } break;
        case 1: {
            DeserializeColour(line, curr->mBackground);
        } break;
        case 2: {
            DeserializeColour(line, curr->mBorder);
        } break;
        default: {
            Oasis::Console::Error("Unexpected token while deserializing UI Background: %s", line.c_str());
        } break;
    }
}

void UISerializer::DeserializeUITextLine(const std::string& line, int index, Ref<UIElement> curr)
{
    switch(index)
    {
        case 0: {
            char * text = new char[256];
            strcpy_s(text, 256, line.c_str());
            curr->mText = text;
        } break;
        case 1: {
            DeserializeColour(line, curr->mColour);
        } break;
        case 2: {
            curr->mFont = static_cast<UIFont>(std::stoi(line));
        } break;
        default: {
            Oasis::Console::Error("Unexpected token while deserializing UI Text: %s", line.c_str());
        } break;
    }
}

void UISerializer::DeserializeUITextureLine(const std::string& line, int index, Ref<UIElement> curr)
{
    switch(index)
    {
        case 0: {
            char * text = new char[256];
            strcpy_s(text, 256, line.c_str());
            curr->mPath = text;
            curr->mCachedSprite = nullptr;
        } break;
        default: {
            Oasis::Console::Error("Unexpected token while deserializing UI Texture: %s", line.c_str());
        } break;
    }
}

void UISerializer::DeserializeUIDynamicTextLine(const std::string& line, int index, Ref<UIElement> curr)
{
    switch(index)
    {
        case 0: {
            char * text = new char[512];
            strcpy_s(text, 512, line.c_str());
            curr->mFormatString = text;
        } break;
        case 1: {
            DeserializeColour(line, curr->mColour);
        } break;
        case 2: {
            curr->mFont = static_cast<UIFont>(std::stoi(line));
        } break;
        default: {
            Oasis::Console::Error("Unexpected token while deserializing UI Dynamic Text: %s", line.c_str());
        } break;
    }
}

void UISerializer::DeserializeUIButtonLine(const std::string& line, int index, Ref<UIElement> curr)
{
    switch(index)
    {
        case 0: {
            char * text = new char[512];
            strcpy_s(text, 512, line.c_str());
            curr->mPath = text;
            curr->mCachedButtonSprite = nullptr;
        } break;
        case 1: {
            char * text = new char[512];
            strcpy_s(text, 512, line.c_str());
            curr->mHoverPath = text;
            curr->mCachedHoverSprite = nullptr;
        } break;
        case 2: {
            char * text = new char[512];
            strcpy_s(text, 512, line.c_str());
            curr->mClickEvent = text;
        } break;
        default: {
            Oasis::Console::Error("Unexpected token while deserializing UI Button: %s", line.c_str());
        } break;
    }
}

void UISerializer::DeserializeUIAnimatedTextureLine(const std::string& line, int index, Ref<UIElement> curr)
{
    switch(index)
    {
        case 0: {
            char * text = new char[512];
            strcpy_s(text, 512, line.c_str());
            curr->mPath = text;
            curr->mCachedAnimatedSprite = nullptr;
        } break;
        case 1: {
            curr->mFrameWidth = std::stoi(line);
        } break;
        case 2: {
            curr->mFrameHeight = std::stoi(line);
        } break;
        case 3: {
            curr->mAnimFrames = std::stoi(line);
        } break;
        case 4: {
            curr->mFps = std::stoi(line);
        } break;
        default: {
            Oasis::Console::Error("Unexpected token while deserializing UI Animated Texture: %s", line.c_str());
        } break;
    }
}

void UISerializer::DeserializeColour(const std::string& line, Oasis::Colour& colour)
{
    // RGB separated by '|' characters
    // TODO: Want to check sstream.good() ideally
    std::stringstream sstream(line);
    std::string token;
    trim(token);
    std::getline(sstream, token, '|');
    colour.r = std::stof(token);
    std::getline(sstream, token, '|');
    colour.g = std::stof(token);
    std::getline(sstream, token, '|');
    colour.b = std::stof(token);
}