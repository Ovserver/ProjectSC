#include "framework.h"

ID3D11Buffer* ObStar::fillVertexBuffer = nullptr;
ID3D11Buffer* ObStar::vertexBuffer = nullptr;

void ObStar::CreateStaticMember()
{
    VertexPC* Vertex;

    Vertex = new VertexPC[5 * 3];


    for (UINT i = 0; i < 5; i++)
    {
        Vertex[i * 3].position.x = 0.0f;
        Vertex[i * 3].position.y = 0.0f;
        //0 1 2 ... 359
        Vertex[i * 3 + 1].position.x = cosf(i *144.0f* ToRadian) * 0.5f;
        Vertex[i * 3 + 1].position.y = sinf(i * 144.0f * ToRadian) * 0.5f;
        //1 2 3 .. 360
        Vertex[i * 3 + 2].position.x = cosf((i + 1) * 144.0f * ToRadian) * 0.5f;
        Vertex[i * 3 + 2].position.y = sinf((i + 1) * 144.0f * ToRadian) * 0.5f;

        Vertex[i * 3].color.x = Vertex[i * 3].position.x + 0.5f;
        Vertex[i * 3].color.y = Vertex[i * 3].position.x + 0.5f;
        Vertex[i * 3].color.z = Vertex[i * 3].position.x + 0.5f;

        Vertex[i * 3+1].color.x = Vertex[i * 3 + 1].position.x + 0.5f;
        Vertex[i * 3 + 1].color.y = Vertex[i * 3 + 1].position.x + 0.5f;
        Vertex[i * 3 + 1].color.z = Vertex[i * 3 + 1].position.x + 0.5f;
      
        Vertex[i * 3 + 2].color.x = Vertex[i * 3 + 2].position.x + 0.5f;
        Vertex[i * 3 + 2].color.y = Vertex[i * 3 + 2].position.x + 0.5f;
        Vertex[i * 3 + 2].color.z = Vertex[i * 3 + 2].position.x + 0.5f;

    }

    //CreateVertexBuffer
    { //익명스코프
        //DESC 서술형 구조체
        D3D11_BUFFER_DESC desc;
        desc = { 0 };
        desc.Usage = D3D11_USAGE_IMMUTABLE;//버퍼를 읽고 쓰는 방법
        desc.ByteWidth = sizeof(VertexPC) *15; //버퍼 크기 (바이트)입니다.
        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;//버퍼가 파이프 라인에 바인딩되는 방법을 식별하십시오

        D3D11_SUBRESOURCE_DATA data = { 0 };
        //하위 리소스를 초기화하기위한 데이터를 지정합니다.
        data.pSysMem = Vertex;
        //초기화 데이터의 포인터.

        //버퍼 만들기
        HRESULT hr = D3D->GetDevice()->CreateBuffer(&desc, &data, &fillVertexBuffer);
        assert(SUCCEEDED(hr));
    }

    delete[] Vertex;



    Vertex = new VertexPC[6];
    //VertexCount = 5;
    Vertex[0].position.x = cosf(0)*0.5f;
    Vertex[0].position.y = sinf(0)*0.5f;
    Vertex[0].color = Color(1.0f, 1.0f, 1.0f, 1.0f);

    Vertex[1].position.x = cosf(72.0f *2.0f * ToRadian) * 0.5f;
    Vertex[1].position.y = sinf(72.0f *2.0f * ToRadian) * 0.5f;
    Vertex[1].color = Color(1.0f, 1.0f, 1.0f, 1.0f);

    Vertex[2].position.x = cosf(72.0f * 4.0f * ToRadian) * 0.5f;
    Vertex[2].position.y = sinf(72.0f * 4.0f * ToRadian) * 0.5f;
    Vertex[2].color = Color(1.0f, 1.0f, 1.0f, 1.0f);

    Vertex[3].position.x = cosf(72.0f * 1.0f * ToRadian) * 0.5f;
    Vertex[3].position.y = sinf(72.0f * 1.0f * ToRadian) * 0.5f;
    Vertex[3].color = Color(1.0f, 1.0f, 1.0f, 1.0f);

    Vertex[4].position.x = cosf(72.0f * 3.0f * ToRadian) * 0.5f;
    Vertex[4].position.y = sinf(72.0f * 3.0f * ToRadian) * 0.5f;
    Vertex[4].color = Color(1.0f, 1.0f, 1.0f, 1.0f);

    Vertex[5].position.x = cosf(0) * 0.5f;
    Vertex[5].position.y = sinf(0) * 0.5f;
    Vertex[5].color = Color(1.0f, 1.0f, 1.0f, 1.0f);

    //CreateVertexBuffer
    {
        D3D11_BUFFER_DESC desc;
        desc = { 0 };
        desc.Usage = D3D11_USAGE_IMMUTABLE;//버퍼를 읽고 쓰는 방법
        desc.ByteWidth = sizeof(VertexPC) * 6; //버퍼 크기 (바이트)입니다.
        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;//버퍼가 파이프 라인에 바인딩되는 방법을 식별하십시오

        D3D11_SUBRESOURCE_DATA data = { 0 };
        //하위 리소스를 초기화하기위한 데이터를 지정합니다.
        data.pSysMem = Vertex;
        //초기화 데이터의 포인터.

        //버퍼 만들기
        HRESULT hr = D3D->GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer);
        assert(SUCCEEDED(hr));
    }
    delete[] Vertex;
}

void ObStar::DeleteStaticMember()
{
    vertexBuffer->Release();
    fillVertexBuffer->Release();
}

ObStar::ObStar()
{

}

void ObStar::Render(Camera* cam)
{
    if (!isVisible)return;
    GameObject::Render(cam);

    basicShader->Set();

    UINT stride = sizeof(VertexPC);
    UINT offset = 0;

    if (isFilled)
    {
        D3D->GetDC()->IASetVertexBuffers(0,
            1,
            &fillVertexBuffer,
            &stride,
            &offset);
        D3D->GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        D3D->GetDC()->Draw(15, 0);
    }
    else
    {
        D3D->GetDC()->IASetVertexBuffers(0, //입력슬롯(16~32개까지 설정가능)
            1,//입력슬롯에 붙이고자 하는 버퍼의 갯수
            &vertexBuffer,
            &stride,//정점버퍼의 한 원소의 바이트단위 크기
            &offset);
        D3D->GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
        D3D->GetDC()->Draw(6, 0);
    }
}

