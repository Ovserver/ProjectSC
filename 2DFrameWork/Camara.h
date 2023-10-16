#pragma once
class Camera : public GameObject
{
private:
    Matrix              V, P, VP;     
public:
    //카메라위치
    Viewport            viewport;  
protected:
public:
    Camera();
    void          UpdateMatrix();
    void          ResizeScreen();
    const Matrix& GetVP() { return VP; };
};