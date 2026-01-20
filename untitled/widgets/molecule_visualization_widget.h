#ifndef MOLECULE_VISUALIZATION_WIDGET_H
#define MOLECULE_VISUALIZATION_WIDGET_H

#include <QObject>
#include <QtOpenGLWidgets/QOpenGLWidget>
#include <qopenglfunctions.h>

class Molecule_visualization_widget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit Molecule_visualization_widget(QWidget *parent = nullptr);
    ~Molecule_visualization_widget();
protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
signals:
};

#endif // MOLECULE_VISUALIZATION_WIDGET_H
