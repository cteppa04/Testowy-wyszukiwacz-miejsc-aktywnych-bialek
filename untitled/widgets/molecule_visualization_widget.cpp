#include <widgets/molecule_visualization_widget.h>
#include "classes/OpenGl/objects/object_factory.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/common.hpp>

#include <QElapsedTimer>
#include <QTimer>
#include <QKeyEvent>
#include <QStack>

#include <classes/OpenGl/shaders/shader_object.h>

#include <classes/OpenGl/geometry/geometry_master.h>
#include <classes/OpenGl/material/material_master.h>
#include <classes/OpenGl/objects/object.h>
#include <classes/OpenGl/resource_manager.h>

#include <classes/tetra.h>
#include <classes/triangle_face.h>
#include <unordered_map>

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
    camera.set_camera_position(glm::vec3(0.0f, 0.0f, 20.0f));
    camera.set_camera_direction(glm::vec3(0,0,0));
    camera.start();
    camera.set_camera_orbit_point(orbit_camera_orbit_point);

    //set fps
    QTimer *timer_refresh = new QTimer(this);
    connect(timer_refresh, &QTimer::timeout, this, QOverload<>::of(&Molecule_visualization_widget::update));
    timer_refresh->start(16); // ~60 FPS

}


void Molecule_visualization_widget::add_protein(Protein *protein)
{
    current_protein = protein;
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
        vdw_radius->add_instance(atom.m_position,glm::vec3(it->second.vdw_radius),glm::mat4(1),color,0.15f);

        avg_pos += atom.m_position;

        float scale = 0.2f;
        cores->add_instance(atom.m_position,glm::vec3(scale),glm::mat4(1),color,1.0); //colored cores

    }
    avg_pos /= protein->m_atom_list.count();
    camera.set_camera_direction(avg_pos);

    vdw_radius->updateGPU();
    cores->updateGPU();

    cores->clear();
    cores->updateGPU();

    camera.set_camera_direction(avg_pos);

    //calculate deluay triangulation
    create_deluay_triangulation();
}

void Molecule_visualization_widget::delete_protein()
{
    cores->clear();
    vdw_radius->clear();
    triangulation_lines->clear();
}

void Molecule_visualization_widget::set_animation_step(int step)
{
    current_animation_step = step;
    animation_timer.start();
}

void Molecule_visualization_widget::draw_cores(){

    //set state
    glEnable(GL_DEPTH_TEST); // enable depth testing
    glEnable(GL_CULL_FACE);

    //draw cores
    //use program
    auto shader = cores->m_object->material->m_shader;
    glUseProgram(shader->shader_ID);
    //send matrices to shader program
    shader->set_mat4("view",view);
    shader->set_mat4("projection",projection);
    shader->set_float("scale",1.0f);
    shader->set_bool("probe",false);
    //render
    glBindVertexArray(cores->VAO);
    glDrawElementsInstanced(GL_TRIANGLES,
                            object_manager.get("atom")->mesh->indices.count(),
                            GL_UNSIGNED_INT, 0, cores->instance_count());

    //restore state
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
}

void Molecule_visualization_widget::draw_vdw_radii(){
    //state start
    glEnable(GL_DEPTH_TEST);

    //set program
    auto shader = vdw_radius->m_object->material->m_shader;
    glUseProgram(shader->shader_ID);

    //send matrices to shader program
    shader->set_float("scale",1.0f);
    shader->set_mat4("view",view);
    shader->set_mat4("projection",projection);
    shader->set_bool("force_opaque",true);
    shader->set_bool("probe",false);
    //render
    glBindVertexArray(vdw_radius->VAO);
    glDrawElementsInstanced(GL_TRIANGLES,
                            vdw_radius->m_object->mesh->indices.count(),
                            GL_UNSIGNED_INT,
                            0,
                            vdw_radius->instance_count());

    //state restore
    glDisable(GL_DEPTH_TEST);
}

void Molecule_visualization_widget::draw_radii_outline(){
    //enabl stencil test
    glEnable(GL_STENCIL_TEST);
    glEnable(GL_CULL_FACE);

    //set stencil for first pass
    glStencilMask(0xFF);
    glClear(GL_STENCIL_BUFFER_BIT);
    glStencilOp(GL_KEEP,GL_KEEP,GL_REPLACE);
    glStencilFunc(GL_ALWAYS,1,0xFF);

    //disable drawing to screen color
    glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);

    //draw vdw_radii
    //use program
    auto shader = vdw_radius->m_object->material->m_shader;
    glUseProgram(shader->shader_ID);
    //send matrices to shader program
    shader->set_float("scale",1.0f);
    shader->set_mat4("view",view);
    shader->set_mat4("projection",projection);
    shader->set_bool("force_opaque",true);
    shader->set_bool("probe",false);
    //render
    glBindVertexArray(vdw_radius->VAO);
    glDrawElementsInstanced(GL_TRIANGLES,
                            object_manager.get("atom")->mesh->indices.count(),
                            GL_UNSIGNED_INT, 0, vdw_radius->instance_count());

    //turn on drawing to screen color buffer
    glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);

    glStencilFunc(GL_NOTEQUAL,1,0xFF);
    glStencilMask(0x00);

    glUseProgram(shader_manager.get("outline")->shader_ID);
    shader_manager.get("outline")->set_float("scale",1.2f);
    shader_manager.get("outline")->set_mat4("view",view);
    shader_manager.get("outline")->set_mat4("projection",projection);
    //render
    glBindVertexArray(vdw_radius->VAO);
    glDrawElementsInstanced(GL_TRIANGLES,
                            object_manager.get("atom")->mesh->indices.count(),
                            GL_UNSIGNED_INT, 0, cores->instance_count());

    glStencilMask(0xFF);
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_CULL_FACE);
}

void Molecule_visualization_widget::draw_transparent_radii(){

    glBindFramebuffer(GL_FRAMEBUFFER,fbo);

    GLenum drawBuffers[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, drawBuffers);

    //transparent pass for OIT texture
    auto OITShader = shader_manager.get("OITSetTexture");

    glUseProgram(OITShader->shader_ID);
    //clear the frambuffer
    glClearBufferfv(GL_COLOR,0,(float[]){0.0,0.0,0.0});

    glClearBufferfv(GL_COLOR,1,(float[]){1.0});

    //fill the accColor and accRevelage textures
    //send matrices to shader program
    OITShader->set_float("weight",0.2f);
    OITShader->set_float("scale", 1.0f);
    OITShader->set_mat4("view",view);
    OITShader->set_mat4("projection",projection);

    //render
    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_ONE,GL_ONE,GL_ZERO,GL_ONE_MINUS_SRC_ALPHA);
    glBindVertexArray(vdw_radius->VAO);
    glDrawElementsInstanced(GL_TRIANGLES,
                            object_manager.get("atom")->mesh->indices.count(),
                            GL_UNSIGNED_INT, 0, vdw_radius->instance_count());
    glDisable(GL_BLEND);
    //textures should be occupied by the oit values;
    glBindFramebuffer(GL_FRAMEBUFFER,defaultFramebufferObject());

    //set program
    auto shader = shader_manager.get("OITCompositePass");
    glUseProgram(shader->shader_ID);
    glActiveTexture(GL_TEXTURE0);

    glBindTexture(GL_TEXTURE_2D,accumColorTex);
    auto loc = glGetUniformLocation(shader->shader_ID,"textColor");
    glUniform1i(loc,0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,accumRevealTex);
    loc = glGetUniformLocation(shader->shader_ID,"textRevelage");
    glUniform1i(loc,1);

    glBindVertexArray(screenVAO);
    glEnable(GL_BLEND);

    glBlendFuncSeparate(GL_ONE,GL_ONE,GL_SRC_ALPHA,GL_DST_ALPHA);
    glDrawArrays(GL_TRIANGLES,0,3);
    //state cleanup
    glDisable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ZERO);

    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);

    glDrawBuffer(GL_BACK);

    glViewport(0,0,width(),height());
}

void Molecule_visualization_widget::draw_probe_radius(){
    glEnable(GL_DEPTH_TEST); // enable depth testing

    //draw probe
    //use program
    auto shader = vdw_radius->m_object->material->m_shader;
    glUseProgram(shader->shader_ID);
    //send matrices to shader program
    shader->set_float("scale",1.0f);
    shader->set_mat4("view",view);
    shader->set_mat4("projection",projection);
    shader->set_bool("probe",true);
    shader->set_float("probe_size",1.4);
    shader->set_vec3("probe_color",glm::vec3(0.0,0.0,1.0));
    //render
    glBindVertexArray(vdw_radius->VAO);
    glDrawElementsInstanced(GL_TRIANGLES,
                            object_manager.get("atom")->mesh->indices.count(),
                            GL_UNSIGNED_INT, 0, vdw_radius->instance_count());
    glDisable(GL_DEPTH_TEST);
}

void Molecule_visualization_widget::add_line(glm::vec3 from, glm::vec3 to, float radius,glm::vec3 color)
{
    float distance = glm::distance(from,to);
    glm::vec3 scale = glm::vec3(distance,radius,radius);
    glm::vec3 dir = glm::normalize(to - from);
    glm::vec3 base = glm::vec3(1,0,0);

    double dot_product = glm::dot(dir,base);
    dot_product = glm::clamp(dot_product,-1.0,1.0);

    glm::vec3 axis = glm::cross(base,dir);
    glm::mat4 rotation_matrix(1.0);

    if(glm::length(axis) < 0.0001){
        //prostopadle
        if(dot_product > 0.0){
            //zgodne
        }else{
            //przeciwne
            rotation_matrix = glm::rotate(rotation_matrix,glm::pi<float>(),glm::vec3(0,1,0));
        }
    }else{
        axis = glm::normalize(axis);
        float angle = acos(dot_product);
        rotation_matrix = glm::rotate(rotation_matrix,angle,axis);
    }
    triangulation_lines->add_instance(from,glm::vec3(distance,radius,radius),rotation_matrix,color,1.0);
}

void Molecule_visualization_widget::draw_triangulation_lines()
{
    //set state
    glEnable(GL_DEPTH_TEST); // enable depth testing
    glEnable(GL_CULL_FACE);

    //draw cores
    //use program
    auto shader = triangulation_lines->m_object->material->m_shader;
    glUseProgram(shader->shader_ID);
    //send matrices to shader program
    shader->set_float("scale",1.0f);
    shader->set_mat4("view",view);
    shader->set_mat4("projection",projection);
    shader->set_bool("probe",false);
    //render
    glBindVertexArray(triangulation_lines->VAO);
    glDrawElementsInstanced(GL_TRIANGLES,
                            triangulation_lines->m_object->mesh->indices.count(),
                            GL_UNSIGNED_INT, 0, triangulation_lines->instance_count());

    //restore state
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
}

void Molecule_visualization_widget::create_deluay_triangulation()
{
    const int EMPTY_NEIGHBOUR = -1;
    const int RECONNECT_NEIGHBOUR = -2;
    QVector<glm::vec3> triangulation_points;
    QVector<Tetra> tetra_list;
    std::unordered_map<Triangle_face,QList<int>,Triangle_face_hash> face_map;
    QVector<int>dead_tetra_indexes;

    //dodaj super tetra
    triangulation_points.append(create_super_tetra());
    for(Atom atom : current_protein->m_atom_list){
        triangulation_points.append(atom.m_position);
    }

    //dodaj tetra do listy
    tetra_list.append(Tetra(0,1,2,3,&triangulation_points));
    //dodaj sasiadow
    for(int i = 0; i < 4; i++){
        tetra_list[0].neighbours[i].index = EMPTY_NEIGHBOUR;
    }

    int start;
    for(int x = 4; x < triangulation_points.size(); x++){
        //dla każdego punktu
        qDebug() << x;
        //qDebug() << "point: " << triangulation_points[x].x << " " << triangulation_points[x].y << " " << triangulation_points[x].z << " ";

        start = tetra_list.size() - 1;
        glm::vec3 point = triangulation_points[x];
        // qDebug() << tetra_list[start].point_insinde(point);
        // int bac = 0;
        // for(Tetra t :tetra_list){
        //     if(!t.alive) continue;
        //     qDebug() << bac;
        //     bac++;
        //     qDebug() << "A: " << triangulation_points[t.verticies[0]].x << " " << triangulation_points[t.verticies[0]].y << " " << triangulation_points[t.verticies[0]].z;
        //     qDebug() << "B: " << triangulation_points[t.verticies[1]].x << " " << triangulation_points[t.verticies[1]].y << " " << triangulation_points[t.verticies[1]].z;
        //     qDebug() << "C: " << triangulation_points[t.verticies[2]].x << " " << triangulation_points[t.verticies[2]].y << " " << triangulation_points[t.verticies[2]].z;
        //     qDebug() << "D: " << triangulation_points[t.verticies[3]].x << " " << triangulation_points[t.verticies[3]].y << " " << triangulation_points[t.verticies[3]].z;
        // }
        //tetra doesn't contain the point
        //find first invalid tetra by walking
        while(!tetra_list[start].point_insinde(point)){
            auto neighbours = tetra_list[start].neighbours;
            int smallest_index;
            float smallest_distance = MAXFLOAT;
            for(int i = 0; i < 4; i++){
                if(neighbours[i].index == EMPTY_NEIGHBOUR || neighbours[i].index == RECONNECT_NEIGHBOUR) continue;
                float distance = glm::distance(point,tetra_list[neighbours[i].index].circ_sphere_center);
                if(smallest_distance > distance){
                    smallest_distance = distance;
                    smallest_index = neighbours[i].index;
                }
            }
            start = smallest_index;
        }

        //tetra containts the point
        //fill neighbours till full cavity filled
        QStack<int> stack;
        QSet<int> visited;
        QList<int> cavity;

        stack.push(start);

        while(!stack.isEmpty()){
            int tetra = stack.pop();

            if(visited.contains(tetra)) continue;//continue if already visited

            visited.insert(tetra);

            if(tetra_list[tetra].point_insinde(point)){
                //invalid tetra
                cavity.append(tetra);
                for(int i = 0; i < 4; i++){
                    //if neighbour exists and not visited add to stack
                    if(tetra_list[tetra].neighbours[i].index == EMPTY_NEIGHBOUR ||
                        visited.contains(tetra_list[tetra].neighbours[i].index)) continue;
                    stack.push(tetra_list[tetra].neighbours[i].index);
                }
            }
        }
        //set tetra as dead
        //find boundary faces
        //delete connections to dead tetra
        //save affected tetra
        QSet<int> affected;
        face_map.clear();
        QVector<Triangle_face> boundary_faces;
        for(int t : cavity){
            //set tetra as dead
            tetra_list[t].alive = false;
            dead_tetra_indexes.append(t);
            //find boundary faces
            //add face to face map
            auto cavity_faces = tetra_list[t].get_faces();
            for(auto face : cavity_faces){
                face_map[face].append(t);
            }
            for(int i = 0; i < 4; i++){
                //for each neighbour of the dead tetra sever the connection
                int neight = tetra_list[t].neighbours[i].index;
                if(neight == EMPTY_NEIGHBOUR || neight == RECONNECT_NEIGHBOUR){
                    continue;
                }
                for(int j = 0; j < 4; j++){
                    //for each neihgbour of connected neighbour to dead tetra
                    //delete connections to dead tetra

                    if(tetra_list[neight].neighbours[j].index == t){
                        tetra_list[neight].neighbours[j].index = RECONNECT_NEIGHBOUR;
                    }

                    //save affected tetra
                    if(!cavity.contains(neight)){
                        affected.insert(neight);
                    }
                }
            }
        }
        //find boundary faces
        //fill boundary faces
        for (const auto &pair : face_map) {
            if (pair.second.size() == 1) {
                boundary_faces.append(pair.first);
            }
        }

        //create new tetra
        QSet<int> reconfiguration_tetra = affected;
        QList<Tetra> new_tetra;
        for(const Triangle_face &face : boundary_faces){
            Tetra t(face.id[0],face.id[1],face.id[2],x,&triangulation_points);
            new_tetra.append(t);
            tetra_list.append(t);
            reconfiguration_tetra.insert(tetra_list.size() - 1);
        }

        //retriangulate cavity and affected
        face_map.clear();
        //add all faces that need reconfiguration
        for(auto tetra : reconfiguration_tetra){
            auto faces = tetra_list[tetra].get_faces();
            for(auto face : faces){
                face_map[face].append(tetra);
            }
        }
        //for each face
        for(auto const &face : face_map){
            if(face.second.size() != 2) continue;

            //for two connection tetra
            int a_index = face.second[0];
            int b_index = face.second[1];

            Tetra* a = &tetra_list[a_index];
            Tetra* b = &tetra_list[b_index];

            //skip if already connected
            bool alreadyNeighbours = false;
            for (int i = 0; i < 4; i++) {
                if (a->neighbours[i].index == b_index) {
                    alreadyNeighbours = true;
                    break;
                }
            }
            if (alreadyNeighbours) continue;

            int slot_a = a->find_reconnect_neighbour_slot();
            int slot_b = b->find_reconnect_neighbour_slot();
            //fill tetra a
            a->neighbours[slot_a].index = b_index;
            a->neighbours[slot_a].face = face.first;
            a->neighbours[slot_a].normal = a->get_face_normal(face.first);
            //fill tetra b
            b->neighbours[slot_b].index = a_index;
            b->neighbours[slot_b].face = face.first;
            b->neighbours[slot_b].normal = a->get_face_normal(face.first);
        }

        //convert hanging faces to empty neighbours
        for(auto t : reconfiguration_tetra){
            auto* tetra = &tetra_list[t];
            for(int i = 0; i < 4; i++){
                if (tetra->neighbours[i].index == -2) {
                    tetra->neighbours[i].index = -1;
                }
            }
        }
    }

    for(Tetra tetra : tetra_list){
        qDebug() << "(" << tetra.verticies[0] << " " << tetra.verticies[1] << " " << tetra.verticies[2] << " " << tetra.verticies[3] << ")";
        cores->add_instance(triangulation_points[tetra.verticies[0]],glm::vec3(0.1),glm::mat4(1.0),glm::vec3(1,0,0),1.0);
        cores->add_instance(triangulation_points[tetra.verticies[1]],glm::vec3(0.1),glm::mat4(1.0),glm::vec3(0,1,0),1.0);
        cores->add_instance(triangulation_points[tetra.verticies[2]],glm::vec3(0.1),glm::mat4(1.0),glm::vec3(0,0,1),1.0);
        cores->add_instance(triangulation_points[tetra.verticies[3]],glm::vec3(0.1),glm::mat4(1.0),glm::vec3(1,1,0),1.0);

        add_line(triangulation_points[tetra.verticies[0]],triangulation_points[tetra.verticies[3]],0.01,glm::vec3(0,1,0));
        add_line(triangulation_points[tetra.verticies[0]],triangulation_points[tetra.verticies[1]],0.01,glm::vec3(0,1,0));
        add_line(triangulation_points[tetra.verticies[0]],triangulation_points[tetra.verticies[2]],0.01,glm::vec3(0,1,0));
        add_line(triangulation_points[tetra.verticies[1]],triangulation_points[tetra.verticies[3]],0.01,glm::vec3(0,1,0));
        add_line(triangulation_points[tetra.verticies[1]],triangulation_points[tetra.verticies[2]],0.01,glm::vec3(0,1,0));
        add_line(triangulation_points[tetra.verticies[2]],triangulation_points[tetra.verticies[3]],0.01,glm::vec3(0,1,0));
    }
    cores->updateGPU();
    triangulation_lines->updateGPU();
}

QVector<glm::vec3> Molecule_visualization_widget::create_super_tetra()
{
    glm::vec3 centroid(0,0,0);
    for(Atom atom: current_protein->m_atom_list){
        centroid+=atom.m_position;
    }
    centroid /= float(current_protein->m_atom_list.size());

    float radius = 0.0f;
    for(Atom atom: current_protein->m_atom_list){
        float distance = glm::distance(atom.m_position,centroid);
        if(distance > radius){
            radius = distance;
        }
    }
    radius *= 1.2; // make sure no clipping occurs

    float R_tetra = radius * sqrt(3);
    glm::vec3 unit_tetra[4] = {
       glm::vec3( 1,  1,  1),
       glm::vec3(-1, -1,  1),
       glm::vec3(-1,  1, -1),
       glm::vec3( 1, -1, -1)
    };

    QVector<glm::vec3> tetra_verticies;
    for(int i = 0; i < 4; ++i){
        tetra_verticies.append(centroid + unit_tetra[i] * R_tetra);
    }
    return tetra_verticies;
}

void Molecule_visualization_widget::paintGL()
{
    view = camera.update(&keys);

    glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    switch(current_animation_step){
    case 1:{
        draw_cores();
        draw_triangulation_lines();

        //draw_cores();
        break;
    }
    case 2:{
        draw_cores();
        draw_radii_outline();
        draw_transparent_radii();
        break;
    }
    case 3:{
        draw_vdw_radii();
        break;
    }
    case 4:{
        draw_probe_radius();
        break;
    }
    case 5:{
        draw_cores();
        draw_triangulation_lines();
    }
    default:{
        break;
    }
    }

    // draw_cores();

    // draw_radii_outline();

    // draw_transparent_radii();
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
    projection = glm::perspective(glm::radians(45.0f),800.0f/600.0f,0.1f,300.0f);

    //create atom object
    mesh_manager.add("atom_sphere",Mesh_factory::Sphere_mesh(16,16));
    shader_manager.add("basic",new Shader_object(":/resources/shaders/testShader.vert",
                                                  ":/resources/shaders/testShader2.fsh"));

    material_manager.add("smooth",Material_factory::material(shader_manager.get("basic")));

    object_manager.add("atom",Object_factory::Basic(mesh_manager.get("atom_sphere"),
                                                      material_manager.get("smooth")));
    //add line
    mesh_manager.add("triangulation_line",Mesh_factory::Line_mesh(6));

    object_manager.add("triangulation_line",Object_factory::Basic(mesh_manager.get("triangulation_line"),
                                                                   material_manager.get("smooth")));
    //setup atoms
    cores = new Object_instance(object_manager.get("atom"));
    vdw_radius = new Object_instance(object_manager.get("atom"));
    probe_radius = new Object_instance(object_manager.get("atom"));
    triangulation_lines = new Object_instance(object_manager.get("triangulation_line"));

    shader_manager.add("outline",new Shader_object(":/resources/shaders/testShader.vert",
                                                    ":/resources/shaders/outline_shader.fsh"));

    shader_manager.add("OITSetTexture",new Shader_object(":/resources/shaders/testShader.vert",
                                                ":/resources/shaders/OIT.fsh"));

    shader_manager.add("textureDraw",new Shader_object(":/resources/shaders/fullScreen.vert",
                                                ":/resources/shaders/textureOutput.fsh"));

    shader_manager.add("OITCompositePass",new Shader_object(":/resources/shaders/fullScreen.vert",
                                                             ":/resources/shaders/OITCompositePass.frag"));

    //test spheres
    cores->add_instance(glm::vec3(10,0,0),glm::vec3(1.25),glm::mat4(1),glm::vec3(1.0,0.0,0.0),1.0);
    cores->add_instance(glm::vec3(0,0,0),glm::vec3(1.25),glm::mat4(1),glm::vec3(1.0,0.0,0.0),1.0);
    cores->add_instance(glm::vec3(0,-10,0),glm::vec3(1.25),glm::mat4(1),glm::vec3(1.0,0.0,0.0),1.0);
    cores->add_instance(glm::vec3(10,-10,0),glm::vec3(1.25),glm::mat4(1),glm::vec3(1.0,0.0,0.0),1.0);
    cores->add_instance(glm::vec3(0,0,10),glm::vec3(1.25),glm::mat4(1),glm::vec3(1.0,0.0,0.0),1.0);
    cores->add_instance(glm::vec3(10,10,10),glm::vec3(1.25),glm::mat4(1),glm::vec3(1.0,0.0,0.0),1.0);
    cores->updateGPU();

    // vdw_radius->add_instance(glm::vec3(0),1.2,glm::vec3(0),glm::vec3(0.0,1.0,0.0),0.25);
    // vdw_radius->updateGPU();

    //test line
    add_line(glm::vec3(0),glm::vec3(10,0,0),0.5,glm::vec3(0.0,1.0,0.0));
    add_line(glm::vec3(0),glm::vec3(-10,0,0),0.5,glm::vec3(0.0,1.0,0.0));
    add_line(glm::vec3(0),glm::vec3(0,-10,0),0.5,glm::vec3(0.0,1.0,0.0));
    add_line(glm::vec3(0),glm::vec3(0,0,-10),0.5,glm::vec3(0.0,1.0,0.0));
    add_line(glm::vec3(0),glm::vec3(10,0,0),0.5,glm::vec3(0.0,1.0,0.0));
    add_line(glm::vec3(0),glm::vec3(0,10,0),0.5,glm::vec3(0.0,1.0,0.0));
    add_line(glm::vec3(0),glm::vec3(0,0,10),0.5,glm::vec3(0.0,1.0,0.0));
    triangulation_lines->updateGPU();
    // add_line(glm::vec3(0.0,0.0,10.0),glm::vec3(0,0,0),0.5,glm::vec3(0.0,1.0,0.0));
    // add_line(glm::vec3(0.0,0.0,10.0),glm::vec3(10,0,0),0.5,glm::vec3(0.0,1.0,0.0));
    // add_line(glm::vec3(0.0,0.0,20.0),glm::vec3(0,0,10),0.5,glm::vec3(0.0,1.0,0.0));
    // add_line(glm::vec3(0.0,0.0,0),glm::vec3(0,-10,0),0.5,glm::vec3(0.0,1.0,0.0));
    // add_line(glm::vec3(0,0,10),glm::vec3(0,-10,0),0.5,glm::vec3(0.0,1.0,0.0));
    // add_line(glm::vec3(10,10,10),glm::vec3(0,0,0),0.5,glm::vec3(0.0,1.0,0.0));

    //create framebuffer for opaque objects;
    QSize screenSize = this->size();

    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    // Texture for accumulated color
    glGenTextures(1, &accumColorTex);
    glBindTexture(GL_TEXTURE_2D, accumColorTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, screenSize.width(), screenSize.height(), 0, GL_RGB, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, accumColorTex, 0);

    // Texture for accumulated reveal
    glGenTextures(1, &accumRevealTex);
    glBindTexture(GL_TEXTURE_2D, accumRevealTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R16F, screenSize.width(), screenSize.height(), 0, GL_RED, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, accumRevealTex, 0);

    GLenum attachements[2] = {GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1};
    glDrawBuffers(2,attachements);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        qDebug() << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!";
    glBindFramebuffer(GL_FRAMEBUFFER,defaultFramebufferObject());

    glGenVertexArrays(1,&screenVAO);
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
