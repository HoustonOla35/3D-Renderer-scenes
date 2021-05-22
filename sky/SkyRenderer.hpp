#ifndef SKYRENDERER_HPP
#define SKYRENDERER_HPP

#include "renderer/Renderer.hpp"

class SkyRenderer : public Renderer
{
public:
    SkyRenderer(Scene *scene);
    ~SkyRenderer();

    Color color(const Ray &ray, const int remainingDepth);
};

#endif // SKYRENDERER_HPP
