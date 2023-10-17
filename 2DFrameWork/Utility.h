#pragma once

namespace Utility
{
    //충돌타입
    struct RECT
    {
        Vector2 min;
        Vector2 max;
        RECT(Vector2 pivot, Vector2 scale);
        RECT(GameObject* ob);
    };
    struct CIRCLE
    {
        Vector2 pivot;
        float   radius;
        CIRCLE(Vector2 pivot, Vector2 scale);
        CIRCLE(GameObject* ob);
    };

    //사각형 원 과 좌표
    bool IntersectRectCoord(RECT & rc, Vector2 coord);
    bool IntersectCircleCoord(CIRCLE & cc, Vector2 coord);

    //네모 네모
    bool IntersectRectRect(RECT & rc1, RECT & rc2);
    //OBB
    bool IntersectRectRect(GameObject * ob1, GameObject * ob2);

    //원 원
    bool IntersectCircleCircle(CIRCLE & cc1, CIRCLE & cc2);
    //네모 원
    bool IntersectRectCircle(RECT & rc, CIRCLE & cc);

    bool IntersectRectCircle(RECT& rc, CIRCLE& cc,int& result);

    float DirToRadian(Vector2 Dir);

    Vector2 WorldToScreen(Vector2 wol);

    //템플릿 함수는 선언,정의 분리 불가
    template<typename T>
    T Saturate(T src, T min = 0, T max = 1)
    {
        if (src < min)
        {
            return min;
        }
        else if (src > max)
        {
            return max;
        }
        return src;
    }

    template<typename T>
    void Swap(T & a, T & b)
    {
        T temp = a;
        a = b;
        b = temp;
    }

    void Replace(string* str, string comp, string rep);

    template<typename T>
    T Lerp(T& a, T& b , float w)
    {
        return a + (b - a) * w;
    }
};

