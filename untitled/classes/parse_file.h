#ifndef PARSE_FILE_H
#define PARSE_FILE_H

#include <QVector>
#include <QString>
#include "classes/atom.h"

class Parse_file
{
public:
    Parse_file() = delete;
    static QVector<Atom> parse_PDB(const QString &file_path);
};


#endif // PARSE_FILE_H
