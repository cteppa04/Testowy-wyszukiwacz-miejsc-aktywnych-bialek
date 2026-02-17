#include "protein.h"


Protein::Protein(QString name,QVector<Atom> atom_list):
    m_name(std::move(name)),
    m_atom_list(std::move(atom_list))
{

}


