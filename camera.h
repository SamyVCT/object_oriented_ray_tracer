#ifndef CAMERA_H
#define CAMERA_H

#include "functions.h"
#include <vector>
#include "color.h"
#include "hittable.h"
#include <iostream>
#include "material.h"
#include <chrono>

#include<omp.h> // OpenMP pour le multi-threading

class camera {
  public:
    double aspect_ratio = 1.0;  // Ratio largeur/hauteur de l'image
    int    image_width  = 100;  // Largeur de l'image en pixels
    int    samples_per_pixel = 10;   // Nombre de samples par pixel
    int    max_depth         = 10;   // la fonction ray_color est récursive, et s'arrête seulement lorsqu'elle ne touche plus d'objets (ce qui peut prendre un moment), donc on définit un nombre de rebonds maximum

    double vfov = 90;  // Angle de vue vertical (Field Of View) en degrés (90° = grand angle, 20° = téléobjectif, 180° = fish-eye)
    point3 lookfrom = point3(0,0,-1);  // position de la caméra
    point3 lookat   = point3(0,0,0);   // position où la caméra regarde
    vec3   vup      = vec3(0,1,0);     // vecteur indiquant le haut de la caméra

    double defocus_angle = 0;  // on simule la lentille de la caméra en définissant un angle de flou pour définir l'ouverture du diaphragme
    double focus_dist = 10;    // Distance entre la position de la caméra et le plan du rendu où le focus est fait

    void render(const hittable& world,int nb_threads) {
        initialize();
        auto start = std::chrono::high_resolution_clock::now();

        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

        std::vector<color> buffer(image_width * image_height);

        color pixel_color(0,0,0);
        ray r;
        #pragma omp parallel for num_threads(nb_threads) shared(world,buffer) schedule(dynamic, 1)//private(pixel_color, r) 
        for (int j = 0; j < image_height; ++j) {
            std::clog << "\rLignes restantes : " << (image_height - j) << " \n " << std::flush;
            
            for (int i = 0; i < image_width; ++i) {
                color pixel_color(0,0,0);
                for (int sample = 0; sample < samples_per_pixel; ++sample) {
                    ray r = get_ray(i, j);
                    pixel_color += ray_color(r, max_depth, world);
                }
                buffer[j * image_width + i] = pixel_color;
                // write_color(std::cout, pixel_color, samples_per_pixel); ne marche pas car on ne peut pas écrire dans le même flux depuis plusieurs threads
            }
        }

        for (const auto& pixel_color : buffer) {
            write_color(std::cout, pixel_color, samples_per_pixel);
        }

        std::clog << "\rDone.                 \n";
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = end-start;
        std::clog << "Temps d'execution : " << diff.count() << " s\n";
    }

  private:
    int    image_height;   // Hauteur de l'image en pixels
    point3 center;         // Centre de la caméra
    point3 pixel00_loc;    // Localisation du pixel 0, 0
    vec3   pixel_delta_u;  // Ecart entre chaque pixel orienté vers la droite
    vec3   pixel_delta_v;  // Ecart entre chaque pixel orienté vers le bas
    vec3   u, v, w;        // Vecteur formant la base de la caméra
    vec3   defocus_disk_u;  // vecteur horizontal pour le rayon du disque de défocalisation
    vec3   defocus_disk_v;  // vecteur vertical pour le rayon du disque de défocalisation

    void initialize() {
        image_height = static_cast<int>(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        center = lookfrom;

        // Dimensions de la fenêtre de rendu
        auto theta = degrees_to_radians(vfov); // vfov est en degrés, on le convertit en radians
        auto h = tan(theta/2); 
        auto viewport_height = 2 * h * focus_dist;
        auto viewport_width = viewport_height * (static_cast<double>(image_width)/image_height);

        // Calcule les vecteur unitaires de la base pour les corrdonnées de la caméra
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        // Calcule les vecteurs horizontaux et verticaux le long de la fenêtre d'affichage
        vec3 viewport_u = viewport_width * u;    
        vec3 viewport_v = viewport_height * -v;  // On inverse le vecteur v pour que le pixel 0,0 soit en haut à gauche

        // Calcule les vecteurs delta entre chaque pixel
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // Calcule la localisation du pixel 0,0
        auto viewport_upper_left = center - (focus_dist * w) - viewport_u/2 - viewport_v/2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

        // Calcule les vecteurs de défocalisation, on spécifie l'angle du cone de sommet le centre de la fenêtre de rendu et de base le disque de défocalisation
        auto defocus_radius = focus_dist * tan(degrees_to_radians(defocus_angle / 2));
        defocus_disk_u = u * defocus_radius;
        defocus_disk_v = v * defocus_radius;
    }

    ray get_ray(int i, int j) const {
        // Retourne un rayon de caméra aléatoire pour le pixel à la position i,j, provenant du disque de défocalisation de la caméra
        
        auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
        auto pixel_sample = pixel_center + pixel_sample_square();

        auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
        auto ray_direction = pixel_sample - ray_origin;

        return ray(ray_origin, ray_direction);
    }

    vec3 pixel_sample_square() const {
        // Retourne un point aléatoire dans le carré unité autour de l'origine
        auto px = -0.5 + random_double();
        auto py = -0.5 + random_double();
        return (px * pixel_delta_u) + (py * pixel_delta_v);
    }

    point3 defocus_disk_sample() const {
        // Retourne un point aléatoire dans le disque de défocalisation de la caméra
        auto p = random_in_unit_disk();
        return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
    }

    color ray_color(const ray& r, int depth, const hittable& world) const {
        hit_record rec;

        // La fonction est récursive donc si on dépasse la limite de rebond, on renvoit un pixel noir
        if (depth <= 0)
            return color(0,0,0);

        if (world.hit(r, interval(0.001, infinity), rec)) { // on ignore les hits qui sont très près du point d'intersection (erreurs de float qui font que l'origine du prochain rayon peut être légèrement sur ou sous la surface)
            ray scattered;
            color attenuation;
            if (rec.mat->scatter(r, rec, attenuation, scattered))
                return attenuation * ray_color(scattered, depth-1, world);
            return color(0,0,0);
        }

        vec3 unit_direction = unit_vector(r.direction());
        auto a = 0.5*(unit_direction.y() + 1.0);
        return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0); // couleur du background (le ciel)
    }
};

#endif