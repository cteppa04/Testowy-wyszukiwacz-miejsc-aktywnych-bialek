#ifndef TETRA_H
#define TETRA_H
#include <glm/glm.hpp>
#include <QVector>
#include <classes/triangle_face.h>
class Tetra
{
public:
    QVector<glm::vec3> *bound_list;
    uint a;
    uint b;
    uint c;
    uint d;

    Tetra(const uint a,const uint b,const uint c,const uint d, QVector<glm::vec3> *bound_list)
        : bound_list(bound_list), a(a), b(b), c(c), d(d) {}

    void calculate_circum_sphere();
    bool point_insinde(glm::vec3 point);
    QVector<Triangle_face> get_faces();
private:
    glm::vec3 circ_sphere_center;
    float circ_sphere_radius;
};

#endif // TETRA_H
