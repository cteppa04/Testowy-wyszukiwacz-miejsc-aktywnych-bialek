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

    void set_animation_step(int step);
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
    Object_instance *cores;
    Object_instance *vdw_radius;
    Object_instance *probe_radius;
    Object_instance *lines;
    Object_instance *active_sites;

    GLuint fbo;

    //initialise atom colors
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void showEvent(QShowEvent *event) override;

    CAMERA_H::Camera camera;

    struct Element_data {
        glm::vec3 color;
        float vdw_radius;
    };

    std::map<std::string, Element_data> elements_data = {
    {"H",  {{1.0f, 1.0f, 1.0f}, 1.20f}},  // White
    {"C",  {{0.2f, 0.2f, 0.2f}, 1.70f}},  // Dark gray
    {"N",  {{0.0f, 0.0f, 1.0f}, 1.55f}},  // Blue
    {"O",  {{1.0f, 0.0f, 0.0f}, 1.52f}},  // Red
    {"S",  {{1.0f, 1.0f, 0.0f}, 1.80f}},  // Yellow
    {"Se", {{1.0f, 0.6f, 0.0f}, 1.90f}},  // Orange
    {"P",  {{1.0f, 0.5f, 0.0f}, 1.80f}},  // Orange
    {"Cl", {{0.0f, 1.0f, 0.0f}, 1.75f}},  // Green
    {"Na", {{0.5f, 0.5f, 1.0f}, 2.27f}},  // Light blue
    {"K",  {{0.5f, 0.0f, 1.0f}, 2.75f}},  // Purple
    {"Ca", {{0.5f, 0.5f, 0.0f}, 2.31f}},  // Dark yellow
    {"Mg", {{0.0f, 1.0f, 0.0f}, 1.73f}},  // Green
    {"Fe", {{0.8f, 0.4f, 0.0f}, 1.94f}},  // Brown/Orange
    {"Cu", {{1.0f, 0.5f, 0.5f}, 1.96f}},  // Pink
    {"Zn", {{0.5f, 0.5f, 0.5f}, 2.01f}},  // Gray
    {"Co", {{0.0f, 0.0f, 0.5f}, 1.92f}},  // Dark blue
    {"I",  {{0.58f, 0.0f, 0.58f}, 1.98f}} // Purple
    };;

public slots:

private:
    int current_animation_step;
    QElapsedTimer animation_timer;

    void draw_cores();
    void draw_vdw_radii();
    void draw_radii_outline();
    void draw_transparent_radii();
    void draw_probe_radius();

    GLuint screenVAO;
    GLuint accumColorTex;
    GLuint accumRevealTex;
};

#endif // MOLECULE_VISUALIZATION_WIDGET_H
