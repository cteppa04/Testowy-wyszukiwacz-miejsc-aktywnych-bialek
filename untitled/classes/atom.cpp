#include "atom.h"


Atom::Atom(int serial, const glm::vec3 position, const QString element):
    m_serial(serial),
    m_position(position),
    m_element(element)
{
}
