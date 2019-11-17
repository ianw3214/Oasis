#pragma once

#include <functional>

namespace Oasis
{
    class Event;
    class IState;

    ////////////////////////////////////////////////////////
    struct Configuration
    {
        int m_width;
        int m_height;
        const char * m_name;

        std::function<IState*()> m_initState;
    };
    Configuration GetConfiguration();

    ////////////////////////////////////////////////////////
    class Application
    {
    public:
        Application(const Configuration& config);
        ~Application();

        inline int Width() const { return m_width; }
        inline int Height() const { return m_height; }

        void OnEvent(const Event& e);
        void Run();

    private:
        bool m_running;

        int m_width;
        int m_height;

        struct Impl;
        Impl * m_impl;
    };
}