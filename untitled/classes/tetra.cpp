#include "tetra.h"
#include "qdebug.h"
#include <glm/glm.hpp>

using namespace glm;
Tetra::Tetra(const uint a, const uint b, const uint c, const uint d, QVector<glm::vec3> *bound_list)
    : bound_list(bound_list){

    verticies.append(a);
    verticies.append(b);
    verticies.append(c);
    verticies.append(d);

    calculate_circum_sphere();
}

void Tetra::calculate_circum_sphere()
{
    vec3 ba = (*bound_list)[verticies[1]] - (*bound_list)[verticies[0]];
    vec3 ca = (*bound_list)[verticies[2]] - (*bound_list)[verticies[0]];
    vec3 da = (*bound_list)[verticies[3]] - (*bound_list)[verticies[0]];

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
    circ_sphere_center = (*bound_list)[verticies[0]] + num / denom;
    circ_sphere_radius = length(circ_sphere_center - (*bound_list)[verticies[0]]);
}

bool Tetra::point_insinde(glm::vec3 point)
{
    vec3 diff = point - circ_sphere_center;
    float dist2 = dot(diff, diff);
    return dist2 <= circ_sphere_radius * circ_sphere_radius + 1e-6f;
}

glm::vec3 Tetra::get_face_normal(Triangle_face &face)
{
    auto& list = *bound_list;
    glm::vec3* A = &list[face.id[0]];
    glm::vec3* B = &list[face.id[1]];
    glm::vec3* C = &list[face.id[2]];
    glm::vec3* D = nullptr;
    for(int i = 0; i < 4; i++) {
        bool found = false;
        for(int j = 0; j < 3; j++) {
            if(verticies[i] == face.id[j]) {
                found = true;
                break;
            }
        }
        if (!found) {
            D = &list[verticies[i]];
            break;
        }
    }
    //calculate face normal
    glm::vec3 AB = *B - *A;
    glm::vec3 AC = *C - *A;

    glm::vec3 N = glm::cross(AB,AC);

    if(glm::dot(N, *D - *A) < 0){
        N = -N;
    }

    return N;
}

QVector<Triangle_face> Tetra::get_faces()
{
    QList<Triangle_face> faces;
    uint a = verticies[0];
    uint b = verticies[1];
    uint c = verticies[2];
    uint d = verticies[3];

    faces.append(Triangle_face(a,b,c));
    faces.append(Triangle_face(b,d,c));
    faces.append(Triangle_face(d,a,c));
    faces.append(Triangle_face(a,b,d));
    return faces;
}
