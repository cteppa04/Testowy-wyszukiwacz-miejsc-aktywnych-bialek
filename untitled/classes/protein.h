#ifndef PROTEIN_H
#define PROTEIN_H

#include <classes/atom.h>
#include <QVector>

class Protein
{
public:
    Protein(QString name = "",QVector<Atom> atom_list = QVector<Atom>());
    ~Protein() = default;

    QString m_name;
    QVector<Atom>m_atom_list;
};

#endif // PROTEIN_H
