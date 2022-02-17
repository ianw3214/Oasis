#pragma once

#include <string>

namespace Oasis
{
    class Resource
    {
    public:
        Resource() {}
        virtual ~Resource() {}

        virtual void Load(const std::string& path) {}
    };
}