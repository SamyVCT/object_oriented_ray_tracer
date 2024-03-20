#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "functions.h"

class material;

class hit_record {
  public:
    point3 p;
    vec3 normal;
    shared_ptr<material> mat;
    double t;
    bool front_face;

    void set_face_normal(const ray& r, const vec3& outward_normal) {
        // donne la normale sortante de la surface touchée par le rayon
        // outward_normal est un vecteur unitaire

        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable { // hittable est une classe pour chaque objet qui peut être touché par un rayon
  public:
    virtual ~hittable() = default;

    virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
};

#endif