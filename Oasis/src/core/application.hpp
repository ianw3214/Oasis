#pragma once

namespace Oasis
{
    class IState;

    ////////////////////////////////////////////////////////
    struct Configuration
    {
        int m_width;
        int m_height;
        const char * m_name;

        IState * m_state;
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

        void Run();

    private:
        int m_width;
        int m_height;

        struct Impl;
        Impl * m_impl;
    };
}