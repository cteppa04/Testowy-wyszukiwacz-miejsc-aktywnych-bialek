#ifndef MOLECULE_VISUALIZATION_WIDGET_H
#define MOLECULE_VISUALIZATION_WIDGET_H

#include <QtOpenGLWidgets/QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <glm/glm.hpp>

#include <classes/camera.h>
#include <classes/OpenGl/shader_wrapper.h>
#include <classes/OpenGl/geometry/geometry_master.h>
#include <classes/OpenGl/objects/object_master.h>

#include <QObject>
#include <QMessageBox>
#include <QDebug>
#include <QFile>

class Molecule_visualization_widget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    explicit Molecule_visualization_widget(QWidget *parent = nullptr);
protected:
    //functions that must be overriden from QOpenGLWidget
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    Sphere_object *test;
    Shader_wrapper shader_program;

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

    CAMERA_H::Camera camera;
signals:
};

#endif // MOLECULE_VISUALIZATION_WIDGET_H
