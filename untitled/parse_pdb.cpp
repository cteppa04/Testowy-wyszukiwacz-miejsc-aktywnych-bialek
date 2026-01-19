#include <QFile>
#include <QTextStream>
#include <QVector3D>
#include <QDebug>

#include "atom.h"

QVector<Atom> parsePDB(const QString &filePath)
{
    QVector<Atom> atoms;

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Cannot open PDB file";
        return atoms;
    }

    QTextStream in(&file);

    while (!in.atEnd()) {
        QString line = in.readLine();

        // Only ATOM and HETATM records
        if (!line.startsWith("ATOM") && !line.startsWith("HETATM"))
            continue;

        // Defensive check for short lines
        if (line.length() < 78)
            continue;

        int serial = line.mid(6, 5).trimmed().toInt();

        float x = line.mid(30, 8).trimmed().toFloat();
        float y = line.mid(38, 8).trimmed().toFloat();
        float z = line.mid(46, 8).trimmed().toFloat();

        QString element = line.mid(76, 2).trimmed();

        QVector3D position(x, y, z);

        atoms.append(Atom(serial, position, element));
    }

    file.close();
    return atoms;
}
