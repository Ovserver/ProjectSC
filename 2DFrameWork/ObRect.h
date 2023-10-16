#pragma once
class ObRect: public GameObject
{
private:
    static ID3D11Buffer* fillVertexBuffer;  //꼭지점
    static ID3D11Buffer* vertexBuffer;      //꼭지점

public:
    static void CreateStaticMember();
    static void DeleteStaticMember();

public:
    ObRect();
    void Render(class Camera* cam = app.maincam)override;
};

