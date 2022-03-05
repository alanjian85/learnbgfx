#ifndef LEARNBGFX_CAMERA_H_
#define LEARNBGFX_CAMERA_H_

#include <array>

#include <bx/math.h>

namespace learnbgfx {
    struct Camera {
        bx::Vec3 position;
        bx::Vec3 worldUp;

        float yaw;
        float pitch;

        float movementSpeed;
        float mouseSensitivity;
        float zoom;

        Camera()
            : position(bx::Vec3(0.0f, 0.0f, 0.0f)),
              worldUp(bx::Vec3(0.0f, 1.0f, 0.0f))
        {
            yaw = 90.0f;
            pitch = 0.0f;

            movementSpeed = 2.5f;
            mouseSensitivity = 0.1f;
            zoom = 45.0f;
        }

        std::array<float, 16> getViewMatrix() const {
            std::array<float, 16> result;
            bx::mtxLookAt(result.data(), position, bx::add(position, direction()), up());
            return result;
        }

        bx::Vec3 direction() const {
            bx::Vec3 result(0.0f);
            result.x = bx::cos(bx::toRad(yaw)) * bx::cos(bx::toRad(pitch));
            result.y = bx::sin(bx::toRad(pitch));
            result.z = bx::sin(bx::toRad(yaw)) * bx::cos(bx::toRad(pitch));
            return result;
        }

        bx::Vec3 right() const {
            return bx::normalize(bx::cross(worldUp, direction()));
        }

        bx::Vec3 up() const {
            return bx::normalize(bx::cross(direction(), right()));
        }

        void processKeyboard(bool w, bool a, bool s, bool d, float deltaTime) {
            if (w)
                position = bx::add(position, bx::mul(direction(), movementSpeed * deltaTime));
            if (s)
                position = bx::sub(position, bx::mul(direction(), movementSpeed * deltaTime));
            if (a)
                position = bx::sub(position, bx::mul(right(), movementSpeed * deltaTime));
            if (d)
                position = bx::add(position, bx::mul(right(), movementSpeed * deltaTime));
            position.y = 0.0f;
        }

        void processMouseMovement(float xoffset, float yoffset) {
            xoffset *= mouseSensitivity;
            yoffset *= mouseSensitivity;

            yaw -= xoffset;
            pitch += yoffset;

            if (pitch > 89.0f)
                pitch = 89.0f;
            if (pitch < -89.0f)
                pitch = -89.0f;
        }

        void processMouseScroll(float yoffset) {
            zoom -= yoffset;
            if (zoom < 1.0f)
                zoom = 1.0f;
            if (zoom > 45.0f)
                zoom = 45.0f;
        }
    };
}

#endif