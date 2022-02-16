#include "oasis.h"
#include "core/entry.hpp"

#include "sandbox.hpp"

Oasis::Configuration Oasis::GetConfiguration()
{
    Oasis::Configuration config;
    config.m_width = 1280;
    config.m_height = 720;
    config.m_name = "sandbox";
    config.m_startingScene = "test.yaml";
    return config;
}