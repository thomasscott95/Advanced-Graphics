#include "Camera.h"
#include "Window.h"
#include  <algorithm >



void  Camera :: UpdateCamera(float dt) {
    pitch -= (Window :: GetMouse()-> GetRelativePosition ().y);
    yaw -= (Window :: GetMouse()-> GetRelativePosition ().x);
    
    pitch = std::min(pitch , 90.0f);     
    pitch = std::max(pitch ,-90.0f);     

    if(yaw < 0){
    yaw +=  360.0f;
    }
    if(yaw > 360.0f){
    yaw  -= 360.0f;
    }
    
    Matrix4  rotation = Matrix4::Rotation(yaw, Vector3(0, 1, 0)); 

    Vector3  forward = rotation * Vector3(0, 0, -1); 
    Vector3  right = rotation * Vector3(1, 0, 0); 

    float  speed = 1050.0f * dt;

    if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_C)) {
        autoCam = !autoCam;
    }

    if (autoCam == false) {
        if (Window::GetKeyboard()->KeyDown(KEYBOARD_W)) {
            position += forward * speed;
        }
        if (Window::GetKeyboard()->KeyDown(KEYBOARD_S)) {
            position -= forward * speed;
        }
        if (Window::GetKeyboard()->KeyDown(KEYBOARD_A)) {
            position -= right * speed;
        }
        if (Window::GetKeyboard()->KeyDown(KEYBOARD_D)) {
            position += right * speed;
        }
        if (Window::GetKeyboard()->KeyDown(KEYBOARD_SHIFT)) {
            position.y += speed;
        }
        if (Window::GetKeyboard()->KeyDown(KEYBOARD_SPACE)) {
            position.y -= speed;
        }
    }
    else {
        Vector3 direction = waypoints[currentWaypoint] - position;
        if (direction.Length() < 5) currentWaypoint = (currentWaypoint + 1) % waypointSize;
        direction.Normalise();
        position += direction * 500 * dt;
    }

    std::cout << "Y =" << position.y << std::endl;
    std::cout << "Z =" << position.z << std::endl;
    std::cout << "X =" << position.x << std::endl;
}

Matrix4  Camera::BuildViewMatrix() { 
    return  Matrix4::Rotation(-pitch, Vector3(1, 0, 0)) * 
            Matrix4::Rotation(-yaw, Vector3(0, 1, 0)) * 
            Matrix4::Translation(-position); 
};

