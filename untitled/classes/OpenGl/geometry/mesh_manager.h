#ifndef MESH_MANAGER_H
#define MESH_MANAGER_H

#include <classes/OpenGl/geometry/mesh.h>

class Mesh_manager
{
public:
    Mesh_manager();
    ~Mesh_manager();
    void add_mesh(const std::string name,Mesh* Mesh);
    Mesh* get_mesh(std::string name);


    // Disable copy and assignment
    Mesh_manager(const Mesh_manager&) = delete;
    Mesh_manager& operator=(const Mesh_manager&) = delete;
private:
    std::unordered_map<std::string,Mesh*> m_meshes;
};

#endif // MESH_MANAGER_H
