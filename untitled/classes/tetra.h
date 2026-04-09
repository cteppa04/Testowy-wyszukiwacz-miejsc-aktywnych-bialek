#ifndef TETRA_H
#define TETRA_H
#include <glm/glm.hpp>
#include <QVector>
#include <classes/triangle_face.h>
class Tetra
{
public:
    QVector<glm::vec3> *bound_list;
    QVector<uint> verticies;
    uint neighbours[4];
    //0 - abc
    //1 - bdc
    //2 - dac
    //3 - adb

    Tetra();
    Tetra(const uint a,const uint b,const uint c,const uint d, QVector<glm::vec3> *bound_list);

    void calculate_circum_sphere();
    bool point_insinde(glm::vec3 point);
    glm::vec3 get_face_normal(Triangle_face& face);
    QVector<Triangle_face> get_faces();
    glm::vec3 circ_sphere_center;
    float circ_sphere_radius;
private:
};

#endif // TETRA_H
