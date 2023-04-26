#include "Camera.h"

Camera::Camera()
    :mPosition(XMFLOAT3(0.0f,0.0f,0.0f)),
    mPositionVector(XMLoadFloat3(&mPosition)),
    mRotation(XMFLOAT3(0.0f, 0.0f, 0.0f)),
    mRotationVector(XMLoadFloat3(&mPosition))

{
    UpdateViewMatrix();
}

void Camera::SetProjectionValues(float aFovInDegrees, float anAspectRatio, float aNearZ, float aFarZ)
{
    float fovRadians = (aFovInDegrees / 360.f) * XM_2PI;
    mProjectionMatrix = XMMatrixPerspectiveFovLH(fovRadians, anAspectRatio, aNearZ, aFarZ);
}

const XMMATRIX& Camera::GetViewMatrix() const
{
    return mViewMatrix;
}

const XMMATRIX& Camera::GetProjectionMatrix() const
{
    return mProjectionMatrix;
}

const XMVECTOR& Camera::GetPositionVector() const
{
    return mPositionVector;
}

const XMFLOAT3& Camera::GetPositionFloat3() const
{
    return mPosition;
}

const XMVECTOR& Camera::GetRotationVector() const
{
    return mRotationVector;
}

const XMFLOAT3& Camera::GetRotationFloat3() const
{
    return mRotation;
}

void Camera::SetPosition(const XMVECTOR& aPos)
{
    XMStoreFloat3(&mPosition, aPos);
    mPositionVector = aPos;
    UpdateViewMatrix();
}

void Camera::SetPosition(float x, float y, float z)
{
    mPosition = XMFLOAT3(x, y, z);
    mPositionVector = XMLoadFloat3(&mPosition);
    UpdateViewMatrix();
}

void Camera::AdjustPosition(const XMVECTOR& aPos)
{
    mPositionVector += aPos;
    XMStoreFloat3(&mPosition, mPositionVector);
    UpdateViewMatrix();
}

void Camera::AdjustPosition(float x, float y, float z)
{
    mPosition.x += x;
    mPosition.y += y;
    mPosition.z += z;
    mPositionVector = XMLoadFloat3(&mPosition);
    UpdateViewMatrix();
}

void Camera::SetRotation(const XMVECTOR& aRot)
{
    XMStoreFloat3(&mRotation, aRot);
    mRotationVector = aRot;
    UpdateViewMatrix();
}

void Camera::SetRotation(float x, float y, float z)
{
    mRotation = XMFLOAT3(x, y, z);
    mRotationVector = XMLoadFloat3(&mRotation);
    UpdateViewMatrix();
}

void Camera::AdjustRotation(const XMVECTOR& aRot)
{
    mPositionVector += aRot;
    XMStoreFloat3(&mRotation, mRotationVector);
    UpdateViewMatrix();
}

void Camera::AdjustRotation(float x, float y, float z)
{
    mRotation.x += x;
    mRotation.y += y;
    mRotation.z += z;
    mRotationVector= XMLoadFloat3(&mRotation);
    UpdateViewMatrix();
}

void Camera::SetLookAtPos(XMFLOAT3 aLookAtPos)
{
    if (mPosition.x == aLookAtPos.x && mPosition.y == aLookAtPos.y && mPosition.z == aLookAtPos.z)
    {
        return;
    }

    aLookAtPos.x = mPosition.x - aLookAtPos.x;
    aLookAtPos.y = mPosition.y - aLookAtPos.y;
    aLookAtPos.z = mPosition.z - aLookAtPos.z;

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
    XMMATRIX camRotationMatrix = XMMatrixRotationRollPitchYaw(mRotation.x, mRotation.y, mRotation.z);
    XMVECTOR camTarget = XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, camRotationMatrix);
    camTarget += mPositionVector;
    XMVECTOR upDir = XMVector3TransformCoord(DEFAULT_UP_VECTOR, camRotationMatrix);
    mViewMatrix = XMMatrixLookAtLH(mPositionVector, camTarget, upDir);

    XMMATRIX vectorRotationMatrix = XMMatrixRotationRollPitchYaw(0.0f, mRotation.y, 0.0f);
    vectorForward = XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, vectorRotationMatrix);
    vectorBack = XMVector3TransformCoord(DEFAULT_BACK_VECTOR, vectorRotationMatrix);
    vectorRight = XMVector3TransformCoord(DEFAULT_RIGHT_VECTOR, vectorRotationMatrix);
    vectorLeft = XMVector3TransformCoord(DEFAULT_LEFT_VECTOR, vectorRotationMatrix);
}
