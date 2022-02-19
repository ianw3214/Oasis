#pragma once

namespace Oasis
{
    class Scene;
    class SceneManager
    {
    public:
        static void Init(const std::string& file);

        static Scene* CurrentScene() { return mCurrentScene; }
        static void ChangeScene(Scene* newScene);

    private:
        static Scene* mCurrentScene;
    };
}