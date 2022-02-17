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
    BoundVariableType m_type;
    // Use a union to store all the different types there are
    union {
        int m_int;
        unsigned int m_uint;
        char* m_str;
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
    std::unordered_map<std::string, BoundVariable> m_variables;

    // TODO: Only create by default when setting variable, not when getting
    inline BoundVariable& GetVariable(const std::string& var_name)
    {
        auto it = m_variables.find(var_name);
        if (it == m_variables.end())
        {
            m_variables[var_name] = BoundVariable{};
            // Initialize to 0
            m_variables[var_name].m_type = BoundVariableType::INT;
            m_variables[var_name].m_int = 0;
        }
        return m_variables[var_name];
    }
};