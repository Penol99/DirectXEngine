#pragma once
#include <DirectXMath.h>
#include "../Ray.h"
using namespace DirectX;

class Camera
{
public:
	Camera();
	
	void SetProjectionValues(float aFovInDegrees, float anAspectRatio, float aNearZ, float aFarZ);
	const XMMATRIX& GetViewMatrix() const;
	const XMMATRIX& GetProjectionMatrix() const;

	const XMVECTOR& GetPositionVector() const;
	const XMFLOAT3& GetPositionFloat3() const;
	const XMVECTOR& GetRotationVector() const;
	const XMFLOAT3& GetRotationFloat3() const;

	void SetPosition(const XMVECTOR& aPos);
	void SetPosition(float x,float y, float z);
	void AdjustPosition(const XMVECTOR& aPos);
	void AdjustPosition(float x, float y, float z);

	void SetRotation(const XMVECTOR& aRot);
	void SetRotation(float x, float y, float z);
	void AdjustRotation(const XMVECTOR& aRot);
	void AdjustRotation(float x, float y, float z);

	void SetLookAtPos(XMFLOAT3 aLookAtPos);
	const XMVECTOR& GetForwardVector();
	const XMVECTOR& GetRightVector();
	const XMVECTOR& GetLeftVector();
	const XMVECTOR& GetBackVector();

	Ray ScreenPointToRay(float mouseX, float mouseY);
private:
	void UpdateViewMatrix();
	XMVECTOR myPositionVector;
	XMVECTOR myRotationVector;
	XMFLOAT3 myPosition;
	XMFLOAT3 myRotation;
	XMMATRIX myViewMatrix;
	XMMATRIX myProjectionMatrix;

	const XMVECTOR DEFAULT_FORWARD_VECTOR = XMVectorSet(0.f, 0.f, 1.f, 0.f);
	const XMVECTOR DEFAULT_UP_VECTOR = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	const XMVECTOR DEFAULT_BACK_VECTOR = XMVectorSet(0.f, 0.f, -1.f, 0.f);
	const XMVECTOR DEFAULT_RIGHT_VECTOR = XMVectorSet(1.f, 0.f, 0.f, 0.f);
	const XMVECTOR DEFAULT_LEFT_VECTOR = XMVectorSet(-1.f, 0.f, 0.f, 0.f);

	XMVECTOR vectorForward;
	XMVECTOR vectorRight;
	XMVECTOR vectorLeft;
	XMVECTOR vectorBack;

};