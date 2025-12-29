#ifndef PARSEPDB_H
#define PARSEPDB_H

#include <QVector>
#include <QString>
#include "classes/Atom.h"

QVector<Atom> parsePDB(const QString &filePath);

#endif // PARSEPDB_H
