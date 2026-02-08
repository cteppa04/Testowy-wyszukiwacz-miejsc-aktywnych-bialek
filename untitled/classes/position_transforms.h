#ifndef POSITION_TRANSFORMS_H
#define POSITION_TRANSFORMS_H

#include <glm/glm.hpp>

//returns vec3(x,y,z);
// up y+
//right x+
//back z+
//pitch rotates around x
//yaw rotates around y
//pitch 0 => y = 0
//yaw 0 => z = 1, x = 0
//give radians
inline glm::vec3 euler_to_cartesian(float pitch, float yaw, float radius){
    glm::vec3 direction_rad;
    direction_rad.x = std::sin(yaw) * cos(pitch);
    direction_rad.z = std::cos(yaw) * cos(pitch);
    direction_rad.y = std::sin(pitch);
    glm::vec3 position = direction_rad * radius;
    return position;
}

//reurns yaw,pitch,radius in degrees
inline std::array<float,3> cartesian_to_euler(glm::vec3 position){
    float radius = glm::length(position);
    float yaw = glm::degrees(std::atan2(position.x,position.z));
    float pitch = glm::degrees(std::atan2(position.y,radius));
    return {yaw,pitch,radius};
}
#endif // POSITION_TRANSFORMS_H
