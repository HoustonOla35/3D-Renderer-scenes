#include <QApplication>
#include <QMainWindow>
#include <QImage>
#include <QLabel>

#include <iostream>
#include <iomanip>

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
    const clock_t startTime = clock();

    Image *image = generateImage();

    const clock_t endTime = clock();

    // Show stats
    const int width1 = 40;
    const int width2 = 16;

    std::cout.imbue(std::locale("")); // Pretty print numbers
    std::cout << "##### Statistics\n";

    std::cout << std::left  << std::setw(width1) << "Total CPU time:"
              << std::right << std::setw(width2-2) << (double)(endTime-startTime) / CLOCKS_PER_SEC << " s\n";

    std::cout << std::left  << std::setw(width1) << "Ray-triangle tests count:"
              << std::right << std::setw(width2) << Stats::rayTriangleTestsCount() << "\n";
    std::cout << std::left  << std::setw(width1) << "Ray-triangle intersections count:"
              << std::right << std::setw(width2) << Stats::rayTriangleIntersectionsCount() << "\n";

    std::cout << std::left  << std::setw(width1) << "Ray-box tests count:"
              << std::right << std::setw(width2) << Stats::rayBoxTestsCount() << "\n";
    std::cout << std::left  << std::setw(width1) << "Ray-box intersections count:"
              << std::right << std::setw(width2) << Stats::rayBoxIntersectionsCount() << "\n";

    std::cout.flush();

    showImage(*image);

    return app.exec();
}
