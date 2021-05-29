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

// Rayleigh scattering
inline double betaR(const Vect M, const double wavelength)
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
    return beta * exp(-h / 8000.);
}

// Mie scattering
inline double betaM(const Vect M)
{
    const double h = M.norm() - EARTH_RADIUS;
    return 2e-5 * exp(-h / 1200.);
}

// Transmission coefficient from A to B
double transmission(const Vect A, const Vect B, const double wavelength)
{
    Vect v = B - A;
    const double norm = v.norm();

    double integrale = 0;

    const int N = 10;
    for (int i=0; i<N; i++) {
        const Vect M = A + ((i + .5) / N) * v;

        const double beta = betaR(M, wavelength) + 1.1*betaM(M);
        integrale += beta * (norm / N);
    }

    return exp(-integrale);
}

double SkyRenderer::intensity(const Ray &ray, const double wavelength)
{
    Sphere endAtmosphere(Vect(), EARTH_RADIUS + 4*8000);
    const double t_max = endAtmosphere.collisionDate(ray);

    double intensity = 0;

    const int N = 10;
    const double dt = t_max / N;
    for (int i=0; i<N; i++) {
        const Vect M = ray.pos() + (i + .5)*dt * ray.dir();
        const double cosTheta = ray.dir() * m_toSun;

        // Rayleigh
        const double phaseR = 3. / (16.*M_PI) * (1. + cosTheta*cosTheta);
        const double coefRayleigh = phaseR * betaR(M, wavelength);

        // Mie
        const double g = 0.76;
        const double phaseM = (3. / (8.*M_PI) * (1. - g*g) * (1. + cosTheta*cosTheta))
                            / ((2. + g*g) * pow(1. + g*g - 2.*g*cosTheta, 1.5));
        const double coefMie = phaseM * betaM(M);

        intensity += 6e8 / N
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
