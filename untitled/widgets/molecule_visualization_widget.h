#ifndef MOLECULE_VISUALIZATION_WIDGET_H
#define MOLECULE_VISUALIZATION_WIDGET_H

#include <QtOpenGLWidgets/QOpenGLWidget>
#include <QObject>
#include <QMessageBox>
#include <QFile>
#include <QDebug>
#include <QOpenGLFunctions_3_3_Core>

class Molecule_visualization_widget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    explicit Molecule_visualization_widget(QWidget *parent = nullptr);
    ~Molecule_visualization_widget();
protected:
    //functions that must be overriden from QOpenGLWidget
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    GLuint VAO[2];
    GLuint VBO[2];
    GLuint shader_program;
signals:
};

#endif // MOLECULE_VISUALIZATION_WIDGET_H
