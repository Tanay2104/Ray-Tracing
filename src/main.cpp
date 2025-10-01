#include "rtcommon.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"
#include "threadsafequeue.h"

void worker_function(ThreadSafeQueue& task_queue,  camera& cam, hittable& world, std::vector<color>& data);

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cout  << "Usage: ./ray_tracer (SAMPLES) (NUM_THREADS)" << std::endl;
        return 1;
    }
    int samples = std::stoi(argv[1]);
    int num_threads = std::stoi(argv[2]);
    if (num_threads==-1) num_threads = std::thread::hardware_concurrency();

    // We will first make the queue
    hittable_list world;

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-900,0), 900, ground_material));

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

    // Now we intitialse the camera

   
    camera cam;

    cam.aspect_ratio      = 30.0/20.0;
    cam.image_width       = 32*30;
    cam.samples_per_pixel = samples;
    cam.max_depth         = 50;

    cam.block_size_x = 32;
    cam.block_size_y = 32;

    cam.vfov     = 35;
    cam.lookfrom = point3(13,2,3);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0.6;
    cam.focus_dist    = 10.0;

    cam.initialize();

    std::vector<color> data(cam.image_width * cam.image_height);

    // Now we will construct the task queue.
    // It stores block starting coordinates as (x,y)
    ThreadSafeQueue task_queue;
    for (int j=0; j < int(cam.image_height/cam.block_size_y); j++) {
        for (int i=0; i< int(cam.image_width)/cam.block_size_x;i++) {
            task_queue.push(std::make_pair(i, j));
        }
    }
    
    auto start = std::chrono::steady_clock::now();
    // Creating the thread workers
    std::vector<std::thread> workers;
    for (int i = 0; i < num_threads; ++i) {
        workers.emplace_back(worker_function, std::ref(task_queue), std::ref(cam), std::ref(world), std::ref(data));
    }

    task_queue.shutdown();

    for (auto& thread : workers) {
        thread.join();
    }
    std::ofstream imageFile;
    imageFile.open("imagefile.ppm", std::ios::out);
    imageFile << "P3\n" << cam.image_width << ' ' << cam.image_height << "\n255\n";

    for (auto v: data) {
        write_color(imageFile, v);
    }
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
                /* << "\nMax depth: " << cam.max_depth \ */
                << "\nVertical fov: " << cam.vfov \
                /* << "\nLook from: " << cam.lookfrom \
                << "\nLook At: " << cam.lookat \
                << "\nCamera up direction: " << cam.vup \
                << "\nDefocus angle: " << cam.defocus_angle \
                << "\nFocus dist: " << cam.focus_dist \ */
                << "\nCPU Threads: " << num_threads \
                << "\nBlock size x " << cam.block_size_x \
                << "\nTIME TAKEN(SECONDS): " << duration_seconds.count();
    logFile << "\n-----------";
    logFile.close();
}

void worker_function(ThreadSafeQueue& task_queue, camera& cam, hittable& world, std::vector<color> & data) {
    while (true) {
        std::optional<std::pair<int, int>> task = task_queue.pop();
        if (task == std::nullopt) break;
        cam.render(world, data, task.value().first, task.value().second);
    }
}