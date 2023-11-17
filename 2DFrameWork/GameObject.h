#pragma once
class ObLine;

//LocalPos : 내가 원점으로 삼는곳에서 멀어진 상대 좌표
//WorldPos : 0,0 에서 멀어진 절대 좌표

//Get : 좌표 받아올때
//Set : 좌표 대입할때
//Move : += 좌표에 벡터를 더해줄때

enum class COLLIDER
{
	NONE,
	RECT,
	CIRCLE
};

class GameObject
{
	//static member
private:
	static ObLine*			axisObject;
	static ID3D11Buffer*	WVPBuffer;
	static ID3D11Buffer*	colorBuffer;

protected:
	static Shader*			basicShader;
	static Shader*			imageShader;
	static Shader*			tileMapShader;

	//static member function
public:
	static void CreateStaticMember();
	static void DeleteStaticMember();

	//member
private:
	Matrix		S, R, T, R2, RT, Pi, WVP;
	Matrix*		P;
	Vector2		position;
	
protected:
	bool		update;
	Vector2		pivot;
	Vector2		scale;
	Vector3		rotation;
	Vector3		rotation2;
	Matrix		W;

public:
	bool		hasAxis;
	bool        isVisible;
	bool        isFilled;
	Color       color;
	COLLIDER	collider;

public:
	GameObject();
	virtual ~GameObject() {};
	virtual void Render(class Camera* cam = app.maincam);

	virtual void UpdateMatrix();

	bool Intersect(Vector2 coord);
	bool Intersect(GameObject* ob);
	bool Intersect(GameObject* ob,int& result);

	//getter setter
public:


	//값 대입 불가
	Vector3 GetRotation() { return rotation; }
	Vector3 GetRotation2() { return rotation2; }
	Vector2 GetScale() { return scale; }
	Vector2 GetPivot() { return pivot; }
	//값 대입 가능
	Vector3& SetRotation() { update = false; return rotation; }
	Vector3& SetRotation2() { update = false; return rotation2; }
	Vector2& SetScale() { update = false; return scale; }
	Vector2& SetPivot() { update = false; return pivot; }


	//위치 받아오기
	Vector2 GetLocalPos()	{ return position;}
	Vector2 GetWorldPos()	{ UpdateMatrix(); return Vector2(RT._41, RT._42);}
	Vector2 GetWorldPivot()	{ UpdateMatrix(); return Vector2(W._41, W._42);}
	//위치를 고정할때 =
	void	SetLocalPos(Vector2 location) { update = false; position = location; }
	void	SetLocalPosX(float lcationX)	{ update = false; position.x = lcationX; }
	void	SetLocalPosY(float lcationY)	{ update = false; position.y = lcationY; }
	void	SetWorldPos(Vector2 worldPos);
	void	SetWorldPosX(float worldPosX);
	void	SetWorldPosY(float worldPosY);
	//이동시킬때 +=
	void	MoveLocalPos(Vector2 Velocity)	{ update = false; position += Velocity; }
	void	MoveWorldPos(Vector2 Velocity);
	//월드기준 방향
	Vector2 GetRight()	{ UpdateMatrix(); return Vector2(RT._11, RT._12); }
	Vector2 GetUp()		{ UpdateMatrix(); return Vector2(RT._21, RT._22); }
	//부모행렬 만들기
	void	SetParentRT(GameObject& src)	{ update = false;  P = &src.RT; }
	void	SetParentT(GameObject& src)	{ update = false;  P = &src.T; }
};

