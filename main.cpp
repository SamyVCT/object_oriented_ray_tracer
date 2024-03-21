#include "functions.h"

#include "camera.h"
#include "color.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"
#include "rect.h"
#include "box.h"

int main() {
    hittable_list world;

    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_left   = make_shared<dielectric>(1.5);
    auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2), 0.0);

    world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));

    world.add(make_shared<sphere>(point3( 0.0,    0.0, -1.0),   0.5, material_center));
    world.add(make_shared<sphere>(point3(11.0,    0.0, -1.0),   0.5, material_left));
    world.add(make_shared<sphere>(point3(1.0,    0.0, -1.0),  -0.4, material_left));
    world.add(make_shared<sphere>(point3( -1.0,    0.0, -1.0),   0.5, material_right));


    // world.add(make_shared<xy_rect>(-0.5, 0.5, -1, 1, -1, material_center));
    // world.add(make_shared<xz_rect>(-0.5, 0.5, -1, 1, 1, material_center));
    // world.add(make_shared<yz_rect>(-1, 1, -1, 1, -0.5, material_center));
    
    // world.add(make_shared<box>(point3(-0.5, 0, 0), point3(0, 0.5, 0.5), material_center));

    //NICE SCENE
    // auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    // world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    // auto material1 = make_shared<dielectric>(1.5);
    // world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    // auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    // world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    // auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    // world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));
    
    // world.add(make_shared<box>(point3(4, 0, 2), point3(4.5, 0.5, 2.5), make_shared<lambertian>(color(0.1, 0.2, 0.5))));

    // world.add(make_shared<sphere>(point3(2, 0.4, 1.7), 0.4, make_shared<metal>(color(0.8,0.2,0.2), 0.0)));

    // world.add(make_shared<box>(point3(0, 0, 2), point3(0.5, 0.5, 2.5), make_shared<metal>(color(0.8, 0.6, 0.2), 0.0)));

    // world.add(make_shared<sphere>(point3(4.8,0.2,1), 0.2, make_shared<lambertian>(color::random(0.5, 1))));

    // world.add(make_shared<sphere>(point3(4,0.2,1.4), 0.2,  make_shared<metal>(color::random(0.5, 1), random_double(0, 0.5))));
    
    
    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 500;
    cam.samples_per_pixel = 200;
    cam.max_depth         = 40;

    cam.vfov     = 50;
    cam.lookfrom = point3(2,2,1);
    cam.lookat   = point3(0,0,-1);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;
    cam.focus_dist    = 3.4;
    int nb_threads = 15;

    cam.render(world,nb_threads);
}