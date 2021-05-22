#include "SkyRenderer.hpp"
#include "renderer/Scene.hpp"

SkyRenderer::SkyRenderer(Scene *scene) :
    Renderer(scene)
{}

SkyRenderer::~SkyRenderer()
{}

Color SkyRenderer::color(const Ray &ray, const int remainingDepth)
{
    (void)remainingDepth;

    if (m_scene->collisionDate(ray) != INFINITY) {
        return Color(0x567d46);
    }
    return Color();
}
