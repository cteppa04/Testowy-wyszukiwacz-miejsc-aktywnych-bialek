#ifndef POSITION_TRANSFORMS_H
#define POSITION_TRANSFORMS_H

#include <glm/glm.hpp>

glm::vec3 euler_to_cartesian(float pitch, float yaw, float radius){
    glm::vec3 direction_rad;
    direction_rad.x = std::sin(yaw) * cos(pitch);
    direction_rad.z = std::cos(yaw) * cos(pitch);
    direction_rad.y = std::sin(pitch);
    glm::vec3 position = direction_rad * radius;
    return position;
}

std::array<float,3> cartesian_to_euler(glm::vec3 position){
    float radius = glm::length(position);
    float yaw = glm::degrees(std::atan2(position.x,position.z));
    float pitch = glm::degrees(std::atan2(position.y,radius));
    return {yaw,pitch,radius};
}
#endif // POSITION_TRANSFORMS_H
