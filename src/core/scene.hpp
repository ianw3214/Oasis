#pragma once

#include "events/event.hpp"
#include "util/util.hpp"

#include <string>
#include <vector>

namespace Oasis
{
    class Sprite;
    struct SceneObject {
        Oasis::Sprite * mSprite;
        int mLayer;
        SceneObject(Oasis::Sprite* sprite, int layer) : mSprite(sprite), mLayer(layer) {}
    };

    class Entity;
    class Scene
    {
    public:
        void Init();
        void Close();

        void OnEvent(const Event& event);
        void Update();

        void AddEntity(Entity* entity);
        std::vector<Entity*>& getEntitiesMutable() { return mEntities; }
    
        // Rendering related
        SceneObject* InsertSceneObject(Oasis::Sprite* sprite, int layer);
    private:
        std::vector<Entity*> mEntities;
        std::vector<SceneObject*> mSceneObjects;

    public:
        bool loadFromYAMLtext(const std::string& text);
        bool loadFromFile(const std::string& fileName);
    };
}