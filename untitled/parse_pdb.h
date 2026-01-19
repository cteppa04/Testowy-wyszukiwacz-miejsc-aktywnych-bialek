#ifndef PARSE_PDB_H
#define PARSE_PDB_H

#include <QVector>
#include <QString>
#include "atom.h"

QVector<Atom> parsePDB(const QString &filePath);

#endif // PARSE_PDB_H
