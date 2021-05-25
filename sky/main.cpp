#include <QApplication>
#include <QMainWindow>
#include <QImage>
#include <QLabel>

#include <iostream>

#include "renderer/Stats.hpp"
#include "renderer/Scene.hpp"
#include "renderer/objects/Sphere.hpp"
#include "image_utility.hpp"

#include "constants.hpp"
#include "SkyRenderer.hpp"

using namespace std;


Image* generateImage (const double theta)
{
    const Vect posSun(sin(theta)*DIST_EARTH_SUN, 0, cos(theta)*DIST_EARTH_SUN);

    Scene *scene = new Scene;
    Renderer *renderer = new SkyRenderer(scene, posSun);

    // Earth
    scene->addObject(new Sphere(Vect(), EARTH_RADIUS, Material(Color(0x567d46))));
    //scene->addLight(new Light(Vect(0, 0, 2*EARTH_RADIUS)));

    // Sun
    //Vect posSun(DIST_EARTH_SUN, 0, 10*EARTH_RADIUS);
    //scene->addObject(new Sphere(posSun, SUN_RADIUS, Material(Color(255, 0, 0))));

    scene->camera()->setPos(Vect(0, 0, EARTH_RADIUS + 1));
    scene->camera()->setDir(Vect(1, 0, 0));
    scene->camera()->setWidth(3*scene->camera()->width());
    scene->camera()->setHeight(3*scene->camera()->height());

    scene->init();

    Image *image = scene->camera()->image(renderer);

    delete scene;
    return image;
}

int main(int argc, char *argv[])
{
#if 1
    QApplication app(argc, argv);
    Stats::init();

    Image *image = generateImage(0.5 * M_PI / 2);

    int r = 0;
    for (int x=0; x<image->width(); x++) {
        for (int y=0; y<image->height(); y++) {
            r = max(image->color(x, y).red(), r);
            r = max(image->color(x, y).green(), r);
            r = max(image->color(x, y).blue(), r);
        }
    }
    cerr << "Max : " << r << endl;

    Stats::endTimer();
    Stats::print(std::cout);

    showImage(*image);
    saveImage(*image, "output/day.png");

    return app.exec();
#else
    const int N = 20;
    for (int i=0; i<=N; i++) {
        const double theta = ((double)i/ N) * M_PI / 8 + 3*M_PI/8;
        Image *image = generateImage(theta);

        std::string filename = std::to_string(i);
        filename = "output/" + std::string(4 - filename.length(), '0') + filename + ".png";

        saveImage(*image, filename);
    }
#endif
}
