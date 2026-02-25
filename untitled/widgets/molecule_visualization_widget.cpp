#include <widgets/molecule_visualization_widget.h>
#include "classes/OpenGl/objects/object_factory.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <QElapsedTimer>
#include <QTimer>
#include <QKeyEvent>

#include <classes/OpenGl/shaders/shader_object.h>

#include <classes/OpenGl/geometry/geometry_master.h>
#include <classes/OpenGl/material/material_master.h>
#include <classes/OpenGl/objects/object.h>
#include <classes/OpenGl/resource_manager.h>


glm::mat4 view(1.0f);
glm::mat4 projection(1.0f);

//keyboard input array
QSet<int> keys;

//origin point camera initialization

const glm::vec3 CAMERA_STARTING_POS(0.0f,0.0f,10.0f);
glm::vec3 orbit_camera_orbit_point (0.0f,0.0f,0.0f);
glm::vec3 camera_position(CAMERA_STARTING_POS);

Molecule_visualization_widget::Molecule_visualization_widget(QWidget *parent)
    : QOpenGLWidget(parent)
{

    //handle camera
    camera.setType(CAMERA_H::Camera::Type::Free);
    camera.set_camera_position(glm::vec3(0.0f, 0.0f, -10.0f));
    camera.start();
    camera.set_camera_orbit_point(orbit_camera_orbit_point);

    //set fps
    QTimer *timer_refresh = new QTimer(this);
    connect(timer_refresh, &QTimer::timeout, this, QOverload<>::of(&Molecule_visualization_widget::update));
    timer_refresh->start(16); // ~60 FPS

}

void Molecule_visualization_widget::add_protein(Protein *protein)
{
    glm::vec3 avg_pos(0.0);

    for(int i = 0;i < protein->m_atom_list.count();++i){
        auto atom = protein->m_atom_list[i];

        //normalize atom color to read map properly
        std::string key = atom.m_element.toStdString();
        key.erase(0, key.find_first_not_of(" \t\n\r"));
        key.erase(key.find_last_not_of(" \t\n\r") + 1);

        if(!key.empty()){
            key[0] = std::toupper(key[0]);
            if(key.size() > 1){
                key[1] = std::toupper(key[1]);
            }
        }

        auto it = elements_data.find(key);

        glm::vec3 color = it->second.color;
        auto model = glm::mat4(1.0f);
        model = glm::translate(model,atom.m_position);
        model = glm::scale(model,glm::vec3(it->second.vdw_radius));
        vdw_radius->add_instance(model,color,1.0f);

        avg_pos += atom.m_position;

        model = glm::mat4(1.0);
        model = glm::translate(model,atom.m_position);
        model = glm::scale(model,glm::vec3(0.25f));
        cores->add_instance(model,glm::vec3(0,0,0),1.0f);
    }
    avg_pos /= protein->m_atom_list.count();
    camera.set_camera_direction(avg_pos);

    vdw_radius->updateGPU();
    cores->updateGPU();
}

void Molecule_visualization_widget::delete_protein()
{
    cores->clear();
    vdw_radius->clear();
}


void Molecule_visualization_widget::paintGL()
{
    view = camera.update(&keys);

    glEnable(GL_DEPTH_TEST); // enable depth testing
    glEnable(GL_STENCIL_TEST);
    glEnable(GL_DEPTH_TEST);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //draw cores
    //use program
    glUseProgram(cores->m_object->material->m_shader->shader_ID);
    //send matrices to shader program
    cores->m_object->material->m_shader->set_float("scale",1.0f);
    cores->m_object->material->m_shader->set_mat4("view",view);
    cores->m_object->material->m_shader->set_mat4("projection",projection);
    //render
    glBindVertexArray(cores->VAO);
    glDrawElementsInstanced(GL_TRIANGLES,
                            object_manager.get("atom")->mesh->indices.count(),
                            GL_UNSIGNED_INT, 0, cores->instance_count());

    //set stencil for first pass
    glStencilMask(0xFF);
    glClear(GL_STENCIL_BUFFER_BIT);
    glStencilOp(GL_KEEP,GL_KEEP,GL_REPLACE);
    glStencilFunc(GL_ALWAYS,1,0xFF);

    //draw vdw_radii
    //use program
    glUseProgram(vdw_radius->m_object->material->m_shader->shader_ID);
    //send matrices to shader program
    vdw_radius->m_object->material->m_shader->set_mat4("view",view);
    vdw_radius->m_object->material->m_shader->set_mat4("projection",projection);
    //render
    glBindVertexArray(vdw_radius->VAO);
    glDrawElementsInstanced(GL_TRIANGLES,
                            object_manager.get("atom")->mesh->indices.count(),
                            GL_UNSIGNED_INT, 0, vdw_radius->instance_count());

    glStencilFunc(GL_NOTEQUAL,1,0xFF);
    glStencilMask(0x00);
    glDisable(GL_DEPTH_TEST);

    glUseProgram(shader_manager.get("outline")->shader_ID);
    shader_manager.get("outline")->set_float("scale",1.2f);
    shader_manager.get("outline")->set_mat4("view",view);
    shader_manager.get("outline")->set_mat4("projection",projection);
    //render
    glBindVertexArray(vdw_radius->VAO);
    glDrawElementsInstanced(GL_TRIANGLES,
                            object_manager.get("atom")->mesh->indices.count(),
                            GL_UNSIGNED_INT, 0, cores->instance_count());
}


void Molecule_visualization_widget::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
}

void Molecule_visualization_widget::initializeGL()
{
    //set buffers and shit
    qDebug() << "OpenGL version:"
             << context()->format().majorVersion()
             << "." << context()->format().minorVersion();
    makeCurrent();
    initializeOpenGLFunctions();

    //create transformation matrces
    projection = glm::perspective(glm::radians(45.0f),800.0f/600.0f,0.1f,100.0f);

    //create atom object
    mesh_manager.add("atom_sphere",Mesh_factory::Sphere_mesh(16,16));

    shader_manager.add("basic",new Shader_object(":/resources/shaders/testShader.vert",
                                                  ":/resources/shaders/testShader2.fsh"));

    material_manager.add("smooth",Material_factory::material(shader_manager.get("basic")));

    object_manager.add("atom",Object_factory::sphere(mesh_manager.get("atom_sphere"),
                                                      material_manager.get("smooth")));
    //setup atoms
    cores = new Object_instance(object_manager.get("atom"));
    vdw_radius = new Object_instance(object_manager.get("atom"));

    shader_manager.add("outline",new Shader_object(":/resources/shaders/testShader.vert",
                                                    ":/resources/shaders/outline_shader.fsh"));

    shader_manager.add("OIT",new Shader_object(":/resources/shaders/testShader.vert",
                                                ":/resources/shaders/OIT.fsh"));

    cores->add_instance(glm::mat4(1.0f),glm::vec3(1.0,0.0,0.0),1.0f);
    cores->updateGPU();

    //create framebuffer for opaque objects;
    QSize screenSize = this->size();

    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    // Texture for accumulated color
    GLuint accumColorTex;
    glGenTextures(1, &accumColorTex);
    glBindTexture(GL_TEXTURE_2D, accumColorTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screenSize.width(), screenSize.height(), 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, accumColorTex, 0);

    // Texture for accumulated reveal
    GLuint accumRevealTex;
    glGenTextures(1, &accumRevealTex);
    glBindTexture(GL_TEXTURE_2D, accumRevealTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R16F, screenSize.width(), screenSize.height(), 0, GL_RED, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, accumRevealTex, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,accumColorTex,0);
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT1,GL_TEXTURE_2D,accumRevealTex,0);

    GLenum attachements[2] = {GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1};
    glDrawBuffers(2,attachements);

    glBindFramebuffer(GL_FRAMEBUFFER,0);
}

//keyboard key handling
void Molecule_visualization_widget::keyPressEvent(QKeyEvent *event) {
    if (!event->isAutoRepeat()) {
        keys.insert(event->key());
    }
}

void Molecule_visualization_widget::keyReleaseEvent(QKeyEvent *event) {
    if (!event->isAutoRepeat()) {
        keys.remove(event->key());
    }
}

void Molecule_visualization_widget::showEvent(QShowEvent *event)
{
    grabKeyboard();
    QOpenGLWidget::showEvent(event);
}
