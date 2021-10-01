#include <vector>
#include <d3dx9.h>
#include "utils.hpp"
#pragma once

struct Rotation
{
	float yaw,
	      pitch,
	      roll;

	// 代入
	Rotation& operator = (const Rotation& rh)
	{
		this->yaw   = rh.yaw;
		this->pitch = rh.pitch;
		this->roll  = rh.roll;
		return *this;
	}

	// 算術
	Rotation operator + (const Rotation& rh) const
	{
		Rotation ret;
		ret.yaw   = this->yaw   + rh.yaw;
		ret.pitch = this->pitch + rh.pitch;
		ret.roll  = this->roll  + rh.roll;
		return ret;
	};
	Rotation operator - (const Rotation& rh) const
	{
		Rotation ret;
		ret.yaw   = this->yaw   - rh.yaw;
		ret.pitch = this->pitch - rh.pitch;
		ret.roll  = this->roll  - rh.roll;
		return ret;
	};
	Rotation operator * (const Rotation& rh) const
	{
		Rotation ret;
		ret.yaw   = this->yaw   * rh.yaw;
		ret.pitch = this->pitch * rh.pitch;
		ret.roll  = this->roll  * rh.roll;
		return ret;
	};
	Rotation operator * (float rh) const
	{
		Rotation ret;
		ret.yaw   = this->yaw   * rh;
		ret.pitch = this->pitch * rh;
		ret.roll  = this->roll  * rh;
		return ret;
	};
	Rotation operator / (const Rotation& rh) const
	{
		Rotation ret;
		ret.yaw   = this->yaw   / rh.yaw;
		ret.pitch = this->pitch / rh.pitch;
		ret.roll  = this->roll  / rh.roll;
		return ret;
	};
	Rotation operator / (float rh) const
	{
		Rotation ret;
		ret.yaw   = this->yaw   / rh;
		ret.pitch = this->pitch / rh;
		ret.roll  = this->roll  / rh;
		return ret;
	};

	// 複合
	Rotation& operator += (const Rotation& rh)
	{
		this->yaw   += rh.yaw;
		this->pitch += rh.pitch;
		this->roll  += rh.roll;
		return *this;
	};
	Rotation& operator -= (const Rotation& rh)
	{
		this->yaw   -= rh.yaw;
		this->pitch -= rh.pitch;
		this->roll  -= rh.roll;
		return *this;
	};
	Rotation& operator *= (const Rotation& rh)
	{
		this->yaw   *= rh.yaw;
		this->pitch *= rh.pitch;
		this->roll  *= rh.roll;
		return *this;
	};
	Rotation& operator *= (float rh)
	{
		this->yaw   *= rh;
		this->pitch *= rh;
		this->roll  *= rh;
		return *this;
	};
	Rotation& operator /= (const Rotation& rh)
	{
		this->yaw   /= rh.yaw;
		this->pitch /= rh.pitch;
		this->roll  /= rh.roll;
		return *this;
	};
	Rotation& operator /= (float rh)
	{
		this->yaw   /= rh;
		this->pitch /= rh;
		this->roll  /= rh;
		return *this;
	};
	Rotation& operator %= (float rh)
	{
		this->yaw   = fmodf(this->yaw,rh);
		this->pitch = fmodf(this->pitch,rh);
		this->roll  = fmodf(this->roll,rh);
		return *this;
	};

	//単項
	Rotation operator + () const
	{
		return *this;
	};
	Rotation operator - () const
	{
		Rotation ret;
		ret.yaw   = -this->yaw;
		ret.pitch = -this->pitch;
		ret.roll  = -this->roll;
		return ret;
	};

	// 比較
	bool operator == (const Rotation& rh) const
	{
		if (!(this->yaw   == rh.yaw))   return false;
		if (!(this->pitch == rh.pitch)) return false;
		if (!(this->roll  == rh.roll))  return false;
		return true;
	}
	bool operator != (const Rotation& rh) const
	{
		if (!(this->yaw   != rh.yaw))   return false;
		if (!(this->pitch != rh.pitch)) return false;
		if (!(this->roll  != rh.roll))  return false;
		return true;
	}
	bool operator < (const Rotation& rh) const
	{
		if (!(this->yaw   < rh.yaw))   return false;
		if (!(this->pitch < rh.pitch)) return false;
		if (!(this->roll  < rh.roll))  return false;
		return true;
	}
	bool operator > (const Rotation& rh) const
	{
		if (!(this->yaw   > rh.yaw))   return false;
		if (!(this->pitch > rh.pitch)) return false;
		if (!(this->roll  > rh.roll))  return false;
		return true;
	}

	// キャスト
	operator D3DXVECTOR3() const 
	{
		return D3DXVECTOR3
		(
			this->pitch,
			this->yaw,
			this->roll
		);
	}
	
	// 自身を弧度法に変換
	Rotation ToRadian()
	{
		float convert = D3DX_PI / 180.0f;

		return Rotation(this->yaw * convert, this->pitch * convert, this->roll * convert);
	}

	// 自身のラジアン値を返す
	Rotation GetRadian() const
	{
		float convert = D3DX_PI / 180.0f;

		return Rotation(this->yaw * convert, this->pitch * convert, this->roll * convert);
	}

	// 自身を度数法に変換
	Rotation ToDegree()
	{
		float convert = 180.0f / D3DX_PI;

		return Rotation(this->yaw *= convert, this->pitch *= convert, this->roll *= convert);
	}

	// 自身の度を返す
	Rotation GetDegree() const
	{
		float convert = D3DX_PI / 180.0f;

		return Rotation(this->yaw * convert, this->pitch * convert, this->roll * convert);
	}
	

	// -PI ~ +PI の範囲に正規化
	void Normalize(bool degree = false)
	{
		const float unit = (degree ? 180.0f : D3DX_PI);

		this->yaw   = fmodf(this->yaw   + unit, (2 * unit)) - unit;
		this->pitch = fmodf(this->pitch + unit, (2 * unit)) - unit;
		this->roll  = fmodf(this->roll  + unit, (2 * unit)) - unit;

		if (this->yaw   < -unit) this->yaw   += (2 * unit);
		if (this->pitch < -unit) this->pitch += (2 * unit);
		if (this->roll  < -unit) this->roll  += (2 * unit);
	}

	// 正規化されたRotationを返す
	Rotation GetNormal(bool degree = false) const
	{
		Rotation result;
		const float unit = (degree ? 180.0f : D3DX_PI);

		result.yaw   = fmodf(this->yaw   + unit, (2 * unit)) - unit;
		result.pitch = fmodf(this->pitch + unit, (2 * unit)) - unit;
		result.roll  = fmodf(this->roll  + unit, (2 * unit)) - unit;

		if (result.yaw   < -unit) result.yaw   += (2 * unit);
		if (result.pitch < -unit) result.pitch += (2 * unit);
		if (result.roll  < -unit) result.roll  += (2 * unit);

		return result;
	}
	
	// クォータニオン -> オイラー角
	static Rotation QuatToRotation(const D3DXQUATERNION* q)
	{
		Rotation result;

		// yaw
		float sinp = 2 * (q->w * q->y - q->z * q->x);
		if (std::fabsf(sinp) >= 1)
			result.yaw = std::copysign(D3DX_PI / 2, sinp); // use 90 degrees if out of range
		else
			result.yaw = std::asinf(sinp);

		// pitch
		float sinr_cosp = 2 * (q->w * q->x + q->y * q->z);
		float cosr_cosp = 1 - 2 * (q->x * q->x + q->y * q->y);
		result.pitch = std::atan2f(sinr_cosp, cosr_cosp);

		// roll
		float siny_cosp = 2 * (q->w * q->z + q->x * q->y);
		float cosy_cosp = 1 - 2 * (q->y * q->y + q->z * q->z);
		result.roll = std::atan2(siny_cosp, cosy_cosp);

		return result;
	};

	static Rotation MatrixToRotation(const D3DXMATRIX* m)
	{
		return Rotation
		(
			atan2f(-m->_13, sqrtf(m->_23 * m->_23 + m->_33 * m->_33) ),
			atan2f(m->_23, m->_33),
			atan2f(m->_12, m->_11)
		);
	}
};

class Transform
{
public:
	/***** ctor, dtor *****/

	Transform();

	Transform(Transform* const parent);

	Transform
	(
		Transform*   const parent,
		D3DXVECTOR3* const location,
		Rotation*    const rotation = nullptr,
		D3DXVECTOR3* const scale    = nullptr
	);

	~Transform();


public:
	/***** 親子関連 *****/

	// accessor
    Transform* GetParent() const;
	void SetParent(Transform* const parent);

	const std::vector<Transform*>& GetChildren() const;

	// 親子になる ( nullptr を指定したら親子解除 )
	void BecomeParents(Transform* const parent);

	// 子を追加
	bool AddChild(Transform* const child);

	// 子を除く
	bool RemoveChild(Transform* const child);

	// 先祖に ancestor が存在するか
	bool CheckAncestor(Transform* const ancestor);

	// 親を持ってるか
	bool HasParent();

	// 子を持ってるか
	bool HasChild();

public:
	/***** matrix *****/

	const D3DXMATRIX& GetWorldMatrix()  const;
	const D3DXMATRIX& GetLocalMatrix()  const;

	// 回転行列を取得
	D3DXMATRIX GetWorldRotationMatrix() const;
	D3DXMATRIX GetLocalRotationMatrix() const;

	// ワールド変換行列を生成
	static D3DXMATRIX CreateWorldTranslationMatrix
	(
		const D3DXVECTOR3* const location,
		const Rotation*    const rotation,
		const D3DXVECTOR3* const scale
	);
	static D3DXMATRIX CreateWorldTranslationMatrix
	(
		const D3DXVECTOR3* const location,
		const D3DXMATRIX*  const rotationMatrix,
		const D3DXVECTOR3* const scale
	);



public:
	/***** location *****/

	/*** world ***/

	D3DXVECTOR3 GetWorldLocation() const;

	void SetWorldLocation(const D3DXVECTOR3* const location);
	void SetWorldLocation(float x, float y, float z);

	void SetWorldLocationX(float x);
	void SetWorldLocationY(float y);
	void SetWorldLocationZ(float z);

	void AddWorldLocation(const D3DXVECTOR3* const location);
	void AddWorldLocation(float x, float y, float z);

	/*** local ***/

	D3DXVECTOR3 GetLocalLocation() const;

	void SetLocalLocation(const D3DXVECTOR3* const location);
	void SetLocalLocation(float x, float y, float z);

	void SetLocalLocationX(float x);
	void SetLocalLocationY(float y);
	void SetLocalLocationZ(float z);

	void AddLocalLocation(const D3DXVECTOR3* const location);
	void AddLocalLocation(float x, float y, float z);


public:
	/***** rotation *****/

	/*** world ***/

	// 誤差有
	Rotation GetWorldRotation() const;

	void SetWorldRotation(const Rotation* const rotation);
	void SetWorldRotation(float yaw, float pitch, float roll);

	void AddWorldRotation(const Rotation* const rotation);
	void AddWorldRotation(float yaw, float pitch, float roll);

	void WorldRotateAroundAxis(float x, float y, float z, float w);
	void WorldRotateAroundAxis(const D3DXVECTOR3* axis, float w);

	/*** local ***/

	// 誤差有
	Rotation GetLocalRotation() const;

	void SetLocalRotation(const Rotation* const rotation);
	void SetLocalRotation(float yaw, float pitch, float roll);

	void AddLocalRotation(const Rotation* const rotation);
	void AddLocalRotation(float yaw, float pitch, float roll);

	void LocalRotateAroundAxis(float x, float y, float z, float w);
	void LocalRotateAroundAxis(const D3DXVECTOR3* axis, float w);


public:
	/***** scale *****/

	/*** world ***/

	// 誤差有
	D3DXVECTOR3 GetWorldScale() const;

	void SetWorldScale(const D3DXVECTOR3* const scale);
	void SetWorldScale(float x, float y, float z);

	void SetWorldScaleX(float x);
	void SetWorldScaleY(float y);
	void SetWorldScaleZ(float z);

	void AddWorldScale(const D3DXVECTOR3* const scale);
	void AddWorldScale(float x, float y, float z);

	/*** local ***/
	
	// 誤差有
	D3DXVECTOR3 GetLocalScale() const;

	void SetLocalScale(const D3DXVECTOR3* const scale);
	void SetLocalScale(float x, float y, float z);

	void SetLocalScaleX(float x);
	void SetLocalScaleY(float y);
	void SetLocalScaleZ(float z);

	void AddLocalScale(const D3DXVECTOR3* const scale);
	void AddLocalScale(float x, float y, float z);


public:
	/***** direction *****/

	// z direction ( world )
	D3DXVECTOR3 GetForwardVector() const;

	// y direction ( world )
	D3DXVECTOR3 GetUpVector() const;

	// x direction ( world )
	D3DXVECTOR3 GetRightVector() const;


public:
	/****** matrix updater *****/

	// ワールド行列を更新
	void UpdateWorldMatrix();

	// ローカル行列を更新
	void UpdateLocalMatrix();


protected:
	// worldMatrix を直接変更したら、UpdateLocalMatrix() も呼ぶこと
	D3DXMATRIX worldMatrix;

	// localMatrix を直接変更したら、UpdateWorldMatrix() も呼ぶこと
	D3DXMATRIX localMatrix;


private:
	std::vector<Transform*> childrenTransforms;

	Transform* parentTransform;
};
