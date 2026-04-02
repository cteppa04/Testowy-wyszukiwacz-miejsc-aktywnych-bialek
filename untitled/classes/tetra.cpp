#include "tetra.h"
#include <glm/glm.hpp>

using namespace glm;
void Tetra::calculate_circum_sphere()
{
    vec3 ba = (*bound_list)[b] - (*bound_list)[a];
    vec3 ca = (*bound_list)[c] - (*bound_list)[a];
    vec3 da = (*bound_list)[d] - (*bound_list)[a];

    float bal = dot(ba, ba);
    float cal = dot(ca, ca);
    float dal = dot(da, da);

    vec3 cross_ca_da = cross(ca, da);
    vec3 cross_da_ba = cross(da, ba);
    vec3 cross_ba_ca = cross(ba, ca);

    float denom = 2.0f * dot(ba, cross_ca_da);

    // If denom is zero, tetrahedron is degenerate
    if (fabs(denom) < 1e-8f) {
        circ_sphere_center = vec3(0.0f);
        circ_sphere_radius = 0.0f;
        return;
    }

    vec3 num = bal * cross_ca_da + cal * cross_da_ba + dal * cross_ba_ca;
    circ_sphere_center = (*bound_list)[a] + num / denom;
    circ_sphere_radius = length(circ_sphere_center - (*bound_list)[a]);
}

bool Tetra::point_insinde(glm::vec3 point)
{
    calculate_circum_sphere();
    vec3 diff = point - circ_sphere_center;
    float dist2 = dot(diff, diff);
    return dist2 <= circ_sphere_radius * circ_sphere_radius + 1e-6f;
}

QVector<Triangle_face> Tetra::get_faces()
{
    QList<Triangle_face> faces;
    faces.append(Triangle_face(a,b,c));
    faces.append(Triangle_face(b,d,c));
    faces.append(Triangle_face(d,a,c));
    faces.append(Triangle_face(a,b,d));
    return faces;
}
