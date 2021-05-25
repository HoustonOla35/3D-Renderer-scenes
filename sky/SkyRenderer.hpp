#ifndef SKYRENDERER_HPP
#define SKYRENDERER_HPP

#include "renderer/Renderer.hpp"

class SkyRenderer : public Renderer
{
public:
    SkyRenderer(Scene *scene, Vect toSun);
    ~SkyRenderer();

    Color color(const Ray &ray, const int remainingDepth);

private:
    double intensity(const Ray &ray, const double wavelength);

    Vect m_toSun;
};

#endif // SKYRENDERER_HPP
