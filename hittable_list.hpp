#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.hpp"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class hittable_list : public hittable {
    public:
        hittable_list() {}
        hittable_list(shared_ptr<hittable> object) { add(object); }

        void clear() { objects.clear(); }
        void add(shared_ptr<hittable> object) { objects.push_back(object); }
        void add(hittable_list mesh) { objects.insert(objects.end(), mesh.objects.begin(), mesh.objects.end()); }
        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) override;

        point3 get_upper_extent();
        point3 get_lower_extent();

    public:
        std::vector<shared_ptr<hittable>> objects;

        int count()
        {
            return objects.size();
        }

        void calculate_extents() override { this->lower_extent = get_lower_extent(); this->upper_extent = get_upper_extent(); }
};

bool hittable_list::hit(const ray& r, double t_min, double t_max, hit_record& rec) {
    hit_record temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;

    int id_index = 0;

    for (const auto& object : objects) {
        if (object->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
            rec.id = id_index;
            id_index++;
        }
    }

    return hit_anything;
}

point3 hittable_list::get_upper_extent()
{
    point3 first_extent = objects.at(0)->get_upper_extent();
    double max_x = first_extent.x();
    double max_y = first_extent.y();
    double max_z = first_extent.z();

    for (auto object : objects)
    {
        point3 point = object->get_upper_extent();
        if (point.x() > max_x)
        {
            max_x = point.x();
        }
        if (point.y() > max_y)
        {
            max_y = point.y();
        }
        if (point.z() > max_z)
        {
            max_z = point.z();
        }
    }
    return point3(max_x, max_y, max_z);
}

point3 hittable_list::get_lower_extent()
{
    point3 first_extent = objects.at(0)->get_lower_extent();
    double min_x = first_extent.x();
    double min_y = first_extent.y();
    double min_z = first_extent.z();

    for (auto object : objects)
    {
        point3 point = object->get_lower_extent();
        
        if (point.x() < min_x)
        {
            min_x = point.x();
        }
        if (point.y() < min_y)
        {
            min_y = point.y();
        }
        if (point.z() < min_z)
        {
            min_z = point.z();
        }
    }
    return point3(min_x, min_y, min_z);
}

#endif