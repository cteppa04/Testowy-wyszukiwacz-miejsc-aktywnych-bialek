#include "mesh_manager.h"
#include <QDebug>

Mesh_manager::Mesh_manager() {}

Mesh_manager::~Mesh_manager()
{
    for(const auto& pair: m_meshes){
        delete pair.second;
    }
}

void Mesh_manager::add_mesh(const std::string name, Mesh* mesh)
{
    auto test = m_meshes.find(name);
    if(test != m_meshes.end()){
        qDebug() << "Mesh already exists. Overriding";
        delete test->second;
        test->second = mesh;
        return;
    }
    m_meshes[name] = mesh;
}

Mesh *Mesh_manager::get_mesh(std::string name)
{
    auto test = m_meshes.find(name);
    if(test == m_meshes.end()){
        qDebug() << "Mesh doesn't exists";
        return nullptr;
    }
    return test->second;
}
