#ifndef TETRA_H
#define TETRA_H
#include "classes/triangle_face.h"
#include <glm/glm.hpp>
#include <QVector>
class Tetra
{
public:
    struct neighbour{
        int index;
        glm::vec3 normal;
        Triangle_face face;

        neighbour()
            : index(-2),
            normal(glm::vec3(0.0f)),
            face(0,0,0) {}
    };

    Tetra(const int a,const int b,const int c,const int d, QVector<glm::vec3> *bound_list);
    QVector<glm::vec3> *bound_list;
    bool alive;

    QVector<int> verticies;
    neighbour neighbours[4];

    glm::vec3 circ_sphere_center;
    float circ_sphere_radius;

    //methods
    bool point_insinde(glm::vec3 point);

    void calculate_circum_sphere();

    glm::vec3 get_face_normal(const Triangle_face& face);
    QVector<Triangle_face> get_faces();

    int find_reconnect_neighbour_slot();
private:
};

#endif // TETRA_H
