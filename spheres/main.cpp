#include <QApplication>
#include <QMainWindow>
#include <QImage>
#include <QLabel>

#include <iostream>

#include "renderer/Stats.hpp"
#include "renderer/Scene.hpp"
#include "renderer/objects/Sphere.hpp"
#include "image_utility.hpp"

Image* generateImage ()
{
    Scene *scene = new Scene;

    scene->addObject(new Sphere(Vect(-1, 0, 1), 1, Material(Color(255, 0, 0))));
    scene->addObject(new Sphere(Vect(-1, -2.1, 1), 1, Material(Color(0, 255, 0)))); // Left
    scene->addObject(new Sphere(Vect(-1, 2.1, 1), 1, Material(Color(0, 0, 255), 0.6))); // Right

    scene->addLight(new Light(Vect(3, 2, 2)));

    scene->camera()->setPos( Vect (10, 0, 1) );
    scene->camera()->setDir( Vect (-3, 0, 0) );

    Image *image = scene->camera()->image();
    delete scene;
    return image;
}

int main (int argc, char *argv[])
{
    QApplication app(argc, argv);

    Stats::init();

    Image *image = generateImage();

    Stats::endTimer();
    Stats::print(std::cout);

    showImage(*image);

    return app.exec();
}
