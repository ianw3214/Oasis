#include "uiBoundVariables.hpp"

#include "util/trap.hpp"

UIBoundVariables::UIBoundVariables()
{
    // Initialize things here
}

UIBoundVariables::~UIBoundVariables()
{
    // Free things here
}

void UIBoundVariables::SetVariableInt(const std::string& var_name, int val)
{
    GetVariable(var_name).m_type = BoundVariableType::INT;
    GetVariable(var_name).m_int = val;
}

void UIBoundVariables::SetVariableUint(const std::string& var_name, unsigned int val)
{
    GetVariable(var_name).m_type = BoundVariableType::UINT;
    GetVariable(var_name).m_uint = val;
}

void UIBoundVariables::SetVariableStr(const std::string& var_name, const std::string& val)
{
    GetVariable(var_name).m_type = BoundVariableType::STR;
    char * text = new char[256];
    strcpy_s(text, 256, val.c_str());
    GetVariable(var_name).m_str = text;
}

int UIBoundVariables::GetVariableInt(const std::string& var_name)
{
    return GetVariable(var_name).m_int;
}

unsigned int UIBoundVariables::GetVariableUint(const std::string& var_name)
{
    return GetVariable(var_name).m_uint;
}

const char * UIBoundVariables::GetVariableStr(const std::string& var_name)
{
    return GetVariable(var_name).m_str;
}

std::string UIBoundVariables::GetVariableAsString(const std::string& var_name)
{
    BoundVariable& var = GetVariable(var_name);
    switch(var.m_type)
    {
        case BoundVariableType::INT: {
            return std::to_string(var.m_int);
        } break;
        case BoundVariableType::UINT: {
            return std::to_string(var.m_uint);
        } break;
        case BoundVariableType::STR: {
            return std::string(var.m_str);
        } break;
        default: {
            OASIS_TRAP(false);
        } break;
    }
    return "";
}