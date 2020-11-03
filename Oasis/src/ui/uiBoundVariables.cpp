#include "uiBoundVariables.hpp"

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
    GetVariable(var_name).m_int = val;
}

void UIBoundVariables::SetVariableUint(const std::string& var_name, unsigned int val)
{
    GetVariable(var_name).m_uint = val;
}

void UIBoundVariables::SetVariableStr(const std::string& var_name, const std::string& val)
{
    std::strcpy(GetVariable(var_name).m_str, val.c_str());
}

int UIBoundVariables::GetVariableInt(const std::string& var_name)
{
    return GetVariable(var_name).m_int;
}

unsigned int UIBoundVariables::GetVariableUint(const std::string& var_name)
{
    return GetVariable(var_name).m_uint;
}

const std::string& UIBoundVariables::GetVariableStr(const std::string& var_name)
{
    return std::string(GetVariable(var_name).m_str);
}