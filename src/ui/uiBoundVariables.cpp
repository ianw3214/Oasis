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
    GetVariable(var_name).mType = BoundVariableType::INT;
    GetVariable(var_name).mInt = val;
}

void UIBoundVariables::SetVariableUint(const std::string& var_name, unsigned int val)
{
    GetVariable(var_name).mType = BoundVariableType::UINT;
    GetVariable(var_name).mUint = val;
}

void UIBoundVariables::SetVariableStr(const std::string& var_name, const std::string& val)
{
    GetVariable(var_name).mType = BoundVariableType::STR;
    char * text = new char[256];
    strcpy_s(text, 256, val.c_str());
    GetVariable(var_name).mStr = text;
}

int UIBoundVariables::GetVariableInt(const std::string& var_name)
{
    return GetVariable(var_name).mInt;
}

unsigned int UIBoundVariables::GetVariableUint(const std::string& var_name)
{
    return GetVariable(var_name).mUint;
}

const char * UIBoundVariables::GetVariableStr(const std::string& var_name)
{
    return GetVariable(var_name).mStr;
}

std::string UIBoundVariables::GetVariableAsString(const std::string& var_name)
{
    BoundVariable& var = GetVariable(var_name);
    switch(var.mType)
    {
        case BoundVariableType::INT: {
            return std::to_string(var.mInt);
        } break;
        case BoundVariableType::UINT: {
            return std::to_string(var.mUint);
        } break;
        case BoundVariableType::STR: {
            return std::string(var.mStr);
        } break;
        default: {
            OASIS_TRAP(false);
        } break;
    }
    return "";
}