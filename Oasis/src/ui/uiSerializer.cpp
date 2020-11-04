#include "uiSerializer.hpp"

#include <fstream>
#include <sstream>

// TODO: Needs more error checking in general
// TODO: White space trimming

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
            curr->m_show = true;

            // Parse the comma deliminated representation of a UI element
            std::stringstream sstream(line);
            unsigned int counter = 0;
            while(sstream.good())
            {
                std::string token;
                std::getline(sstream, token, ',');
                // Parse the current token
                switch(counter)
                {
                    // name of UI Element
                    case 0: {
                        addedElements[token] = curr;
                    } break;
                    // width
                    case 1: {
                        curr->m_width = std::stoi(token);
                    } break;
                    // height
                    case 2: {
                        curr->m_height = std::stoi(token);
                    } break;
                    // anchor (JUST USE NUMBERS TO REPRESENT)
                    case 3: {
                        curr->m_anchor = static_cast<UIAnchor>(std::stoi(token));
                    } break;
                    // x offset
                    case 4: {
                        curr->m_xOffset = std::stoi(token);
                    } break;
                    // y offset
                    case 5: {
                        curr->m_yOffset = std::stoi(token);
                    } break;
                    // parent
                    case 6: {
                        if (token.size() > 0)
                        {
                            auto it = addedElements.find(token);
                            if (it != addedElements.end())
                            {
                                it->second->m_children.push_back(curr);
                            }
                            else
                            {
                                // TODO: Error logging
                                // attach to root for now
                                root->m_children.push_back(curr);
                            }
                        }
                        else
                        {
                            root->m_children.push_back(curr);
                        }
                    } break;
                    // Type
                    case 7: {
                        curr->m_UIType = static_cast<UIType>(std::stoi(token));
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
                        if (curr->m_UIType == UIType::BACKGROUND)
                        {
                            DeserializeUIBackgroundLine(token, type_index, curr);
                        }
                        if (curr->m_UIType == UIType::TEXT)
                        {
                            DeserializeUITextLine(token, type_index, curr);
                        }
                        if (curr->m_UIType == UIType::TEXTURE)
                        {
                            DeserializeUITextureLine(token, type_index, curr);
                        }
                        if (curr->m_UIType == UIType::TEXT_DYNAMIC)
                        {
                            DeserializeUIDynamicTextLine(token, type_index, curr);
                        }
                    } break;
                };
                counter++;
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
            curr->m_borderWidth = std::stoi(line);
        } break;
        case 1: {
            DeserializeColour(line, curr->m_background);
        } break;
        case 2: {
            DeserializeColour(line, curr->m_border);
        } break;
        default: {
            // Error logging, should have ended here
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
            curr->m_text = text;
        } break;
        case 1: {
            DeserializeColour(line, curr->m_colour);
        } break;
        case 2: {
            curr->m_font = static_cast<UIFont>(std::stoi(line));
        } break;
        default: {
            // TODO: Error logging, should have ended here
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
            curr->m_path = text;
        } break;
        default: {
            // TODO: Error logging, should have ended here
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
            curr->m_formatString = text;
        } break;
        case 1: {
            DeserializeColour(line, curr->m_colour);
        } break;
        case 2: {
            curr->m_font = static_cast<UIFont>(std::stoi(line));
        } break;
        default: {
            // TODO: Error logging, should have ended here
        } break;
    }
}

void UISerializer::DeserializeColour(const std::string& line, Oasis::Colour& colour)
{
    // RGB separated by '|' characters
    // TODO: Want to check sstream.good() ideally
    std::stringstream sstream(line);
    std::string token;
    std::getline(sstream, token, '|');
    colour.r = std::stof(token);
    std::getline(sstream, token, '|');
    colour.g = std::stof(token);
    std::getline(sstream, token, '|');
    colour.b = std::stof(token);
}