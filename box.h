#ifndef BOX_H
#define BOX_H

#include "functions.h"

#include "rect.h"
#include "hittable_list.h"

class box : public hittable  {
    public:
        box() {}
        box(const point3& p0, const point3& p1, shared_ptr<material> ptr);

        virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const override;


    private:
        point3 box_min;
        point3 box_max;
        hittable_list cotes;
};

box::box(const point3& p0, const point3& p1, shared_ptr<material> ptr) {
    box_min = p0;
    box_max = p1;

    cotes.add(make_shared<xy_rect>(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), ptr));
    cotes.add(make_shared<xy_rect>(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), ptr));

    cotes.add(make_shared<xz_rect>(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), ptr));
    cotes.add(make_shared<xz_rect>(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), ptr));

    cotes.add(make_shared<yz_rect>(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), ptr));
    cotes.add(make_shared<yz_rect>(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), ptr));
}

bool box::hit(const ray& r, interval ray_t, hit_record& rec) const {
    return cotes.hit(r, ray_t, rec);
}

#endif