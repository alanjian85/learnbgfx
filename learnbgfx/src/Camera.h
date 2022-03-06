#ifndef LEARNBGFX_CAMERA_H_
#define LEARNBGFX_CAMERA_H_

#include <array>

#include <bx/math.h>

namespace learnbgfx {
    void lookAt(float* result, bx::Vec3 position, bx::Vec3 target, bx::Vec3 worldUp) {
        auto zaxis = bx::normalize(bx::sub(target, position));
        auto xaxis = bx::normalize(bx::cross(worldUp, zaxis));
        auto yaxis = bx::cross(zaxis, xaxis);

        float translation[16];
        bx::mtxIdentity(translation);
        translation[12] = -position.x;
        translation[13] = -position.y;
        translation[14] = -position.z;

        float rotation[16];
        bx::mtxIdentity(rotation);
        rotation[0] = xaxis.x;
        rotation[1] = yaxis.x;
        rotation[2] = zaxis.x;

        rotation[4] = xaxis.y;
        rotation[5] = yaxis.y;
        rotation[6] = zaxis.y;
        
        rotation[8] = xaxis.z;
        rotation[9] = yaxis.z;
        rotation[10] = zaxis.z;

        bx::mtxMul(result, translation, rotation);
    }

    enum class Orientation {
        kFront,
        kBack,
        kLeft,
        kRight
    };

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

        void getViewMatrix(float* result) const {
            lookAt(result, position, bx::add(position, direction()), up());
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

        void processKeyboard(Orientation orientation, float deltaTime) {
            switch (orientation) {
                case Orientation::kFront:
                    position = bx::add(position, bx::mul(direction(), movementSpeed * deltaTime));
                    break;
                case Orientation::kBack:
                    position = bx::sub(position, bx::mul(direction(), movementSpeed * deltaTime));
                    break;
                case Orientation::kLeft:
                    position = bx::sub(position, bx::mul(right(), movementSpeed * deltaTime));
                    break;
                case Orientation::kRight:
                    position = bx::add(position, bx::mul(right(), movementSpeed * deltaTime));
                    break;
            }
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