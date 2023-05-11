#include "Camera.h"

Camera::Camera()
    :myPosition(XMFLOAT3(0.0f,0.0f,0.0f)),
    myPositionVector(XMLoadFloat3(&myPosition)),
    myRotation(XMFLOAT3(0.0f, 0.0f, 0.0f)),
    myRotationVector(XMLoadFloat3(&myPosition))

{
    UpdateViewMatrix();
}

void Camera::SetProjectionValues(float aFovInDegrees, float anAspectRatio, float aNearZ, float aFarZ)
{
    float fovRadians = (aFovInDegrees / 360.f) * XM_2PI;
    myProjectionMatrix = XMMatrixPerspectiveFovLH(fovRadians, anAspectRatio, aNearZ, aFarZ);
}

const XMMATRIX& Camera::GetViewMatrix() const
{
    return myViewMatrix;
}

const XMMATRIX& Camera::GetProjectionMatrix() const
{
    return myProjectionMatrix;
}

const XMVECTOR& Camera::GetPositionVector() const
{
    return myPositionVector;
}

const XMFLOAT3& Camera::GetPositionFloat3() const
{
    return myPosition;
}

const XMVECTOR& Camera::GetRotationVector() const
{
    return myRotationVector;
}

const XMFLOAT3& Camera::GetRotationFloat3() const
{
    return myRotation;
}

void Camera::SetPosition(const XMVECTOR& aPos)
{
    XMStoreFloat3(&myPosition, aPos);
    myPositionVector = aPos;
    UpdateViewMatrix();
}

void Camera::SetPosition(float x, float y, float z)
{
    myPosition = XMFLOAT3(x, y, z);
    myPositionVector = XMLoadFloat3(&myPosition);
    UpdateViewMatrix();
}

void Camera::AdjustPosition(const XMVECTOR& aPos)
{
    myPositionVector += aPos;
    XMStoreFloat3(&myPosition, myPositionVector);
    UpdateViewMatrix();
}

void Camera::AdjustPosition(float x, float y, float z)
{
    myPosition.x += x;
    myPosition.y += y;
    myPosition.z += z;
    myPositionVector = XMLoadFloat3(&myPosition);
    UpdateViewMatrix();
}

void Camera::SetRotation(const XMVECTOR& aRot)
{
    XMStoreFloat3(&myRotation, aRot);
    myRotationVector = aRot;
    UpdateViewMatrix();
}

void Camera::SetRotation(float x, float y, float z)
{
    myRotation = XMFLOAT3(x, y, z);
    myRotationVector = XMLoadFloat3(&myRotation);
    UpdateViewMatrix();
}

void Camera::AdjustRotation(const XMVECTOR& aRot)
{
    myPositionVector += aRot;
    XMStoreFloat3(&myRotation, myRotationVector);
    UpdateViewMatrix();
}

void Camera::AdjustRotation(float x, float y, float z)
{
    myRotation.x += x;
    myRotation.y += y;
    myRotation.z += z;
    myRotationVector= XMLoadFloat3(&myRotation);
    UpdateViewMatrix();
}

void Camera::SetLookAtPos(XMFLOAT3 aLookAtPos)
{
    if (myPosition.x == aLookAtPos.x && myPosition.y == aLookAtPos.y && myPosition.z == aLookAtPos.z)
    {
        return;
    }

    aLookAtPos.x = myPosition.x - aLookAtPos.x;
    aLookAtPos.y = myPosition.y - aLookAtPos.y;
    aLookAtPos.z = myPosition.z - aLookAtPos.z;

    float pitch = 0.f;
    float yaw = 0.f; 
    if (aLookAtPos.y != 0.f)
    {
        const float distance = sqrt(aLookAtPos.x * aLookAtPos.x + aLookAtPos.z * aLookAtPos.z);
        pitch = atan(aLookAtPos.y / distance);
    }
    if (aLookAtPos.x != 0.f)
    {
        yaw = atan(aLookAtPos.x / aLookAtPos.z);
    }
    if (aLookAtPos.z > 0)
    {
        yaw += XM_PI;
    }
    SetRotation(pitch, yaw, 0.0f);
}

const XMVECTOR& Camera::GetForwardVector()
{
    return vectorForward;
}

const XMVECTOR& Camera::GetRightVector()
{
    return vectorRight;
}

const XMVECTOR& Camera::GetLeftVector()
{
    return vectorLeft;
}

const XMVECTOR& Camera::GetBackVector()
{
    return vectorBack;
}

void Camera::UpdateViewMatrix()
{
    XMMATRIX camRotationMatrix = XMMatrixRotationRollPitchYaw(myRotation.x, myRotation.y, myRotation.z);
    XMVECTOR camTarget = XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, camRotationMatrix);
    camTarget += myPositionVector;
    XMVECTOR upDir = XMVector3TransformCoord(DEFAULT_UP_VECTOR, camRotationMatrix);
    myViewMatrix = XMMatrixLookAtLH(myPositionVector, camTarget, upDir);

    XMMATRIX vectorRotationMatrix = XMMatrixRotationRollPitchYaw(0.0f, myRotation.y, 0.0f);
    vectorForward = XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, vectorRotationMatrix);
    vectorBack = XMVector3TransformCoord(DEFAULT_BACK_VECTOR, vectorRotationMatrix);
    vectorRight = XMVector3TransformCoord(DEFAULT_RIGHT_VECTOR, vectorRotationMatrix);
    vectorLeft = XMVector3TransformCoord(DEFAULT_LEFT_VECTOR, vectorRotationMatrix);
}
