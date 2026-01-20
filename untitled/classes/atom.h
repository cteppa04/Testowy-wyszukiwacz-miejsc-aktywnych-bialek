#ifndef ATOM_H
#define ATOM_H
#include <QVector3D>
#include <QString>

class Atom
{
public:
    Atom();
    Atom(int serial, const QVector3D &position, const QString &element);

    int serial;
    QVector3D position;
    QString element;
};

#endif // ATOM_H
