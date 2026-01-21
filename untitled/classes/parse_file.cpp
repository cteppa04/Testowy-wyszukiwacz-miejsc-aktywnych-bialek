#include <QFile>
#include <QTextStream>
#include <QVector3D>
#include <QDebug>
#include "classes/parse_file.h"
#include "classes/atom.h"


QVector<Atom> Parse_file::parse_PDB(const QString &file_path)
{
    QVector<Atom> atoms;

    //check if file can be opened
    QFile file(file_path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Cannot open PDB file";
        return atoms;
    }

    QTextStream in(&file);

    //scan while file
    while (!in.atEnd()) {
        QString line = in.readLine();
        //search of ATOM or HETATM entries
        if (!line.startsWith("ATOM") && !line.startsWith("HETATM"))
            continue;

        if (line.length() < 78)
            continue;
        //trim parts of the entry into the atom variables
        int serial = line.mid(6, 5).trimmed().toInt();

        float x = line.mid(30, 8).trimmed().toFloat();
        float y = line.mid(38, 8).trimmed().toFloat();
        float z = line.mid(46, 8).trimmed().toFloat();

        QString element = line.mid(76, 2).trimmed();

        QVector3D position(x, y, z);
        //add atom to list
        atoms.append(Atom(serial, position,element));
    }
    file.close();
    return atoms;
}

