#pragma once
#ifndef BVH_H
#define BVH_H

#include <vector>

#include "utils.h"
#include "Hittable.h"
#include "hittableList.h"


class BVHNode : public Hittable
{
public:
	BVHNode();

	BVHNode(const HittableList& list, double time0, double time1) 
		: BVHNode(list.objects, 0, list.objects.size(), time0, time1)
	{}

	BVHNode(const std::vector<shared_ptr<Hittable>>& src_objects,
		size_t start, size_t end, double time0, double time1)
	{
        auto objects = src_objects; // Create a modifiable array of the source scene objects

        comparator_axis = random_int(0, 2);

        size_t object_span = end - start;

        if (object_span == 1) 
        {
            left = right = objects[start];
        }
        else if (object_span == 2) 
        {
            if (compare(objects[start], objects[start + 1]))
            {
                left = objects[start];
                right = objects[start + 1];
            }
            else 
            {
                left = objects[start + 1];
                right = objects[start];
            }
        }
        else 
        {
            std::sort(objects.begin() + start, objects.begin() + end, 
                [this](const shared_ptr<Hittable> a, const shared_ptr<Hittable> b)
                {
                    return (comparator_axis == 0) ? box_x_compare(a, b)
                        : (comparator_axis == 1) ? box_y_compare(a, b)
                        : box_z_compare(a, b);
                });

            auto mid = start + object_span / 2;
            left = make_shared<BVHNode>(objects, start, mid, time0, time1);
            right = make_shared<BVHNode>(objects, mid, end, time0, time1);
        }

        AABB box_left, box_right;

        if (!left->bounding_box(time0, time1, box_left)
            || !right->bounding_box(time0, time1, box_right))
            std::cerr << "No bounding box in BVHNode constructor.\n";

        box = surrounding_box(box_left, box_right);
	}

    virtual bool hit(const Ray& r, double t_min, double t_max, HitResult& res) const override
	{
		if (!box.hit(r, t_min, t_max)) { return false; }

		bool hit_left = left->hit(r, t_min, t_max, res);
		bool hit_right = right->hit(r, t_min, t_max, res);

		return hit_left || hit_right;
	}

    bool compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b)
    {
        return (comparator_axis == 0) ? box_x_compare(a, b)
            : (comparator_axis == 1) ? box_y_compare(a, b)
            : box_z_compare(a, b);
    }

    inline bool box_compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b, int axis) 
    {
        AABB box_a;
        AABB box_b;

        if (!a->bounding_box(0, 0, box_a) || !b->bounding_box(0, 0, box_b))
            std::cerr << "No bounding box in BVHNode constructor.\n";

        return box_a.min()[axis] < box_b.min()[axis];
    }

    bool box_x_compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b) 
    {
        return box_compare(a, b, 0);
    }

    bool box_y_compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b) 
    {
        return box_compare(a, b, 1);
    }

    bool box_z_compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b) 
    {
        return box_compare(a, b, 2);
    }

    virtual bool bounding_box(double time0, double time1, AABB& output_box) const override
    {
        output_box = box;
        return true;
    }

public:
	shared_ptr<Hittable> left;
	shared_ptr<Hittable> right;
	AABB box;
    int comparator_axis;
};


#endif