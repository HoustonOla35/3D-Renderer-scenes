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


Image* generateImage ()
{
    Scene *scene = new Scene;
    Renderer *renderer = new SkyRenderer(scene);

    scene->addObject(new Sphere(Vect(), R_TERRE, Material(Color(0x567d46)))); // Terre
    scene->addLight(new Light(Vect(0, 0, 2*R_TERRE)));

    scene->camera()->setPos(Vect(0, 0, R_TERRE + 1));
    scene->camera()->setDir(Vect(1, 0, 0));

    scene->initRender();

    Image *image = scene->camera()->image(renderer);
    delete scene;
    return image;
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Stats::init();

    Image *image = generateImage();

    Stats::endTimer();
    Stats::print(std::cout);

    showImage(*image);

    return app.exec();
}
