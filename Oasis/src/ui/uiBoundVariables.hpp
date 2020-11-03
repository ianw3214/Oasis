#pragma once

#include <string>
#include <unordered_map>

struct BoundVariable
{
    // Use a union to store all the different types there are
    union {
        int m_int;
        unsigned int m_uint;
        char* m_str;
    };
};

// The UI Manager will own an instance of this class, so NOT a static class
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
    const std::string& GetVariableStr(const std::string& var_name);
private:
    std::unordered_map<std::string, BoundVariable> m_variables;

    inline BoundVariable& GetVariable(const std::string& var_name)
    {
        auto it = m_variables.find(var_name);
        if (it == m_variables.end())
        {
            m_variables[var_name] = BoundVariable{};
            // Initialize to 0
            m_variables[var_name].m_int = 0;
        }
        return m_variables[var_name];
    }
};