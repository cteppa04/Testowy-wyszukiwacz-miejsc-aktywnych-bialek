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

    void Camera::set_camera_position(glm::vec3 new_position)
    {
        m_camera_position = new_position;
    }

    void Camera::set_camera_direction(glm::vec3 look_at_position)
    {
        m_camera_forward = glm::normalize(look_at_position - m_camera_position);
    }


    void Camera::start()
    {
        //start delta time measurements
        m_timer.start();
        m_last_time = m_timer.elapsed();
    }

    glm::mat4 Camera::update(QSet<int> *keys)
    {
        //handle delta time calculation
        m_current_time = m_timer.elapsed();
        m_delta_time = (m_current_time - m_last_time) / 1000.0f;

        switch (m_type) {
        case Type::Orbit:{
        //     //origin point camera control
        //     //radius modification
        //     if (keys->contains(Qt::Key_W)){
        //         m_radius -= M_ORBIT_CAMERA_MOVE_SPEED * m_delta_time;
        //     }
        //     if (keys->contains(Qt::Key_S)){
        //         m_radius += M_ORBIT_CAMERA_MOVE_SPEED * m_delta_time;
        //     }
        //     //make sure the radius is never smaller than 0;
        //     m_radius = std::max(0.1f,m_radius);

        //     //pitch modification
        //     if (keys->contains(Qt::Key_D)){
        //         m_orbit_yaw+= M_ORBIT_CAMERA_M_FREE_CAMERA_ROTATION_SPEED * m_delta_time;
        //     }
        //     if (keys->contains(Qt::Key_A)){
        //         m_orbit_yaw -= M_ORBIT_CAMERA_M_FREE_CAMERA_ROTATION_SPEED * m_delta_time;
        //     }

        //     //yaw modification
        //     if (keys->contains(Qt::Key_E)){
        //         m_orbit_pitch += M_ORBIT_CAMERA_M_FREE_CAMERA_ROTATION_SPEED * m_delta_time;
        //     }
        //     if (keys->contains(Qt::Key_Q)){
        //         m_orbit_pitch -= M_ORBIT_CAMERA_M_FREE_CAMERA_ROTATION_SPEED * m_delta_time;
        //     }

        //     //make sure the pitch angle [up/down] does not exceed 90 degrees
        //     m_orbit_pitch = glm::clamp(m_orbit_pitch,glm::radians(-89.0f),glm::radians(89.0f));

        //     //set camera position and look direction
        //     set_camera_position(m_orbit_pitch,m_orbit_yaw,m_radius);
        //     set_camera_direction(m_orbit_camera_orbit_point);

        //     qDebug() << "Pitch :" << m_orbit_pitch;
        //     qDebug() << "Yaw: " << m_orbit_yaw;
        //     qDebug() << "Radius" << m_radius;
            break;
        }
        case Type::Free:{
            //rotation
            float speed = M_FREE_CAMERA_ROTATION_SPEED * m_delta_time;
            if (keys->contains(Qt::Key_Left)) m_camera_yaw += speed; // rotate left
            if (keys->contains(Qt::Key_Right)) m_camera_yaw -= speed; // rotate right
            if (keys->contains(Qt::Key_Up)) m_camera_pitch += speed; // look u
            if (keys->contains(Qt::Key_Down)) m_camera_pitch -= speed; // look down

            m_camera_pitch = glm::clamp(
                m_camera_pitch,
                glm::radians(-89.0f),
                glm::radians(89.0f)
                );

            m_camera_forward.x = cos(m_camera_pitch) * sin(m_camera_yaw);
            m_camera_forward.y = sin(m_camera_pitch);
            m_camera_forward.z = cos(m_camera_pitch) * cos(m_camera_yaw);
            m_camera_forward = glm::normalize(m_camera_forward);

            glm::vec3 right = glm::normalize(glm::cross(m_camera_forward,M_WORLD_Y_AXIS));
            glm::vec3 up = glm::normalize(glm::cross(right,m_camera_forward));
            speed = M_FREE_CAMERA_MOVE_SPEED * m_delta_time;

            if(keys->contains(Qt::Key_W)) m_camera_position += m_camera_forward * speed;
            if(keys->contains(Qt::Key_S)) m_camera_position -= m_camera_forward * speed;
            if(keys->contains(Qt::Key_D)) m_camera_position += right * speed;
            if(keys->contains(Qt::Key_A)) m_camera_position -= right * speed;
            if(keys->contains(Qt::Key_E)) m_camera_position += up * speed;
            if(keys->contains(Qt::Key_Q)) m_camera_position -= up * speed;

            break;
        }

        default:
            break;
        }

        m_last_time = m_timer.elapsed();
        return create_view_matrix();
    }

    glm::mat4 Camera::create_view_matrix()
    {
        glm::mat4 view(1.0f);
        qDebug() << "Camera pos x : " << m_camera_position.x;
        qDebug() << "Camera pos y : " << m_camera_position.y;
        qDebug() << "Camera pos z : " << m_camera_position.z;
        view = glm::lookAt(m_camera_position,m_camera_position + m_camera_forward,M_WORLD_Y_AXIS);
        return view;
    }

