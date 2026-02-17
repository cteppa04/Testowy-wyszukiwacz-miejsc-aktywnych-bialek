#ifndef ATOM_H
#define ATOM_H
#include <QVector3D>
#include <QString>
#include <glm/glm.hpp>

class Atom
{
public:
    Atom(int serial = 0, glm::vec3 position = glm::vec3(0.0), QString element = "");

    int m_serial;
    glm::vec3 m_position;
    QString m_element;
};

#endif // ATOM_H
