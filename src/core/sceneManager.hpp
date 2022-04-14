#pragma once

#include <string>

namespace Oasis
{
    struct SceneObject;
    class Sprite;
    class Scene;
    class SceneManager
    {
    public:
        static void Init(const std::string& file);

        static Scene* CurrentScene() { return mCurrentScene; }
        static void ChangeScene(Scene* newScene);

        static SceneObject* InsertSceneObject(Oasis::Sprite* sprite, int layer = 0);

    private:
        static Scene* mCurrentScene;
    };
}