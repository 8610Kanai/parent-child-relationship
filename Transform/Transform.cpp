#include "Transform.hpp"

Transform::Transform() : Transform(nullptr, nullptr) // 委譲
{
}

Transform::Transform(Transform* parent) : Transform(parent, nullptr) // 委譲
{
}

Transform::Transform(Transform* const parent, D3DXVECTOR3* const location, Rotation* const rotation, D3DXVECTOR3* const scale)
{
    this->parentTransform = nullptr;

    this->localMatrix = this->CreateWorldTranslationMatrix(location, rotation, scale);

    if (parent)
    {
        this->worldMatrix = this->localMatrix * parent->GetWorldMatrix();
        this->BecomeParents(parent);
    }
    else
    {
        this->UpdateWorldMatrix();
    }
}

Transform::Transform(Transform* const parent, const D3DXMATRIX* const localMatrix)
{
    this->parentTransform = nullptr;

    if (localMatrix) this->localMatrix = *localMatrix;
    else             D3DXMatrixIdentity(&this->localMatrix);

    if (parent)
    {
        this->worldMatrix = this->localMatrix * parent->GetWorldMatrix();
        this->BecomeParents(parent);
    }
    else
    {
        this->UpdateWorldMatrix();
    }
}

Transform::~Transform()
{
    //OutputDebugFormat("{} called.", __func__);
}

/**************************************** 親子関連 ****************************************/

Transform* Transform::GetParent() const
{
    return this->parentTransform;
}

void Transform::SetParent(Transform* const parent)
{
    if (parent) this->parentTransform = parent;
    // 引数nullの時は親子解除
    else        this->BreakParents();
}

const std::vector<Transform*>& Transform::GetChildren() const
{
    return this->childrenTransforms;
}

void Transform::BecomeParents(Transform* const parent)
{
    if (parent)
    {
        if (parent->AddChild(this))
        {
            // 親の変更による行列更新
            this->UpdateLocalMatrix();
            this->UpdateWorldMatrix();
        }
    }
    else
    {
        this->BreakParents();
    }
}

void Transform::BreakParents()
{
    if (!this->parentTransform) return;

    // 今の親を解除
    this->parentTransform->RemoveChild(this);

    this->parentTransform = nullptr;

    this->UpdateLocalMatrix(false);
    this->UpdateWorldMatrix();
}

bool Transform::AddChild(Transform* const child)
{
    if (!child) return false;

    // 自分の先祖に子がいた
    if (this->CheckAncestor(child)) return false;

    // 前の親を解除
    child->BreakParents();

    // 親子になる
    child->SetParent(this);
    this->childrenTransforms.push_back(child);

    return true;
}

bool Transform::RemoveChild(Transform* const child)
{
    if (child && this->HasChild())
    {
        // erase_remove で処理高速化
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

void Transform::SetWorldMatrix(const D3DXMATRIX* const worldMatrix)
{
    if (worldMatrix) this->worldMatrix = *worldMatrix;
    else D3DXMatrixIdentity(&this->worldMatrix);

    this->UpdateLocalMatrix();
}

const D3DXMATRIX& Transform::GetLocalMatrix() const
{
    return this->localMatrix;
}

void Transform::SetLocalMatrix(const D3DXMATRIX* const localMatrix)
{
    if (localMatrix) this->localMatrix = *localMatrix;
    else D3DXMatrixIdentity(&this->localMatrix);

    this->UpdateWorldMatrix();
}

D3DXMATRIX Transform::GetWorldRotationMatrix() const
{
    D3DXMATRIX     result = {};
    D3DXVECTOR3    dummy = {};
    D3DXQUATERNION tempQuat = {};

    // ワールド行列からクォータニオンを抽出
    D3DXMatrixDecompose(&dummy, &tempQuat, &dummy, &this->worldMatrix);

    // クォータニオンから回転行列を作成
    D3DXMatrixRotationQuaternion(&result, &tempQuat);

    return result;
}

D3DXMATRIX Transform::GetLocalRotationMatrix() const
{
    D3DXMATRIX     result = {};
    D3DXVECTOR3    dummy = {};
    D3DXQUATERNION tempQuat = {};

    // ローカル行列からクォータニオンを抽出
    D3DXMatrixDecompose(&dummy, &tempQuat, &dummy, &this->localMatrix);

    // クォータニオンから回転行列を作成
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
    D3DXMATRIX scaleMatrix, result;

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

void Transform::SetWorldLocation(const D3DXVECTOR3* const location, bool bLocalUpdate)
{
    if (!location) return;

    this->worldMatrix._41 = location->x;
    this->worldMatrix._42 = location->y;
    this->worldMatrix._43 = location->z;

    if(bLocalUpdate) this->UpdateLocalMatrix();
}

void Transform::SetWorldLocation(float x, float y, float z, bool bLocalUpdate)
{
    this->worldMatrix._41 = x;
    this->worldMatrix._42 = y;
    this->worldMatrix._43 = z;

    if(bLocalUpdate) this->UpdateLocalMatrix();
}

void Transform::SetWorldLocationX(float x, bool bLocalUpdate)
{
    this->worldMatrix._41 = x;

    if(bLocalUpdate) this->UpdateLocalMatrix();
}

void Transform::SetWorldLocationY(float y, bool bLocalUpdate)
{
    this->worldMatrix._42 = y;

    if(bLocalUpdate) this->UpdateLocalMatrix();
}

void Transform::SetWorldLocationZ(float z, bool bLocalUpdate)
{
    this->worldMatrix._43 = z;

    if(bLocalUpdate) this->UpdateLocalMatrix();
}

/*** add ***/

void Transform::AddWorldLocation(const D3DXVECTOR3* const location, bool bLocalUpdate)
{
    this->worldMatrix._41 += location->x;
    this->worldMatrix._42 += location->y;
    this->worldMatrix._43 += location->z;

    if(bLocalUpdate) this->UpdateLocalMatrix();
}

void Transform::AddWorldLocation(float x, float y, float z, bool bLocalUpdate)
{
    this->worldMatrix._41 += x;
    this->worldMatrix._42 += y;
    this->worldMatrix._43 += z;

    if(bLocalUpdate) this->UpdateLocalMatrix();
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

void Transform::SetLocalLocation(const D3DXVECTOR3* const location, bool bWorldUpdate)
{
    this->localMatrix._41 = location->x;
    this->localMatrix._42 = location->y;
    this->localMatrix._43 = location->z;

    if(bWorldUpdate) this->UpdateWorldMatrix();
}

void Transform::SetLocalLocation(float x, float y, float z, bool bWorldUpdate)
{
    this->localMatrix._41 = x;
    this->localMatrix._42 = y;
    this->localMatrix._43 = z;

    if(bWorldUpdate) this->UpdateWorldMatrix();
}

void Transform::SetLocalLocationX(float x, bool bWorldUpdate)
{
    this->localMatrix._41 = x;

    if(bWorldUpdate) this->UpdateWorldMatrix();
}

void Transform::SetLocalLocationY(float y, bool bWorldUpdate)
{
    this->localMatrix._42 = y;

    if(bWorldUpdate) this->UpdateWorldMatrix();
}

void Transform::SetLocalLocationZ(float z, bool bWorldUpdate)
{
    this->localMatrix._43 = z;

    if(bWorldUpdate) this->UpdateWorldMatrix();
}

/*** add ***/

void Transform::AddLocalLocation(const D3DXVECTOR3* const location, bool bWorldUpdate)
{
    this->localMatrix._41 += location->x;
    this->localMatrix._42 += location->y;
    this->localMatrix._43 += location->z;

    if(bWorldUpdate) this->UpdateWorldMatrix();
}

void Transform::AddLocalLocation(float x, float y, float z, bool bWorldUpdate)
{
    this->localMatrix._41 += x;
    this->localMatrix._42 += y;
    this->localMatrix._43 += z;

    if(bWorldUpdate) this->UpdateWorldMatrix();
}



/**************************************** 回転 ****************************************/

/***** world *****/

/*** get ***/

Rotation Transform::GetWorldRotation() const
{
    D3DXVECTOR3    dummy = {};
    D3DXQUATERNION tempQuat = {};
    D3DXMatrixDecompose(&dummy, &tempQuat, &dummy, &this->worldMatrix);
    return Rotation::QuatToRotation(&tempQuat);
}

D3DXQUATERNION Transform::GetWorldQuaternion() const
{
    D3DXQUATERNION result;
    D3DXMATRIX     rotationMatrix = this->GetWorldRotationMatrix();

    D3DXQuaternionRotationMatrix(&result, &rotationMatrix);

    return result;
}

/*** set ***/

void Transform::SetWorldRotation(const Rotation* const rotation, bool bLocalUpdate)
{
    if (!rotation) return;

    D3DXVECTOR3 tempLocation = this->GetWorldLocation(),
        tempScale    = this->GetWorldScale();

    this->worldMatrix 
        = this->CreateWorldTranslationMatrix(&tempLocation, rotation, &tempScale); 

    if(bLocalUpdate) this->UpdateLocalMatrix();
}

void Transform::SetWorldRotation(float yaw, float pitch, float roll, bool bLocalUpdate)
{
    D3DXVECTOR3 tempLocation = this->GetWorldLocation();
    D3DXVECTOR3 tempScale    = this->GetWorldScale();
    Rotation    tempRotation(yaw, pitch, roll);

    this->worldMatrix 
        = this->CreateWorldTranslationMatrix(&tempLocation, &tempRotation, &tempScale);

    if(bLocalUpdate) this->UpdateLocalMatrix();
}

/*** add ***/

void Transform::AddWorldRotation(const Rotation* const rotation, bool bLocalUpdate)
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

    if(bLocalUpdate) this->UpdateLocalMatrix();
}

void Transform::AddWorldRotation(float yaw, float pitch, float roll, bool bLocalUpdate)
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

    if(bLocalUpdate) this->UpdateLocalMatrix();
}

/*** Quat ***/

void Transform::WorldRotateAroundAxis(float x, float y, float z, float w, bool bLocalUpdate)
{
    D3DXVECTOR3    axis(x, y, z);
    D3DXVECTOR3    tempLocation = this->GetWorldLocation();
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

    if(bLocalUpdate) this->UpdateLocalMatrix();
}

void Transform::WorldRotateAroundAxis(const D3DXVECTOR3* axis, float w, bool bLocalUpdate)
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

    if(bLocalUpdate) this->UpdateLocalMatrix();
}

void Transform::SetWorldQuaternion(float x, float y, float z, float w, bool bLocalUpdate)
{
    D3DXVECTOR3    axis(x, y, z);
    D3DXVECTOR3    tempLocation = this->GetWorldLocation();
    D3DXVECTOR3    tempScale    = this->GetWorldScale();
    D3DXQUATERNION tempQuat(0.0f,0.0f,0.0f,1.0f);
    D3DXMATRIX     rotationMatrix;

    D3DXQuaternionRotationAxis(&tempQuat, &axis, w);
    D3DXMatrixRotationQuaternion(&rotationMatrix, &tempQuat);

    this->worldMatrix
        = Transform::CreateWorldTranslationMatrix(&tempLocation, &rotationMatrix, &tempScale);

    if(bLocalUpdate) this->UpdateLocalMatrix();
}

void Transform::SetWorldQuaternion(const D3DXVECTOR3 * axis, float w, bool bLocalUpdate)
{
    D3DXVECTOR3    tempLocation = this->GetWorldLocation();
    D3DXVECTOR3    tempScale    = this->GetWorldScale();
    D3DXQUATERNION tempQuat(0.0f,0.0f,0.0f,1.0f);
    D3DXMATRIX     rotationMatrix;

    D3DXQuaternionRotationAxis(&tempQuat, axis, w);
    D3DXMatrixRotationQuaternion(&rotationMatrix, &tempQuat);

    this->worldMatrix
        = Transform::CreateWorldTranslationMatrix(&tempLocation, &rotationMatrix, &tempScale);

    if(bLocalUpdate) this->UpdateLocalMatrix();
}

void Transform::SetWorldQuaternion(const D3DXQUATERNION* quat, bool bLocalUpdate)
{
    D3DXVECTOR3 tempLocation = this->GetWorldLocation();
    D3DXVECTOR3 tempScale    = this->GetWorldScale();
    D3DXMATRIX  rotationMatrix;

    D3DXMatrixRotationQuaternion(&rotationMatrix, quat);

    this->worldMatrix
        = Transform::CreateWorldTranslationMatrix(&tempLocation, &rotationMatrix, &tempScale);

    if(bLocalUpdate) this->UpdateLocalMatrix();
}

/***** local *****/

/*** get ***/

Rotation Transform::GetLocalRotation() const
{
    D3DXVECTOR3    dummy = {};
    D3DXQUATERNION tempQuat = {};
    D3DXMatrixDecompose(&dummy, &tempQuat, &dummy, &this->localMatrix);
    return Rotation::QuatToRotation(&tempQuat);
}

D3DXQUATERNION Transform::GetLocalQuaternion() const
{
    D3DXQUATERNION result;
    D3DXMATRIX     rotationMatrix = this->GetLocalRotationMatrix();

    D3DXQuaternionRotationMatrix(&result, &rotationMatrix);

    return result;
}

/*** set ***/

void Transform::SetLocalRotation(const Rotation* const rotation, bool bWorldUpdate)
{
    if (!rotation) return;

    D3DXVECTOR3 tempLocation = this->GetLocalLocation(),
        tempScale    = this->GetLocalScale();

    this->localMatrix
        = this->CreateWorldTranslationMatrix(&tempLocation, rotation, &tempScale);

    this->UpdateWorldMatrix();
}

void Transform::SetLocalRotation(float yaw, float pitch, float roll, bool bWorldUpdate)
{
    D3DXVECTOR3 tempLocation = this->GetLocalLocation();
    D3DXVECTOR3 tempScale    = this->GetLocalScale();
    Rotation    tempRotation(yaw, pitch, roll);

    this->localMatrix 
        = this->CreateWorldTranslationMatrix(&tempLocation, &tempRotation, &tempScale);

    if(bWorldUpdate) this->UpdateWorldMatrix();
}

/*** add ***/

void Transform::AddLocalRotation(const Rotation* const rotation, bool bWorldUpdate)
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

    if(bWorldUpdate) this->UpdateWorldMatrix();
}

void Transform::AddLocalRotation(float yaw, float pitch, float roll, bool bWorldUpdate)
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

    if(bWorldUpdate) this->UpdateWorldMatrix();
}

/*** Quat ***/

void Transform::LocalRotateAroundAxis(float x, float y, float z, float w, bool bWorldUpdate)
{
    D3DXVECTOR3    axis(x, y, z);
    D3DXVECTOR3    tempLocation = this->GetLocalLocation();
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

    if(bWorldUpdate) this->UpdateWorldMatrix();
}

void Transform::LocalRotateAroundAxis(const D3DXVECTOR3* axis, float w, bool bWorldUpdate)
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

    if(bWorldUpdate) this->UpdateWorldMatrix();
}

void Transform::SetLocalQuaternion(float x, float y, float z, float w, bool bWorldUpdate)
{
    D3DXVECTOR3    axis(x, y, z);
    D3DXVECTOR3    tempLocation = this->GetLocalLocation();
    D3DXVECTOR3    tempScale    = this->GetLocalScale();
    D3DXQUATERNION tempQuat(0.0f,0.0f,0.0f,1.0f);
    D3DXMATRIX     rotationMatrix;

    D3DXQuaternionRotationAxis(&tempQuat, &axis, w);
    D3DXMatrixRotationQuaternion(&rotationMatrix, &tempQuat);

    this->localMatrix
        = Transform::CreateWorldTranslationMatrix(&tempLocation, &rotationMatrix, &tempScale);

    if(bWorldUpdate) this->UpdateWorldMatrix();
}

void Transform::SetLocalQuaternion(const D3DXVECTOR3 * axis, float w, bool bWorldUpdate)
{
    D3DXVECTOR3    tempLocation = this->GetLocalLocation();
    D3DXVECTOR3    tempScale    = this->GetLocalScale();
    D3DXQUATERNION tempQuat(0.0f,0.0f,0.0f,1.0f);
    D3DXMATRIX     rotationMatrix;

    D3DXQuaternionRotationAxis(&tempQuat, axis, w);
    D3DXMatrixRotationQuaternion(&rotationMatrix, &tempQuat);

    this->localMatrix
        = Transform::CreateWorldTranslationMatrix(&tempLocation, &rotationMatrix, &tempScale);

    if(bWorldUpdate) this->UpdateWorldMatrix();
}

void Transform::SetLocalQuaternion(const D3DXQUATERNION * quat, bool bWorldUpdate)
{
    D3DXVECTOR3 tempLocation = this->GetLocalLocation();
    D3DXVECTOR3 tempScale    = this->GetLocalScale();
    D3DXMATRIX  rotationMatrix;

    D3DXMatrixRotationQuaternion(&rotationMatrix, quat);

    this->localMatrix
        = Transform::CreateWorldTranslationMatrix(&tempLocation, &rotationMatrix, &tempScale);

    if(bWorldUpdate) this->UpdateWorldMatrix();
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

void Transform::SetWorldScale(const D3DXVECTOR3* const scale, bool bLocalUpdate)
{
    if (!scale) return;

    D3DXVECTOR3 tempLocation   = this->GetWorldLocation();
    D3DXMATRIX  rotationMatrix = this->GetWorldRotationMatrix();

    this->worldMatrix
        = Transform::CreateWorldTranslationMatrix(&tempLocation, &rotationMatrix, scale);

    if(bLocalUpdate) this->UpdateLocalMatrix();
}

void Transform::SetWorldScale(float x, float y, float z, bool bLocalUpdate)
{
    D3DXVECTOR3 tempLocation   = this->GetWorldLocation();
    D3DXVECTOR3 tempScale      = { x, y, z };
    D3DXMATRIX  rotationMatrix = this->GetWorldRotationMatrix();

    this->worldMatrix
        = Transform::CreateWorldTranslationMatrix(&tempLocation, &rotationMatrix, &tempScale);

    if(bLocalUpdate) this->UpdateLocalMatrix();
}

void Transform::SetWorldScaleX(float x, bool bLocalUpdate)
{
    float mult = x / this->GetWorldScale().x;

    this->worldMatrix._11 *= mult;
    this->worldMatrix._12 *= mult;
    this->worldMatrix._13 *= mult;

    if(bLocalUpdate) this->UpdateLocalMatrix();
}

void Transform::SetWorldScaleY(float y, bool bLocalUpdate)
{
    float mult = y / this->GetWorldScale().y;

    this->worldMatrix._21 *= mult;
    this->worldMatrix._22 *= mult;
    this->worldMatrix._23 *= mult;

    if(bLocalUpdate) this->UpdateLocalMatrix();
}

void Transform::SetWorldScaleZ(float z, bool bLocalUpdate)
{
    float mult = z / this->GetWorldScale().z;

    this->worldMatrix._31 *= mult;
    this->worldMatrix._32 *= mult;
    this->worldMatrix._33 *= mult;

    if(bLocalUpdate) this->UpdateLocalMatrix();
}

/*** add ***/

void Transform::AddWorldScale(const D3DXVECTOR3* const scale, bool bLocalUpdate)
{
    if (!scale) return;

    D3DXVECTOR3 tempLocation   = this->GetWorldLocation();
    D3DXVECTOR3 tempScale      = this->GetWorldScale() + *scale;
    D3DXMATRIX  rotationMatrix = this->GetWorldRotationMatrix();

    this->worldMatrix
        = Transform::CreateWorldTranslationMatrix(&tempLocation, &rotationMatrix, &tempScale);

    if(bLocalUpdate) this->UpdateLocalMatrix();
}

void Transform::AddWorldScale(float x, float y, float z, bool bLocalUpdate)
{
    D3DXVECTOR3 tempLocation   = this->GetWorldLocation();
    D3DXVECTOR3 tempScale      = this->GetWorldScale() + D3DXVECTOR3(x, y, z);
    D3DXMATRIX  scaleMatrix, rotationInverse;
    D3DXMATRIX  rotationMatrix = this->GetWorldRotationMatrix();

    //this->worldMatrix._41 = 0;
    //this->worldMatrix._42 = 0;
    //this->worldMatrix._43 = 0;

    //D3DXMatrixInverse(&rotationInverse, nullptr, &rotationMatrix);

    //D3DXMatrixScaling(&scaleMatrix, x, y, z);
    //this->worldMatrix = this->worldMatrix * rotationInverse * scaleMatrix * rotationMatrix;

    //this->worldMatrix._41 = tempLocation.x;
    //this->worldMatrix._42 = tempLocation.y;
    //this->worldMatrix._43 = tempLocation.z;

    this->worldMatrix
        = Transform::CreateWorldTranslationMatrix(&tempLocation, &rotationMatrix, &tempScale);

    if(bLocalUpdate) this->UpdateLocalMatrix();
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

void Transform::SetLocalScale(const D3DXVECTOR3* const scale, bool bWorldUpdate)
{
    if (!scale) return;

    D3DXVECTOR3 tempLocation   = this->GetLocalLocation();
    D3DXMATRIX  rotationMatrix = this->GetLocalRotationMatrix();

    this->localMatrix
        = Transform::CreateWorldTranslationMatrix(&tempLocation, &rotationMatrix, scale);

    if(bWorldUpdate) this->UpdateLocalMatrix();
}

void Transform::SetLocalScale(float x, float y, float z, bool bWorldUpdate)
{
    D3DXVECTOR3 tempLocation   = this->GetLocalLocation();
    D3DXVECTOR3 tempScale      = { x, y, z };
    D3DXMATRIX  rotationMatrix = this->GetLocalRotationMatrix();

    this->localMatrix
        = Transform::CreateWorldTranslationMatrix(&tempLocation, &rotationMatrix, &tempScale);

    if(bWorldUpdate) this->UpdateWorldMatrix();
}

void Transform::SetLocalScaleX(float x, bool bWorldUpdate)
{
    float mult = x / this->GetLocalScale().x;

    this->localMatrix._11 *= mult;
    this->localMatrix._12 *= mult;
    this->localMatrix._13 *= mult;

    if(bWorldUpdate) this->UpdateWorldMatrix();
}

void Transform::SetLocalScaleY(float y, bool bWorldUpdate)
{
    float mult = y / this->GetLocalScale().y;

    this->localMatrix._21 *= mult;
    this->localMatrix._22 *= mult;
    this->localMatrix._23 *= mult;

    if(bWorldUpdate) this->UpdateWorldMatrix();
}

void Transform::SetLocalScaleZ(float z, bool bWorldUpdate)
{
    float mult = z / this->GetLocalScale().z;

    this->localMatrix._31 *= mult;
    this->localMatrix._32 *= mult;
    this->localMatrix._33 *= mult;

    if(bWorldUpdate) this->UpdateWorldMatrix();
}

/*** add ***/

void Transform::AddLocalScale(const D3DXVECTOR3* const scale, bool bWorldUpdate)
{
    if (!scale) return;

    D3DXVECTOR3 tempLocation   = this->GetLocalLocation();
    D3DXVECTOR3 tempScale      = this->GetLocalScale() + *scale;
    D3DXMATRIX  rotationMatrix = this->GetLocalRotationMatrix();

    this->localMatrix
        = Transform::CreateWorldTranslationMatrix(&tempLocation, &rotationMatrix, &tempScale);

    if(bWorldUpdate) this->UpdateLocalMatrix();
}

void Transform::AddLocalScale(float x, float y, float z, bool bWorldUpdate)
{
    D3DXVECTOR3 tempLocation   = this->GetLocalLocation();
    D3DXVECTOR3 tempScale      = this->GetLocalScale() + D3DXVECTOR3(x, y, z);
    D3DXMATRIX  rotationMatrix = this->GetLocalRotationMatrix();

    this->localMatrix
        = Transform::CreateWorldTranslationMatrix(&tempLocation, &rotationMatrix, &tempScale);

    if(bWorldUpdate) this->UpdateLocalMatrix();
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

D3DXVECTOR3 Transform::GetLocalForwardVector() const
{
    D3DXVECTOR3 result
    (
        this->localMatrix._31,
        this->localMatrix._32,
        this->localMatrix._33
    );

    D3DXVec3Normalize(&result, &result);

    return result;
}

D3DXVECTOR3 Transform::GetLocalUpVector() const
{
    D3DXVECTOR3 result
    (
        this->localMatrix._21,
        this->localMatrix._22,
        this->localMatrix._23
    );

    D3DXVec3Normalize(&result, &result);

    return result;
}

D3DXVECTOR3 Transform::GetLocalRightVector() const
{
    D3DXVECTOR3 result
    (
        this->localMatrix._11,
        this->localMatrix._12,
        this->localMatrix._13
    );

    D3DXVec3Normalize(&result, &result);

    return result;
}





/**************************************** 更新 ****************************************/

void Transform::UpdateWorldMatrix(bool bCallEventUpdated)
{
    // 親がいる場合
    if (this->HasParent())
    {
        D3DXMATRIX parentMatrix = this->parentTransform->GetWorldMatrix();

        // 親行列が単位行列の場合 掛け算しない
        D3DXMatrixIsIdentity(&parentMatrix)
            ? this->worldMatrix = this->localMatrix
            : this->worldMatrix = this->localMatrix * parentMatrix;
    }
    // 親がいない場合
    else
    {
        this->worldMatrix = this->localMatrix;
    }

    // 子供たちを更新
    for (auto&& child : this->childrenTransforms)
    {
        child->UpdateWorldMatrix();
    }

    // イベント発生
    if (bCallEventUpdated)
        this->EventTransformUpdated();
}

void Transform::UpdateLocalMatrix(bool bCallEventUpdated)
{
    // 親がいる場合
    if (this->HasParent())
    {
        D3DXMATRIX parentMatrix = this->parentTransform->GetWorldMatrix();
        
        if (!D3DXMatrixIsIdentity(&parentMatrix))
        {
            D3DXMATRIX parentInverse;

            D3DXMatrixInverse(&parentInverse, nullptr, &this->parentTransform->GetWorldMatrix());

            this->localMatrix = this->worldMatrix * parentInverse;
        }
        // 親行列が単位行列のとき
        else
        {
            this->localMatrix = this->worldMatrix;
        }
    }
    // 親がいない場合
    else
    {
        this->localMatrix = this->worldMatrix;
    }

    // 子供たちを更新
    for (auto&& child : this->childrenTransforms)
    {
        child->UpdateWorldMatrix();
    }

    // イベント発生
    if (bCallEventUpdated)
        this->EventTransformUpdated();
}