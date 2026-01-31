#include "camera.h"
#include "classes/position_transforms.h"
#include <glm/gtc/matrix_transform.hpp>
#include <QKeyEvent>

Camera::Camera() {

}

void Camera::setType(Type type)
{
    m_type = type;
}

void Camera::start(glm::vec3 *camera_starting_position)
{
    //start delta time measurements
    m_timer.start();
    m_last_time = m_timer.elapsed();

    auto euler = POSITION_TRANSFORMS_H::cartesian_to_euler(*camera_starting_position);
    m_yaw = euler[0];
    m_pitch = euler[1];
    m_radius = euler[2];
}

void Camera::update(const bool *keys)
{
    //handle delta time calculation
    m_current_time = m_timer.elapsed();
    m_delta_time = (m_current_time - m_last_time) / 1000.0f;

    switch (m_type) {
    case Type::Orbit:{
        //origin point camera control
        //radius modification
        if (keys[Qt::Key_W]){
            m_radius -= ORBIT_CAMERA_MOVE_SPEED * m_delta_time;
        }
        if (keys[Qt::Key_S]){
            m_radius += ORBIT_CAMERA_MOVE_SPEED * m_delta_time;
        }
        //make sure the radius is never smaller than 0;
        m_radius = std::max(0.1f,m_radius);
        //pitch modification
        if (keys[Qt::Key_D]){
            m_yaw+= ORBIT_CAMERA_ROTATION_SPEED * m_delta_time;
        }
        if (keys[Qt::Key_A]){
            m_yaw -= ORBIT_CAMERA_ROTATION_SPEED * m_delta_time;
        }

        //yaw modification
        if (keys[Qt::Key_E]){
            m_pitch += ORBIT_CAMERA_ROTATION_SPEED * m_delta_time;
        }
        if (keys[Qt::Key_Q]){
            m_pitch -= ORBIT_CAMERA_ROTATION_SPEED * m_delta_time;
        }
        m_last_time = m_timer.elapsed();
        //make sure the pitch angle [up/down] does not exceed 90 degrees
        m_pitch = glm::clamp(m_pitch,glm::radians(-89.0f),glm::radians(89.0f));
        m_camera_position = euler_to_cartesian(m_pitch,m_yaw,m_radius);

        //rebuild view so the changes dont accumulate
        m_view_matrix = glm::mat4(1.0f);

        //handle looking at the origin_point
        glm::vec3 look_at_direction = glm::normalize(ORBIT_CAMERA_ORBIT_POINT - m_camera_position);
        qDebug() << "look at direction: " << look_at_direction.x << " " <<  look_at_direction.y << " " << look_at_direction.z;
        m_local_x_axis = glm::normalize(glm::cross(look_at_direction,WORLD_Y_AXIS));

        m_view_matrix = glm::rotate(m_view_matrix,-m_yaw,WORLD_Y_AXIS);
        m_view_matrix = glm::rotate(m_view_matrix,m_pitch,m_local_x_axis);

        break;
    }
    case Type::Free:
        break;

    default:
        break;
    }
}

