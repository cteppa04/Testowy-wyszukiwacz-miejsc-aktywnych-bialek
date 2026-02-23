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
    void set_camera_position(glm::vec3 new_position);
    void set_camera_position(float pitch, float yaw, float radius);
    void set_camera_direction(glm::vec3 look_at_position);

    void start();

    glm::mat4 update(QSet<int> *keys);

private:
    Type m_type{Type::Orbit};

    glm::mat4 create_view_matrix();

    //basic camera settings
    const float M_ORBIT_CAMERA_MOVE_SPEED = 20.0f;
    const float M_ORBIT_CAMERA_ROTATION_SPEED = 4.0f;

    const float M_FREE_CAMERA_MOVE_SPEED = 10.0f;
    const float M_FREE_CAMERA_ROTATION_SPEED = 2.0f;

    const glm::vec3 M_WORLD_X_AXIS = glm::vec3(1.0,0.0,0.0);
    const glm::vec3 M_WORLD_Y_AXIS = glm::vec3(0.0,1.0,0.0);
    const glm::vec3 M_WORLD_Z_AXIS = glm::vec3(0.0,0.0,1.0);

    glm::vec3 m_camera_position;
    glm::vec3 m_camera_forward = M_WORLD_Z_AXIS;
    float m_camera_yaw = 0.0;
    float m_camera_pitch = 0.0;

    //origin point camera settings
    glm::vec3 m_orbit_camera_orbit_point = glm::vec3(0.0,0.0,0.0);

    //delta time declarations
    QElapsedTimer m_timer;
    uint m_last_time;
    uint m_current_time;
    float m_delta_time;

};

#endif // CAMERA_H
