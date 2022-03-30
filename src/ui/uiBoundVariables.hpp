#pragma once

#include <string>
#include <unordered_map>

enum class BoundVariableType
{
    INT = 0,
    UINT,
    STR,
    NONE
};

struct BoundVariable
{
    BoundVariableType mType;
    // Use a union to store all the different types there are
    union {
        int mInt;
        unsigned int mUint;
        char* mStr;
    };
};

// The UI Manager will own an instance of this class, so NOT a static class
// TODO: Maybe wanna rename to UIBoundVariableManager?
class UIBoundVariables
{
public:
    UIBoundVariables();
    ~UIBoundVariables();

    void SetVariableInt(const std::string& var_name, int val);
    void SetVariableUint(const std::string& var_name, unsigned int val);
    void SetVariableStr(const std::string& var_name, const std::string& val);

    int GetVariableInt(const std::string& var_name);
    unsigned int GetVariableUint(const std::string& var_name);
    const char * GetVariableStr(const std::string& var_name);

    std::string GetVariableAsString(const std::string& var_name);
private:
    std::unordered_map<std::string, BoundVariable> mVariables;

    // TODO: Only create by default when setting variable, not when getting
    inline BoundVariable& GetVariable(const std::string& var_name)
    {
        auto it = mVariables.find(var_name);
        if (it == mVariables.end())
        {
            mVariables[var_name] = BoundVariable{};
            // Initialize to 0
            mVariables[var_name].mType = BoundVariableType::INT;
            mVariables[var_name].mInt = 0;
        }
        return mVariables[var_name];
    }
};