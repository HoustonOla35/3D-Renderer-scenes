#include "SkyRenderer.hpp"
#include "constants.hpp"

#include "renderer/Scene.hpp"
#include "renderer/objects/Sphere.hpp"

#include <iostream>
using namespace std;

SkyRenderer::SkyRenderer(Scene *scene, Vect toSun) :
    Renderer(scene),
    m_toSun(toSun)
{
    m_toSun.normalize();
}

SkyRenderer::~SkyRenderer()
{}

// Scattering coefficient at point M
double scattering(const Vect M, const double wavelength)
{
    double beta;
    if (wavelength == WAVELENGTH_BLUE) {
        beta = 3.31e-5;
    }
    else if (wavelength == WAVELENGTH_GREEN) {
        beta = 1.35e-5;
    }
    else if (wavelength == WAVELENGTH_RED) {
        beta = 5.8e-6;
    }
    else {
        assert(false);
    }

    const double h = M.norm() - EARTH_RADIUS;
    return beta * exp(-h / H_0);
}

// Transmission coefficient from A to B
double transmission(const Vect A, const Vect B, const double wavelength)
{
    Vect v = B - A;
    const double norm = v.norm();

    double integrale = 0;

    const int N = 10;
    for (int i=0; i<N; i++) {
        integrale += scattering(A + ((double)i / N) * v, wavelength) * (norm / N);
    }

    if (isnan(exp(-integrale))) {
        assert(false);
    }

    return exp(-integrale);
}

double SkyRenderer::intensity(const Ray &ray, const double wavelength)
{
    Sphere endAtmosphere(Vect(), EARTH_RADIUS + 5*H_0);
    const double t_max = endAtmosphere.collisionDate(ray);

    double intensity = 0;

    const int N = 10;
    for (int i=0; i<N; i++) {
        const Vect M = ray.pos() + (i*t_max/N) * ray.dir();

        assert(endAtmosphere.collisionDate(Ray(M, m_toSun)) != INFINITY);

        const double cosTheta = ray.dir() * m_toSun;

        intensity += 8 * 2e5 * SUN_INTENSITY/N // TODO : ajust constant
                   * transmission(M + endAtmosphere.collisionDate(Ray(M, m_toSun)) * m_toSun, M, wavelength)
                   * 3. / (16. * M_PI) * (1. + cosTheta*cosTheta)
                   * scattering(M, wavelength)
                   * transmission(M, ray.pos(), wavelength);
    }

    return intensity;
}

Color SkyRenderer::color(const Ray &ray, const int remainingDepth)
{
    if (m_scene->collisionDate(ray) != INFINITY) {
        return Renderer::color(ray, remainingDepth);
    }

    return Color(
        std::min((int)intensity(ray, WAVELENGTH_RED), 255),
        std::min((int)intensity(ray, WAVELENGTH_GREEN), 255),
        std::min((int)intensity(ray, WAVELENGTH_BLUE), 255)
    );
}
