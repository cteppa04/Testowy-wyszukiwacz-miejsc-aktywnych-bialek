#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <QElapsedTimer>

class Camera
{
public:
    enum class Type{
        Orbit,
        Free
    };

    Camera();

    void setType(Type type);
    void set_camera_orbit_point(glm::vec3 ORBIT_CAMERA_ORBIT_POINT);

    void start(const glm::vec3 *camera_starting_position);

    glm::mat4 update(QSet<int> *keys);


    glm::mat4 m_view_matrix = glm::mat4(1.0f);
    glm::vec3 m_camera_position = glm::vec3(0.0f,0.0f,0.0f);

private:
    Type m_type{Type::Orbit};
    //basic camera settings
    const float M_ORBIT_CAMERA_MOVE_SPEED = 20.0f;
    const float M_ORBIT_CAMERA_ROTATION_SPEED = 4.0f;

    const glm::vec3 M_WORLD_X_AXIS = glm::vec3(1.0,0.0,0.0);
    const glm::vec3 M_WORLD_Y_AXIS = glm::vec3(0.0,1.0,0.0);
    const glm::vec3 M_WORLD_Z_AXIS = glm::vec3(0.0,0.0,1.0);

    glm::vec3 m_local_x_axis;
    glm::vec3 m_local_y_axis;
    glm::vec3 m_local_z_axis;

    float m_pitch = 0.0f;
    float m_yaw = 0.0f;
    float m_roll = 0.0f;
    float m_radius = 0.0f;

    //origin point camera settings
    glm::vec3 m_orbit_camera_orbit_point= glm::vec3(0.0,0.0,0.0);

    //delta time declarations
    QElapsedTimer m_timer;
    uint m_last_time;
    uint m_current_time;
    float m_delta_time;

};

#endif // CAMERA_H
