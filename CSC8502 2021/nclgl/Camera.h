#pragma  once 
#include "Matrix4.h"
#include "Vector3.h"

const int waypointSize = 4;

class  Camera    {
public:

    Camera(void){
        yaw    = 0.0f;
        pitch = 0.0f;
    };

Camera(float  pitch , float yaw , Vector3  position ){
    this ->pitch     = pitch;
    this ->yaw       = yaw;
    this ->position  = position;
    bool autoCam = true;
        
        waypoints[0] = Vector3(4497,951,4107);
        waypoints[1] = Vector3(11477, 1263, 12658);
        waypoints[2] = Vector3(12570, 1840, 3198);
        waypoints[3] = Vector3(3127, 1277, 13311);

     }

~Camera(void){};

void  UpdateCamera(float dt = 1.0f);

Matrix4  BuildViewMatrix();

Vector3   GetPosition ()  const          { return  position; }
void      SetPosition(Vector3  val)    { position = val; }

float     GetYaw ()    const             { return  yaw; }
void      SetYaw(float y)               { yaw = y; }

float     GetPitch ()  const             { return  pitch;}
void      SetPitch(float p)            { pitch = p; }

protected:
    float     yaw;
    float     pitch;
    Vector3   position;    //Set to 0,0,0 by  Vector3  constructor  ;)
    bool autoCam;
    int currentWaypoint = 0;
    Vector3 waypoints[waypointSize];
    
    
};