#include "framework.h"

Camera::Camera()
{
    //첫 카메라 위치
    SetWorldPosX(0.0f);
    SetWorldPosY(0.0f);
    //뷰포트
    viewport.width = app.GetWidth();
    viewport.height = app.GetHeight();
    D3D->GetDC()->RSSetViewports(1, viewport.Get11());
    scale.x = 1.0f;
    scale.y = 1.0f;
}

void Camera::UpdateMatrix()
{
    D3D->GetDC()->RSSetViewports(1, viewport.Get11());

    if (update) return;
    GameObject::UpdateMatrix();
    //뷰행렬 역이동행렬
    V = Matrix::CreateTranslation(-GetWorldPos().x, -GetWorldPos().y, 0.0f);
    //프로젝션행렬 투영행렬
    P = Matrix::CreateOrthographic(app.GetWidth()/ scale.x, app.GetHeight()/ scale.y, 0.0f, 10.0f);

    VP = V * P;
    update = true;

   
}

void Camera::ResizeScreen()
{
    //viewport.x = app.GetWidth() / 2.0f;
    //viewport.width = app.GetWidth()/2.0f;
    //viewport.height = app.GetHeight()/2.0f;
    D3D->GetDC()->RSSetViewports(1, viewport.Get11());
}
