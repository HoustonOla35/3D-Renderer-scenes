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

double rayleighScattering(const Vect M, const double wavelength)
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

double mieScattering(const Vect M)
{
    const double h = M.norm() - EARTH_RADIUS;
    return 2.1e-6 * exp(-h / 1200);
}

// Transmission coefficient from A to B
double transmission(const Vect A, const Vect B, const double wavelength)
{
    Vect v = B - A;
    const double norm = v.norm();

    double integrale = 0;

    const int N = 10;
    for (int i=0; i<N; i++) {
        const double beta = rayleighScattering(A + ((double)i / N) * v, wavelength)
                          + 1.1 * mieScattering(A + ((double)i / N) * v);
        integrale += beta * (norm / N);
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

        const double coefRayleigh = 3./(16.*M_PI)*(1. + cosTheta*cosTheta) * rayleighScattering(M, wavelength);

        const double mu = cosTheta;
        const double g = 0.76;
        const double phaseM = 3.f / (8.f * M_PI) * ((1.f - g * g) * (1.f + mu * mu)) / ((2.f + g * g) * pow(1.f + g * g - 2.f * g * mu, 1.5f));
        const double coefMie = phaseM * mieScattering(M);

        intensity += 8. * 2e5 * SUN_INTENSITY/N // TODO : ajust constant
                   * transmission(M + endAtmosphere.collisionDate(Ray(M, m_toSun)) * m_toSun, M, wavelength)
                   * (coefRayleigh + coefMie)
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
