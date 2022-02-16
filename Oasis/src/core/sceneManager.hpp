#pragma once

namespace Oasis
{
    class Scene;
    class SceneManager
    {
    public:
        static void Init(Scene* scene);

        static Scene* CurrentScene() { return mCurrentScene; }
        static void ChangeScene(Scene* newScene);

    private:
        static Scene* mCurrentScene;
    };
}