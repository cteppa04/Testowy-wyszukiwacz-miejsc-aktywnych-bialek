#include "atom.h"

Atom::Atom()
    : serial(0),
    position(0.0f, 0.0f, 0.0f),
    element("")
{

}

Atom::Atom(int serial, const QVector3D &position, const QString &element)
    : serial(serial),
    position(position),
    element(element)
{

}
