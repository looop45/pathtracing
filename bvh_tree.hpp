#ifndef BVH_TREE_H
#define BVH_TREE_H

#include "hittable.hpp"
#include "hittable_list.hpp"
#include "bounding_volume.hpp"
#include "shadingModel.hpp"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

//intermediate nodes are BVs
//leaf nodes are objects
struct node
{
    bounding_volume box;
    bool isLeaf = false;
    int leftRight = 0;
    hittable_list children;
    int m;
    int n;
    shared_ptr<node> left = nullptr;
    shared_ptr<node> right = nullptr;

    node () {}
    node(int m, bounding_volume box, hittable_list children, int n)
    {

        this->children = children;
        children.calculate_extents();
        this->m = m;
        this->n = n;
        this->box = box;
        if (m < 1 || children.count() <= n)
        {
            isLeaf = true;
            return;
        }

        //find largest dimension
        int largest_dim = get_largest_dimension(box);

        //split bounding_volume
        bounding_volume left_box(box.get_lower_extent(), split_volume(box, largest_dim, 1));
        bounding_volume right_box(split_volume(box, largest_dim, 0), box.get_upper_extent());

        //split children
        hittable_list left_children = get_subspace_children(children, left_box);
        hittable_list right_children = get_subspace_children(children, right_box);

        //set in node
        if (left_children.count() != 0)
        {
            this->left = make_shared<node>(m-1, left_box, left_children, n);
        }
        if (right_children.count() != 0)
        {
            this->right = make_shared<node>(m-1, right_box, right_children, n);
            this->right->leftRight = 1;
        }
    }

    int get_largest_dimension(bounding_volume box)
    {
        point3 t0 = this->box.get_lower_extent();
        point3 t1 = this->box.get_upper_extent();

        double x_dim = abs(t1.x() - t0.x());
        double y_dim = abs(t1.y() - t0.y());
        double z_dim = abs(t1.z() - t0.z());

        double max_dim = x_dim;
        if (y_dim > max_dim)
        {
            max_dim = y_dim;
        }
        if (z_dim > max_dim)
        {
            max_dim  = z_dim;
        }

        if (max_dim == x_dim)
        {
            return 0;
        }
        if (max_dim == y_dim)
        {
            return 1;
        }
        else
        {
            return 2;
        }
    }

    point3 split_volume(bounding_volume box, int dim, int rightleft)
    {
        if (dim == 0)
        {
            double x_split = (box.get_upper_extent().x() - box.get_lower_extent().x())/2;
            if (rightleft == 0)
            {
                return box.get_lower_extent() + point3(x_split, 0, 0);
            }
            else
            {
                return box.get_upper_extent() - point3(x_split, 0, 0);
            }
        }

        if (dim == 1)
        {
            double y_split = (box.get_upper_extent().y() - box.get_lower_extent().y())/2;
            if (rightleft == 0)
            {
                return box.get_lower_extent() + point3(0, y_split, 0);
            }
            else
            {
                return box.get_upper_extent() - point3(0, y_split, 0);
            }
        }
        else
        {
            double z_split = (box.get_upper_extent().z() - box.get_lower_extent().z())/2;
            if (rightleft == 0)
            {
                return box.get_lower_extent() + point3(0, 0, z_split);
            }
            else
            {
                return box.get_upper_extent() - point3(0, 0, z_split);
            }
        }
    }

    bool hit(const ray& cam_ray, double t_min, double t_max, hit_record& rec)
    {
        return this->box.hit(cam_ray, t_min, t_max, rec);
    }

    hittable_list get_subspace_children(hittable_list list, bounding_volume box)
    {
        hittable_list inside;
        for (auto object : list.objects)
        {
            point3 amin = object->get_lower_extent();
            point3 amax = object->get_upper_extent();
            point3 bmin = box.get_lower_extent();
            point3 bmax = box.get_upper_extent();
            if ((amin.x() <= bmax.x() && amax.x() >= bmin.x())
            && (amin.y() <= bmax.y() && amax.y() >= bmin.y())
            && (amin.z() <= bmax.z() && amax.z() >= bmin.z()))
            {
                inside.add(object);
            }
        }

        return inside;
    }
};

class bvh_tree : public hittable
{
    public:
        bvh_tree(int m, hittable_list objects, int n);
        bool hit(const ray& cam_ray, double t_min, double t_max, hit_record& rec) override;
        void calculate_extents() override {}

    private:
        shared_ptr<node> root;
        bool recursive_ray(shared_ptr<node> curr_node, const ray& cam_ray, double t_min, double t_max, hit_record& rec);
};

bvh_tree::bvh_tree(int m, hittable_list objects, int n)
{
    objects.calculate_extents();
    bounding_volume box = bounding_volume(objects.get_lower_extent(), objects.get_upper_extent());
    root = make_shared<node>(m, box, objects, n);

}

bool bvh_tree::hit(const ray& cam_ray, double t_min, double t_max, hit_record& rec)
{
    hit_record temp_rec;
    if (root->hit(cam_ray, t_min, t_max, temp_rec))
    {
        return recursive_ray(root, cam_ray, t_min, t_max, rec);
    }
    else
    {
        return false;
    }
}

bool bvh_tree::recursive_ray(shared_ptr<node> curr_node, const ray& cam_ray, double t_min, double t_max, hit_record& rec)
{
    hit_record temp_rec;
    //check if hit children in subspace
    if (curr_node->isLeaf)
    {
        if (curr_node->children.hit(cam_ray, t_min, t_max, temp_rec))
        {
            //shadingModel white_Sphere(0.8, 0.2, 0.25, color(1.0, 1.0, 1.0), color(0.1,0.1,0.1), color(1.0, 1.0, 1.0), 32, 0, 0, 0);
            //rec.normal = vec3(0,1,0);
            //color c(0.4,0.4,0.4);
            //curr_node->box.hit(cam_ray, t_min, t_max, rec, curr_node->leftRight * c);
            //rec.material = white_Sphere;
            rec = temp_rec;
            return true;
        }
    }
    //if not, check left and right subspaces
    hit_record left_rec;
    hit_record right_rec;

    bool leftHit = false;
    bool rightHit = false;

    if (curr_node->left != nullptr)
    {
        leftHit = curr_node->left->hit(cam_ray, t_min, t_max, left_rec);
    }

    if (curr_node->right != nullptr)
    {
        rightHit = curr_node->right->hit(cam_ray, t_min, t_max, right_rec);
    }

    //ray intersects both, choose the first one
    if (leftHit && rightHit)
    {
        if (left_rec.t < right_rec.t)
        {
            if (recursive_ray(curr_node->left, cam_ray, t_min, t_max, temp_rec))
            {
                rec = temp_rec;
                return true;
            }
            else
            {
                return recursive_ray(curr_node->right, cam_ray, t_min, t_max, rec);
            }
        }
        else
        {
            if (recursive_ray(curr_node->right, cam_ray, t_min, t_max, temp_rec))
            {
                rec = temp_rec;
                return true;
            }
            else
            {
                return recursive_ray(curr_node->left, cam_ray, t_min, t_max, rec);
            }
        }
    }
    else if (leftHit)
    {
        return recursive_ray(curr_node->left, cam_ray, t_min, t_max, rec);
    }
    else if (rightHit)
    {
        return recursive_ray(curr_node->right, cam_ray, t_min, t_max, rec);
    }
    else
    {
        return false;
    }
}




#endif