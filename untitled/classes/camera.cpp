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

void Camera::set_camera_orbit_point(glm::vec3 orbit_camera_orbit_point)
{
    m_orbit_camera_orbit_point = orbit_camera_orbit_point;
}

void Camera::start(const glm::vec3 *camera_starting_position)
{
    //start delta time measurements
    m_timer.start();
    m_last_time = m_timer.elapsed();

    auto euler = POSITION_TRANSFORMS_H::cartesian_to_euler(*camera_starting_position);
    m_yaw = euler[0];
    m_pitch = euler[1];
    m_radius = euler[2];
}

    glm::mat4 Camera::update(QSet<int> *keys)
{
    //handle delta time calculation
    m_current_time = m_timer.elapsed();
    m_delta_time = (m_current_time - m_last_time) / 1000.0f;

    switch (m_type) {
    case Type::Orbit:{
        //origin point camera control
        //radius modification
        if (keys->contains(Qt::Key_W)){
            m_radius -= M_ORBIT_CAMERA_MOVE_SPEED * m_delta_time;
        }
        if (keys->contains(Qt::Key_S)){
            m_radius += M_ORBIT_CAMERA_MOVE_SPEED * m_delta_time;
        }
        //make sure the radius is never smaller than 0;
        m_radius = std::max(0.1f,m_radius);
        //pitch modification
        if (keys->contains(Qt::Key_D)){
            m_yaw+= M_ORBIT_CAMERA_ROTATION_SPEED * m_delta_time;
        }
        if (keys->contains(Qt::Key_A)){
            m_yaw -= M_ORBIT_CAMERA_ROTATION_SPEED * m_delta_time;
        }

        //yaw modification
        if (keys->contains(Qt::Key_E)){
            m_pitch += M_ORBIT_CAMERA_ROTATION_SPEED * m_delta_time;
        }
        if (keys->contains(Qt::Key_Q)){
            m_pitch -= M_ORBIT_CAMERA_ROTATION_SPEED * m_delta_time;
        }
        m_last_time = m_timer.elapsed();
        //make sure the pitch angle [up/down] does not exceed 90 degrees
        m_pitch = glm::clamp(m_pitch,glm::radians(-89.0f),glm::radians(89.0f));
        m_camera_position = euler_to_cartesian(m_pitch,m_yaw,m_radius);
        qDebug() << "Orbit position: " << m_orbit_camera_orbit_point.x << " " << m_orbit_camera_orbit_point.y << " " << m_orbit_camera_orbit_point.z;
        qDebug() << "Camera position: " << m_camera_position.x << " " << m_camera_position.y << " " << m_camera_position.z;
        qDebug() << "Yaw " << m_yaw << " pitch: " << m_pitch << " radius: " << m_radius;

        //rebuild view so the changes dont accumulate
        m_view_matrix = glm::mat4(1.0f);

        //handle looking at the origin_point
        glm::vec3 look_at_direction = glm::normalize(m_orbit_camera_orbit_point - m_camera_position);
        m_local_x_axis = glm::normalize(glm::cross(look_at_direction,M_WORLD_Y_AXIS));

        m_view_matrix = glm::rotate(m_view_matrix,-m_yaw,M_WORLD_Y_AXIS);
        m_view_matrix = glm::rotate(m_view_matrix,m_pitch,m_local_x_axis);
        //handle translation
        m_view_matrix = glm::translate(m_view_matrix,-m_camera_position);
        break;
    }
    case Type::Free:
        break;

    default:
        break;
    }

    return m_view_matrix;
}

