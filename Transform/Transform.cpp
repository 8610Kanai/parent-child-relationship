#include "Transform.hpp"

Transform::Transform() : Transform(nullptr, nullptr)
{
}

Transform::Transform(Transform * parent) : Transform(parent, nullptr)
{
}

Transform::Transform(Transform* const parent, D3DXVECTOR3* const location, Rotation* const rotation, D3DXVECTOR3* const scale)
{
    D3DXMatrixIdentity(&this->worldMatrix);

    this->parentTransform = nullptr;

    this->localMatrix = this->CreateWorldTranslationMatrix(location, rotation, scale);

    this->BecomeParents(parent);

}

Transform::~Transform()
{
    // 親から自分を除く
    if (this->parentTransform)
    {
        this->parentTransform->RemoveChild(this);
    }

    // 子供の親を解除
    for (auto&& child : this->childrenTransforms)
    {
        if (child)
        {
            child->SetParent(nullptr);
        }
    }
}



/**************************************** 親子関連 ****************************************/

Transform* Transform::GetParent() const
{
    return this->parentTransform;
}

void Transform::SetParent(Transform* const parent)
{
    if (parent) this->parentTransform = parent;
    else        this->BecomeParents(nullptr);
}

const std::vector<Transform*>& Transform::GetChildren() const
{
    return this->childrenTransforms;
}

void Transform::BecomeParents(Transform* const parent)
{
    if (parent)
    {
        parent->AddChild(this);

        this->parentTransform = parent;

        this->UpdateWorldMatrix();
        this->UpdateLocalMatrix();
    }
    else
    {
        // 今の親を解除
        if (this->parentTransform)
        {
            this->parentTransform->RemoveChild(this);
        }

        this->parentTransform = nullptr;

        this->UpdateWorldMatrix();
        this->UpdateLocalMatrix();
    }
}

bool Transform::AddChild(Transform* const child)
{
    if (child)
    {
        // 自分の先祖に子がいた
        if (this->CheckAncestor(child)) return false;

        // 前の親を解除
        Transform* prevParent = child->GetParent();
        if (prevParent)
        {
            prevParent->RemoveChild(child);
        }

        // 親子になる
        child->SetParent(this);
        this->childrenTransforms.push_back(child);

        return true;
    }

    return false;
}

bool Transform::RemoveChild(Transform* const child)
{
    if (child && !this->childrenTransforms.empty())
    {
        this->childrenTransforms.erase
        (
            std::remove
            (
                this->childrenTransforms.begin(),
                this->childrenTransforms.end(),
                child
            ),
            this->childrenTransforms.end()
        );

        return true;
    }

    return false;
}

bool Transform::CheckAncestor(Transform* const ancestor)
{
    Transform* check = this;
    do
    {
        if (check == ancestor) return true;

        check = check->GetParent();
    }
    while (check);

    return false;
}

bool Transform::HasParent()
{
    return (this->parentTransform != nullptr);
}

bool Transform::HasChild()
{
    return (this->childrenTransforms.size() != 0);
}



/**************************************** 行列 ****************************************/

const D3DXMATRIX& Transform::GetWorldMatrix() const
{
    return this->worldMatrix;
}

const D3DXMATRIX& Transform::GetLocalMatrix() const
{
    return this->localMatrix;
}

D3DXMATRIX Transform::GetWorldRotationMatrix() const
{
    D3DXMATRIX     result;
    D3DXVECTOR3    dummy;
    D3DXQUATERNION tempQuat;

    D3DXMatrixDecompose(&dummy, &tempQuat, &dummy, &this->worldMatrix);
    D3DXMatrixRotationQuaternion(&result, &tempQuat);

    return result;
}

D3DXMATRIX Transform::GetLocalRotationMatrix() const
{
    D3DXMATRIX     result;
    D3DXVECTOR3    dummy;
    D3DXQUATERNION tempQuat;

    D3DXMatrixDecompose(&dummy, &tempQuat, &dummy, &this->localMatrix);
    D3DXMatrixRotationQuaternion(&result, &tempQuat);

    return result;
}

D3DXMATRIX Transform::CreateWorldTranslationMatrix
(
        const D3DXVECTOR3* const location,
        const Rotation*    const rotation,
        const D3DXVECTOR3* const scale
)
{
    D3DXMATRIX rotationMatrix, 
               scaleMatrix,
               result;

    D3DXMatrixIdentity(&result);

    if (scale)
    {
        D3DXMatrixScaling(&scaleMatrix, scale->x, scale->y, scale->z);
        result *= scaleMatrix;
    }

    if (rotation)
    {
        D3DXMatrixRotationYawPitchRoll(&rotationMatrix, rotation->yaw, rotation->pitch, rotation->roll);
        result *= rotationMatrix;
    }
    
    if (location)
    {
        result._41 = location->x;
        result._42 = location->y;
        result._43 = location->z;
    }

    return result;
}

D3DXMATRIX Transform::CreateWorldTranslationMatrix(const D3DXVECTOR3* const location, const D3DXMATRIX* const rotationMatrix, const D3DXVECTOR3* const scale)
{
    D3DXMATRIX scaleMatrix,
               result;

    D3DXMatrixIdentity(&result);

    if (scale)
    {
        D3DXMatrixScaling(&scaleMatrix, scale->x, scale->y, scale->z);
        result *= scaleMatrix;
    }

    if (rotationMatrix && !D3DXMatrixIsIdentity(rotationMatrix))
    {
        result *= *rotationMatrix;
    }
    
    if (location)
    {
        result._41 = location->x;
        result._42 = location->y;
        result._43 = location->z;
    }

    return result;
}



/**************************************** 座標 ****************************************/

/***** world *****/

/*** get ***/

D3DXVECTOR3 Transform::GetWorldLocation() const
{
    return D3DXVECTOR3
    (
        this->worldMatrix._41,
        this->worldMatrix._42,
        this->worldMatrix._43
    );
}

/*** set ***/

void Transform::SetWorldLocation(const D3DXVECTOR3* const location)
{
    if (!location) return;

    this->worldMatrix._41 = location->x;
    this->worldMatrix._42 = location->y;
    this->worldMatrix._43 = location->z;

    this->UpdateLocalMatrix();
}

void Transform::SetWorldLocation(float x, float y, float z)
{
    this->worldMatrix._41 = x;
    this->worldMatrix._42 = y;
    this->worldMatrix._43 = z;

    this->UpdateLocalMatrix();
}

void Transform::SetWorldLocationX(float x)
{
    this->worldMatrix._41 = x;

    this->UpdateLocalMatrix();
}

void Transform::SetWorldLocationY(float y)
{
    this->worldMatrix._42 = y;

    this->UpdateLocalMatrix();
}

void Transform::SetWorldLocationZ(float z)
{
    this->worldMatrix._43 = z;

    this->UpdateLocalMatrix();
}

/*** add ***/

void Transform::AddWorldLocation(const D3DXVECTOR3* const location)
{
    this->worldMatrix._41 += location->x;
    this->worldMatrix._42 += location->y;
    this->worldMatrix._43 += location->z;

    this->UpdateLocalMatrix();
}

void Transform::AddWorldLocation(float x, float y, float z)
{
    this->worldMatrix._41 += x;
    this->worldMatrix._42 += y;
    this->worldMatrix._43 += z;

    this->UpdateLocalMatrix();
}

/***** local *****/

/*** get ***/

D3DXVECTOR3 Transform::GetLocalLocation() const
{
    return D3DXVECTOR3
    (
        this->localMatrix._41,
        this->localMatrix._42,
        this->localMatrix._43
    );
}

/*** set ***/

void Transform::SetLocalLocation(const D3DXVECTOR3* const location)
{
    this->localMatrix._41 = location->x;
    this->localMatrix._42 = location->y;
    this->localMatrix._43 = location->z;

    this->UpdateWorldMatrix();
}

void Transform::SetLocalLocation(float x, float y, float z)
{
    this->localMatrix._41 = x;
    this->localMatrix._42 = y;
    this->localMatrix._43 = z;

    this->UpdateWorldMatrix();
}

void Transform::SetLocalLocationX(float x)
{
    this->localMatrix._41 = x;

    this->UpdateWorldMatrix();
}

void Transform::SetLocalLocationY(float y)
{
    this->localMatrix._42 = y;

    this->UpdateWorldMatrix();
}

void Transform::SetLocalLocationZ(float z)
{
    this->localMatrix._43 = z;

    this->UpdateWorldMatrix();
}

/*** add ***/

void Transform::AddLocalLocation(const D3DXVECTOR3* const location)
{
    this->localMatrix._41 += location->x;
    this->localMatrix._42 += location->y;
    this->localMatrix._43 += location->z;

    this->UpdateWorldMatrix();
}

void Transform::AddLocalLocation(float x, float y, float z)
{
    this->localMatrix._41 += x;
    this->localMatrix._42 += y;
    this->localMatrix._43 += z;

    this->UpdateWorldMatrix();
}



/**************************************** 回転 ****************************************/

/***** world *****/

/*** get ***/

Rotation Transform::GetWorldRotation() const
{
    D3DXVECTOR3    dummy;
    D3DXQUATERNION tempQuat;
    D3DXMatrixDecompose(&dummy, &tempQuat, &dummy, &this->worldMatrix);
    return Rotation::QuatToRotation(&tempQuat);
}

/*** set ***/

void Transform::SetWorldRotation(const Rotation* const rotation)
{
    if (!rotation) return;

    D3DXVECTOR3 tempLocation = this->GetWorldLocation(),
                tempScale    = this->GetWorldScale();

    this->worldMatrix 
        = this->CreateWorldTranslationMatrix(&tempLocation, rotation, &tempScale); 

    this->UpdateLocalMatrix();
}

void Transform::SetWorldRotation(float yaw, float pitch, float roll)
{
    D3DXVECTOR3 tempLocation = this->GetWorldLocation(),
                tempScale    = this->GetWorldScale();
    Rotation    tempRotation(yaw, pitch, roll);

    this->worldMatrix 
        = this->CreateWorldTranslationMatrix(&tempLocation, &tempRotation, &tempScale);

    this->UpdateLocalMatrix();
}

/*** add ***/

void Transform::AddWorldRotation(const Rotation* const rotation)
{
    if (!rotation) return;

    D3DXVECTOR3 tempLocation = this->GetWorldLocation();
    D3DXMATRIX  rotationMatrix;

    D3DXMatrixRotationYawPitchRoll(&rotationMatrix, rotation->yaw, rotation->pitch, rotation->roll);

    this->worldMatrix._41 = 0.0f;
    this->worldMatrix._42 = 0.0f;
    this->worldMatrix._43 = 0.0f;

    this->worldMatrix *= rotationMatrix;

    this->worldMatrix._41 = tempLocation.x;
    this->worldMatrix._42 = tempLocation.y;
    this->worldMatrix._43 = tempLocation.z;

    this->UpdateLocalMatrix();
}

void Transform::AddWorldRotation(float yaw, float pitch, float roll)
{
    D3DXVECTOR3 tempLocation = this->GetWorldLocation();
    D3DXMATRIX  rotationMatrix;

    D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

    this->worldMatrix._41 = 0.0f;
    this->worldMatrix._42 = 0.0f;
    this->worldMatrix._43 = 0.0f;

    this->worldMatrix *= rotationMatrix;

    this->worldMatrix._41 = tempLocation.x;
    this->worldMatrix._42 = tempLocation.y;
    this->worldMatrix._43 = tempLocation.z;

    this->UpdateLocalMatrix();
}

/*** Quat ***/

void Transform::WorldRotateAroundAxis(float x, float y, float z, float w)
{
    D3DXVECTOR3    axis(x, y, z),
                   tempLocation = this->GetWorldLocation();
    D3DXQUATERNION tempQuat(0.0f,0.0f,0.0f,1.0f);
    D3DXMATRIX     rotationMatrix;

    D3DXQuaternionRotationAxis(&tempQuat, &axis, w);
    D3DXMatrixRotationQuaternion(&rotationMatrix, &tempQuat);

    this->worldMatrix._41 = 0.0f;
    this->worldMatrix._42 = 0.0f;
    this->worldMatrix._43 = 0.0f;

    this->worldMatrix *= rotationMatrix;

    this->worldMatrix._41 = tempLocation.x;
    this->worldMatrix._42 = tempLocation.y;
    this->worldMatrix._43 = tempLocation.z;

    this->UpdateLocalMatrix();
}

void Transform::WorldRotateAroundAxis(const D3DXVECTOR3* axis, float w)
{
    D3DXVECTOR3    tempLocation = this->GetWorldLocation();
    D3DXQUATERNION tempQuat(0.0f, 0.0f, 0.0f, 1.0f);
    D3DXMATRIX     rotationMatrix;

    D3DXQuaternionRotationAxis(&tempQuat, axis, w);
    D3DXMatrixRotationQuaternion(&rotationMatrix, &tempQuat);

    this->worldMatrix._41 = 0.0f;
    this->worldMatrix._42 = 0.0f;
    this->worldMatrix._43 = 0.0f;

    this->worldMatrix *= rotationMatrix;

    this->worldMatrix._41 = tempLocation.x;
    this->worldMatrix._42 = tempLocation.y;
    this->worldMatrix._43 = tempLocation.z;

    this->UpdateLocalMatrix();
}

/***** local *****/

/*** get ***/

Rotation Transform::GetLocalRotation() const
{
    D3DXVECTOR3    dummy;
    D3DXQUATERNION tempQuat;
    D3DXMatrixDecompose(&dummy, &tempQuat, &dummy, &this->localMatrix);
    return Rotation::QuatToRotation(&tempQuat);
}

/*** set ***/

void Transform::SetLocalRotation(const Rotation* const rotation)
{
    if (!rotation) return;

    D3DXVECTOR3 tempLocation = this->GetLocalLocation(),
                tempScale    = this->GetLocalScale();

    this->localMatrix
        = this->CreateWorldTranslationMatrix(&tempLocation, rotation, &tempScale);

    this->UpdateWorldMatrix();
}

void Transform::SetLocalRotation(float yaw, float pitch, float roll)
{
    D3DXVECTOR3 tempLocation = this->GetLocalLocation(),
                tempScale    = this->GetLocalScale();
    Rotation    tempRotation(yaw, pitch, roll);

    this->localMatrix 
        = this->CreateWorldTranslationMatrix(&tempLocation, &tempRotation, &tempScale);

    this->UpdateWorldMatrix();
}

/*** add ***/

void Transform::AddLocalRotation(const Rotation* const rotation)
{
    if (!rotation) return;

    D3DXVECTOR3 tempLocation = this->GetLocalLocation();
    D3DXMATRIX  rotationMatrix;

    D3DXMatrixRotationYawPitchRoll(&rotationMatrix, rotation->yaw, rotation->pitch, rotation->roll);

    this->localMatrix._41 = 0.0f;
    this->localMatrix._42 = 0.0f;
    this->localMatrix._43 = 0.0f;

    this->localMatrix *= rotationMatrix;

    this->localMatrix._41 = tempLocation.x;
    this->localMatrix._42 = tempLocation.y;
    this->localMatrix._43 = tempLocation.z;

    this->UpdateLocalMatrix();
}

void Transform::AddLocalRotation(float yaw, float pitch, float roll)
{
    D3DXVECTOR3 tempLocation = this->GetLocalLocation();
    D3DXMATRIX  rotationMatrix;

    D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

    this->localMatrix._41 = 0.0f;
    this->localMatrix._42 = 0.0f;
    this->localMatrix._43 = 0.0f;

    this->localMatrix *= rotationMatrix;

    this->localMatrix._41 = tempLocation.x;
    this->localMatrix._42 = tempLocation.y;
    this->localMatrix._43 = tempLocation.z;

    this->UpdateWorldMatrix();
}

/*** Quat ***/

void Transform::LocalRotateAroundAxis(float x, float y, float z, float w)
{
    D3DXVECTOR3    axis(x, y, z),
                   tempLocation = this->GetLocalLocation();
    D3DXQUATERNION tempQuat(0,0,0,1);
    D3DXMATRIX     rotationMatrix;

    D3DXQuaternionRotationAxis(&tempQuat, &axis, w);
    D3DXMatrixRotationQuaternion(&rotationMatrix, &tempQuat);

    this->localMatrix._41 = 0.0f;
    this->localMatrix._42 = 0.0f;
    this->localMatrix._43 = 0.0f;

    this->localMatrix *= rotationMatrix;

    this->localMatrix._41 = tempLocation.x;
    this->localMatrix._42 = tempLocation.y;
    this->localMatrix._43 = tempLocation.z;

    this->UpdateWorldMatrix();
}

void Transform::LocalRotateAroundAxis(const D3DXVECTOR3* axis, float w)
{
    D3DXVECTOR3    tempLocation = this->GetLocalLocation();
    D3DXQUATERNION tempQuat(0,0,0,1);
    D3DXMATRIX     rotationMatrix;

    D3DXQuaternionRotationAxis(&tempQuat, axis, w);
    D3DXMatrixRotationQuaternion(&rotationMatrix, &tempQuat);

    this->localMatrix._41 = 0.0f;
    this->localMatrix._42 = 0.0f;
    this->localMatrix._43 = 0.0f;

    this->localMatrix *= rotationMatrix;

    this->localMatrix._41 = tempLocation.x;
    this->localMatrix._42 = tempLocation.y;
    this->localMatrix._43 = tempLocation.z;

    this->UpdateWorldMatrix();
}



/**************************************** 拡縮 ****************************************/

/***** world *****/

/*** get ***/

D3DXVECTOR3 Transform::GetWorldScale() const
{
    D3DXVECTOR3 directions[3] =
    {
        {this->worldMatrix._11, this->worldMatrix._12, this->worldMatrix._13},
        {this->worldMatrix._21, this->worldMatrix._22, this->worldMatrix._23},
        {this->worldMatrix._31, this->worldMatrix._32, this->worldMatrix._33}
    };

    return D3DXVECTOR3
    (
        D3DXVec3Length(&directions[0]),
        D3DXVec3Length(&directions[1]),
        D3DXVec3Length(&directions[2])
    );
}

/*** set ***/

void Transform::SetWorldScale(const D3DXVECTOR3* const scale)
{
    if (!scale) return;

    D3DXVECTOR3 tempLocation   = this->GetWorldLocation();
    D3DXMATRIX  rotationMatrix = this->GetWorldRotationMatrix();

    this->worldMatrix
        = Transform::CreateWorldTranslationMatrix(&tempLocation, &rotationMatrix, scale);

    this->UpdateLocalMatrix();
}

void Transform::SetWorldScale(float x, float y, float z)
{
    D3DXVECTOR3 tempLocation   = this->GetWorldLocation(),
                tempScale      = { x, y, z };
    D3DXMATRIX  rotationMatrix = this->GetWorldRotationMatrix();

    this->worldMatrix
        = Transform::CreateWorldTranslationMatrix(&tempLocation, &rotationMatrix, &tempScale);

    this->UpdateLocalMatrix();
}

void Transform::SetWorldScaleX(float x)
{
    float mult = x / this->GetWorldScale().x;

    this->worldMatrix._11 *= mult;
    this->worldMatrix._12 *= mult;
    this->worldMatrix._13 *= mult;

    this->UpdateLocalMatrix();
}

void Transform::SetWorldScaleY(float y)
{
    float mult = y / this->GetWorldScale().y;

    this->worldMatrix._21 *= mult;
    this->worldMatrix._22 *= mult;
    this->worldMatrix._23 *= mult;

    this->UpdateLocalMatrix();
}

void Transform::SetWorldScaleZ(float z)
{
    float mult = z / this->GetWorldScale().z;

    this->worldMatrix._31 *= mult;
    this->worldMatrix._32 *= mult;
    this->worldMatrix._33 *= mult;

    this->UpdateLocalMatrix();
}

/*** add ***/

void Transform::AddWorldScale(const D3DXVECTOR3* const scale)
{
    if (!scale) return;

    D3DXVECTOR3 tempLocation   = this->GetWorldLocation(),
                tempScale      = this->GetWorldScale() + *scale;
    D3DXMATRIX  rotationMatrix = this->GetWorldRotationMatrix();

    this->worldMatrix
        = Transform::CreateWorldTranslationMatrix(&tempLocation, &rotationMatrix, &tempScale);

    this->UpdateLocalMatrix();
}

void Transform::AddWorldScale(float x, float y, float z)
{
    D3DXVECTOR3 tempLocation   = this->GetWorldLocation(),
                tempScale      = this->GetWorldScale() + D3DXVECTOR3(x, y, z);
    D3DXMATRIX  rotationMatrix = this->GetWorldRotationMatrix();

    this->worldMatrix
        = Transform::CreateWorldTranslationMatrix(&tempLocation, &rotationMatrix, &tempScale);

    this->UpdateLocalMatrix();
}

/***** local *****/

/*** get ***/

D3DXVECTOR3 Transform::GetLocalScale() const
{
    D3DXVECTOR3 directions[3] =
    {
        {this->localMatrix._11, this->localMatrix._12, this->localMatrix._13},
        {this->localMatrix._21, this->localMatrix._22, this->localMatrix._23},
        {this->localMatrix._31, this->localMatrix._32, this->localMatrix._33}
    };

    return D3DXVECTOR3
    (
        D3DXVec3Length(&directions[0]),
        D3DXVec3Length(&directions[1]),
        D3DXVec3Length(&directions[2])
    );
}

/*** set ***/

void Transform::SetLocalScale(const D3DXVECTOR3* const scale)
{
    if (!scale) return;

    D3DXVECTOR3 tempLocation   = this->GetLocalLocation();
    D3DXMATRIX  rotationMatrix = this->GetLocalRotationMatrix();

    this->localMatrix
        = Transform::CreateWorldTranslationMatrix(&tempLocation, &rotationMatrix, scale);

    this->UpdateLocalMatrix();
}

void Transform::SetLocalScale(float x, float y, float z)
{
    D3DXVECTOR3 tempLocation   = this->GetLocalLocation(),
                tempScale      = { x, y, z };
    D3DXMATRIX  rotationMatrix = this->GetLocalRotationMatrix();

    this->localMatrix
        = Transform::CreateWorldTranslationMatrix(&tempLocation, &rotationMatrix, &tempScale);

    this->UpdateWorldMatrix();
}

void Transform::SetLocalScaleX(float x)
{
    float mult = x / this->GetLocalScale().x;

    this->localMatrix._11 *= mult;
    this->localMatrix._12 *= mult;
    this->localMatrix._13 *= mult;

    this->UpdateWorldMatrix();
}

void Transform::SetLocalScaleY(float y)
{
    float mult = y / this->GetLocalScale().y;

    this->localMatrix._21 *= mult;
    this->localMatrix._22 *= mult;
    this->localMatrix._23 *= mult;

    this->UpdateWorldMatrix();
}

void Transform::SetLocalScaleZ(float z)
{
    float mult = z / this->GetLocalScale().z;

    this->localMatrix._31 *= mult;
    this->localMatrix._32 *= mult;
    this->localMatrix._33 *= mult;

    this->UpdateWorldMatrix();
}

/*** add ***/

void Transform::AddLocalScale(const D3DXVECTOR3* const scale)
{
    if (!scale) return;

    D3DXVECTOR3 tempLocation   = this->GetLocalLocation(),
                tempScale      = this->GetLocalScale() + *scale;
    D3DXMATRIX  rotationMatrix = this->GetLocalRotationMatrix();

    this->localMatrix
        = Transform::CreateWorldTranslationMatrix(&tempLocation, &rotationMatrix, &tempScale);

    this->UpdateWorldMatrix();
}

void Transform::AddLocalScale(float x, float y, float z)
{
    D3DXVECTOR3 tempLocation   = this->GetLocalLocation(),
                tempScale      = this->GetLocalScale() + D3DXVECTOR3(x, y, z);
    D3DXMATRIX  rotationMatrix = this->GetLocalRotationMatrix();

    this->localMatrix
        = Transform::CreateWorldTranslationMatrix(&tempLocation, &rotationMatrix, &tempScale);

    this->UpdateWorldMatrix();
}



/**************************************** 方向 ****************************************/

D3DXVECTOR3 Transform::GetForwardVector() const
{
    D3DXVECTOR3 result
    (
        this->worldMatrix._31,
        this->worldMatrix._32,
        this->worldMatrix._33
    );

    D3DXVec3Normalize(&result, &result);

    return result;
}

D3DXVECTOR3 Transform::GetUpVector() const
{
    D3DXVECTOR3 result
    (
        this->worldMatrix._21,
        this->worldMatrix._22,
        this->worldMatrix._23
    );

    D3DXVec3Normalize(&result, &result);

    return result;
}

D3DXVECTOR3 Transform::GetRightVector() const
{
    D3DXVECTOR3 result
    (
        this->worldMatrix._11,
        this->worldMatrix._12,
        this->worldMatrix._13
    );

    D3DXVec3Normalize(&result, &result);

    return result;
}



/**************************************** 更新 ****************************************/

void Transform::UpdateWorldMatrix()
{
    if (this->HasParent() && !D3DXMatrixIsIdentity(&this->parentTransform->GetLocalMatrix()))
    {
        this->worldMatrix = this->localMatrix * this->parentTransform->GetLocalMatrix();
    }
    else
    {
        this->worldMatrix = this->localMatrix;
    }

    // 子供も更新
    for (auto&& child : this->childrenTransforms)
    {
        child->UpdateWorldMatrix();
    }
}

void Transform::UpdateLocalMatrix()
{
    if (this->HasParent() && !D3DXMatrixIsIdentity(&this->parentTransform->GetLocalMatrix()))
    {
        D3DXMATRIX parentInverse;
        D3DXMatrixInverse(&parentInverse, nullptr, &this->parentTransform->GetLocalMatrix());

        this->localMatrix = this->worldMatrix * parentInverse;
    }
    else
    {
        this->localMatrix = this->worldMatrix;
    }

    // 子供も更新
    for (auto&& child : this->childrenTransforms)
    {
        child->UpdateWorldMatrix();
    }
}
