#include "rtcommon.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"


int main(int argc, char* argv[]) {
    if (argc == 1) {
        std::cout  << "Usage: ./ray_tracer (SAMPLES)" << std::endl;
        return 1;
    }
    int samples = int(*argv[1]);
    hittable_list world;

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    for (int a = -5; a < 5; a++) {
        for (int b = -5; b < 5; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.5) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.85) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 1200;
    cam.samples_per_pixel = samples;
    cam.max_depth         = 50;

    cam.vfov     = 20;
    cam.lookfrom = point3(13,2,3);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0.6;
    cam.focus_dist    = 10.0;
    std::ofstream imageFile;
    auto start = std::chrono::steady_clock::now();
    cam.render(world, imageFile);
    auto end = std::chrono::steady_clock::now();
    auto duration_seconds = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    std::ofstream logFile;
    logFile.open("performance_logs.txt", std::ios::app);
     if (!logFile.is_open()) {
        std::cerr << "Error: Unable to open the file." << std::endl;
    }
    logFile << "\n-----------";
    logFile << "\nCamera aspect ratio: " << cam.aspect_ratio \
                << "\nImage width: " << cam.image_width \
                << "\nSamples per pixel: " << cam.samples_per_pixel \
                << "\nMax depth: " << cam.max_depth \
                << "\nVertical fov: " << cam.vfov \
                << "\nLook from: " << cam.lookfrom \
                << "\nLook At: " << cam.lookat \
                << "\nCamera up direction: " << cam.vup \
                << "\nDefocus angle: " << cam.defocus_angle \
                << "\nFocus dist: " << cam.focus_dist \
                << "\nTIME TAKEN(SECONDS): " << duration_seconds.count();
    logFile << "\n-----------";
    logFile.close();
}