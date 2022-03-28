#include "renderSystem.hpp"

#include "graphics/renderer.hpp"
#include "graphics/sprite.hpp"
#include "entity/entity.hpp"
#include "entity/components/core/renderComponent.hpp"

void RenderSystem::Update(std::vector<Oasis::Entity*>& entities) {
    for (Oasis::Entity* entity : entities) {
        RenderComponent* render = entity->getComponent<RenderComponent>();
        render->getSprite()->SetPos(render->getX(), render->getY());
        Oasis::Renderer::DrawSprite(render->getSprite());
    }
}