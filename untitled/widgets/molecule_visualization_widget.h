#ifndef MOLECULE_VISUALIZATION_WIDGET_H
#define MOLECULE_VISUALIZATION_WIDGET_H

#include "classes/OpenGl/instance/object_instance.h"
#include "classes/OpenGl/resource_manager.h"
#include <QtOpenGLWidgets/QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <glm/glm.hpp>

#include <classes/camera.h>
#include <classes/protein.h>
#include <classes/OpenGl/shaders/shader_object.h>
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
    //interface
    void add_protein(Protein *protein);
    void delete_protein();
protected:
    //functions that must be overriden from QOpenGLWidget
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    //managers
    Resource_manager<Mesh> mesh_manager;
    Resource_manager<Material> material_manager;
    Resource_manager<Shader_object> shader_manager;
    Resource_manager<Object> object_manager;
    Object_instance *atoms;

    //initialise atom colors
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void showEvent(QShowEvent *event) override;

    CAMERA_H::Camera camera;
    std::map<std::string, glm::vec3> elements_colors = {
        {"H",  glm::vec3(1.0f, 1.0f, 1.0f)},   // White
        {"C",  glm::vec3(0.2f, 0.2f, 0.2f)},   // Dark gray
        {"N",  glm::vec3(0.0f, 0.0f, 1.0f)},   // Blue
        {"O",  glm::vec3(1.0f, 0.0f, 0.0f)},   // Red
        {"S",  glm::vec3(1.0f, 1.0f, 0.0f)},   // Yellow
        {"Se", glm::vec3(1.0f, 0.6f, 0.0f)},   // Orange
        {"P",  glm::vec3(1.0f, 0.5f, 0.0f)},   // Orange
        {"Cl", glm::vec3(0.0f, 1.0f, 0.0f)},   // Green
        {"Na", glm::vec3(0.5f, 0.5f, 1.0f)},   // Light blue
        {"K",  glm::vec3(0.5f, 0.0f, 1.0f)},   // Purple
        {"Ca", glm::vec3(0.5f, 0.5f, 0.0f)},   // Dark yellow
        {"Mg", glm::vec3(0.0f, 1.0f, 0.0f)},   // Green
        {"Fe", glm::vec3(0.8f, 0.4f, 0.0f)},   // Brown/Orange
        {"Cu", glm::vec3(1.0f, 0.5f, 0.5f)},   // Pink
        {"Zn", glm::vec3(0.5f, 0.5f, 0.5f)},   // Gray
        {"Co", glm::vec3(0.0f, 0.0f, 0.5f)},   // Dark blue
        {"I",  glm::vec3(0.58f, 0.0f, 0.58f)}  // Purple
    };;

public slots:

private:

};

#endif // MOLECULE_VISUALIZATION_WIDGET_H
