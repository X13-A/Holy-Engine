#ifndef CREATIVE_CONTROLS_H
#define CREATIVE_CONTROLS_H

#include "../controls/CameraControls.hpp"
#include "../input/InputManager.hpp"
#include "../math/Vec3.hpp"
#include "../math/Math.hpp"
#include "../utils/Time.hpp"

class CreativeControls : public CameraControls
{
    private:
        float moveSpeed;
        float rotateSpeed;
        float yaw;
        float pitch;

    public:
        CreativeControls(Camera* camera, float moveSpeed, float rotateSpeed);

        void handleMouseMove(float xoffset, float yoffset) override;

        void handleScroll(float xoffset, float yoffset);

        void update(InputManager* inputManager) override;
};

#endif