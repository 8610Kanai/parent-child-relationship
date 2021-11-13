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

	


	/// <summary>
	///  回転からベクトルを取得 例 : Rotation(0,0,0) => D3DXVECTOR3(1,0,0)
	/// </summary>
	/// <param name="mode"> 0 : Forward(デフォルト)</param>
	/// <param name="mode"> 1 : up</param>
	/// <param name="mode"> 2 : left</param>
	/// <returns> mode に応じたベクトル </returns>
	D3DXVECTOR3 ToVector3(int mode = 0) const
	{
		float y =  this->yaw   * (acos(-1.0f) / 180.0f),
			  p =  this->pitch * (acos(-1.0f) / 180.0f),
			  r = -this->roll  * (acos(-1.0f) / 180.0f);

		float sy = sin(y),
			  sp = sin(p),
			  sr = sin(r);

		float cy = cos(y),
			  cp = cos(p),
			  cr = cos(r);

		float vx, vy, vz;
		vx = vy = vz = 0.0f;

		int roundNum = 100000;
		switch (mode)
		{
		case 0: // forward
			vx = round( (-sp*sr*sy + cr*cy) * roundNum ) / roundNum;  //少数第 log(roundNum) 桁で四捨五入
			vy = round(            (-cp*sr) * roundNum ) / roundNum;
			vz = round( (-cr*sy - sp*sr*cy) * roundNum ) / roundNum;
			break;

		case 1: // up
			vx = round( ( sp*cr*sy + sr*cy) * roundNum ) / roundNum;
			vy = round(             (cp*cr) * roundNum ) / roundNum;
			vz = round( (-sr*sy + sp*cr*cy) * roundNum ) / roundNum;
			break;

		case 2: // left
			vx = round( (cp*sy) * roundNum ) / roundNum;
			vy = round(   (-sp) * roundNum ) / roundNum;
			vz = round( (cp*cy) * roundNum ) / roundNum;
			break;

		default:
			OutputDebugFormat("Rotation.ToVector : mode error.\n");
			break;
		}

		return D3DXVECTOR3(vx, vy, vz);
	}

	D3DXVECTOR3 RadianToVector3(int mode = 0) const
	{
		float sy = sin(this->yaw),
			  sp = sin(this->pitch),
			  sr = sin(-this->roll);

		float cy = cos(this->yaw),
			  cp = cos(this->pitch),
			  cr = cos(-this->roll);

		float vx, vy, vz;
		vx = vy = vz = 0.0f;

		int roundNum = 100000;
		switch (mode)
		{
		case 0: // forward
			vx = round( (-sp*sr*sy + cr*cy) * roundNum ) / roundNum;  //少数第 log(roundNum) 桁で四捨五入
			vy = round(            (-cp*sr) * roundNum ) / roundNum;
			vz = round( (-cr*sy - sp*sr*cy) * roundNum ) / roundNum;
			break;

		case 1: // up
			vx = round( ( sp*cr*sy + sr*cy) * roundNum ) / roundNum;
			vy = round(             (cp*cr) * roundNum ) / roundNum;
			vz = round( (-sr*sy + sp*cr*cy) * roundNum ) / roundNum;
			break;

		case 2: // left
			vx = round( (cp*sy) * roundNum ) / roundNum;
			vy = round(   (-sp) * roundNum ) / roundNum;
			vz = round( (cp*cy) * roundNum ) / roundNum;
			break;

		default:
			OutputDebugFormat("Rotation.ToVector : mode error.\n");
			break;
		}

		return D3DXVECTOR3(vx, vy, vz);
	}
	
	// 自身を弧度法に変換
	Rotation ToRadian()
	{
		float convert = 3.141592654f / 180.0f;

		return Rotation(this->yaw * convert, this->pitch * convert, this->roll * convert);
	}

	// 自身のラジアン値を返す
	Rotation GetRadian() const
	{
		float convert = 3.141592654f / 180.0f;

		return Rotation(this->yaw * convert, this->pitch * convert, this->roll * convert);
	}

	// 自身を度数法に変換
	Rotation ToDegree()
	{
		float convert = 180.0f / 3.141592654f;

		return Rotation(this->yaw *= convert, this->pitch *= convert, this->roll *= convert);
	}

	// 自身の度を返す
	Rotation GetDegree() const
	{
		float convert = 3.141592654f / 180.0f;

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
		Rotation*    const rotation,
		D3DXVECTOR3* const scale
	);

	Transform
	(
		Transform*  const parent,
		const D3DXMATRIX* const localMatrix
	);

	~Transform();


public:
	/***** 親子関連 *****/

	// 親を取得
    Transform* GetParent() const;

	// 親を指定する ( parent = nullptr で親子解除 )
	void SetParent(Transform* const parent);

	// 子供たちを取得
	const std::vector<Transform*>& GetChildren() const;

	// 親子になる ( parent = nullptr で親子解除 )
	void BecomeParents(Transform* const parent);

	// 親子を解除する
	void BreakParents();

	// 子を追加
	bool AddChild(Transform* const child);

	// 子を除く
	bool RemoveChild(Transform* const child);

	// 先祖に 引数のancestor が存在するか
	bool CheckAncestor(Transform* const ancestor);

	// 親を持ってるか
	bool HasParent();

	// 子を持ってるか
	bool HasChild();

public:
	/***** matrix *****/

	// ワールド行列を取得
	const D3DXMATRIX& GetWorldMatrix()  const;

	// ワールド行列をセット、ローカル行列を更新
	void SetWorldMatrix(const D3DXMATRIX* const worldMatrix);
	
	// ローカル行列を取得
	const D3DXMATRIX& GetLocalMatrix()  const;

	// ローカル行列をセット、ワールド行列を更新
	void SetLocalMatrix(const D3DXMATRIX* const localMatrix);

	// ワールド回転行列を取得
	D3DXMATRIX GetWorldRotationMatrix() const;

	// ローカル回転行列を取得
	D3DXMATRIX GetLocalRotationMatrix() const;

	// ワールド変換行列を生成

	/// <summary>
	/// ワールド変換行列を生成
	/// </summary>
	/// <param name="location">	座標 </param>
	/// <param name="rotation">	オイラー角 </param>
	/// <param name="scale">	スケール </param>
	/// <returns> ワールド変換行列 </returns>
	static D3DXMATRIX CreateWorldTranslationMatrix
	(
		const D3DXVECTOR3* const location,
		const Rotation*    const rotation,
		const D3DXVECTOR3* const scale
	);

	/// <summary>
	/// ワールド変換行列を生成
	/// </summary>
	/// <param name="location">			座標 </param>
	/// <param name="rotationMatrix">	回転行列 </param>
	/// <param name="scale">			スケール </param>
	/// <returns></returns>
	static D3DXMATRIX CreateWorldTranslationMatrix
	(
		const D3DXVECTOR3* const location,
		const D3DXMATRIX*  const rotationMatrix,
		const D3DXVECTOR3* const scale
	);



public:
	/***** location *****/

	//******************************************************************************************
	// world
	//******************************************************************************************

	/// ワールド座標を取得
	D3DXVECTOR3 GetWorldLocation() const;

	/// <summary>
	/// ワールド座標をセット
	/// </summary>
	/// <param name="location">		セットする座標 </param>
	/// <param name="bLocalUpdate">	ローカル行列を更新するか (デフォルトで true) </param>
	void SetWorldLocation(const D3DXVECTOR3* const location, bool bLocalUpdate = true);

	/// <summary>
	/// ワールド座標をセット
	/// </summary>
	/// <param name="(x, y, z)">	セットする座標 </param>
	/// <param name="bLocalUpdate">	ローカル行列を更新するか (デフォルトで true) </param>
	void SetWorldLocation(float x, float y, float z, bool bLocalUpdate = true);

	/// <summary>
	/// ワールド座標( x のみ)をセット
	/// </summary>
	/// <param name="x">			セットする x 座標 </param>
	/// <param name="bLocalUpdate">	ローカル行列を更新するか (デフォルトで true) </param>
	void SetWorldLocationX(float x, bool bLocalUpdate = true);

	/// <summary>
	/// ワールド座標( y のみ)をセット
	/// </summary>
	/// <param name="y">			セットする y 座標 </param>
	/// <param name="bLocalUpdate">	ローカル行列を更新するか (デフォルトで true) </param>
	void SetWorldLocationY(float y, bool bLocalUpdate = true);

	/// <summary>
	/// ワールド座標( z のみ)をセット
	/// </summary>
	/// <param name="z">			セットする z 座標 </param>
	/// <param name="bLocalUpdate">	ローカル行列を更新するか (デフォルトで true) </param>
	void SetWorldLocationZ(float z, bool bLocalUpdate = true);

	/// <summary>
	/// ワールド座標に加算
	/// </summary>
	/// <param name="location">		加算する座標値 </param>
	/// <param name="bLocalUpdate">	ローカル行列を更新するか (デフォルトで true) </param>
	void AddWorldLocation(const D3DXVECTOR3* const location, bool bLocalUpdate = true);

	/// <summary>
	/// ワールド座標に加算
	/// </summary>
	/// <param name="(x, y, z)">	加算する座標値 </param>
	/// <param name="bLocalUpdate">	ローカル行列を更新するか (デフォルトで true) </param>
	void AddWorldLocation(float x, float y, float z, bool bLocalUpdate = true);

	//******************************************************************************************
	// local
	//******************************************************************************************

	/// ローカル座標を取得
	D3DXVECTOR3 GetLocalLocation() const;

	/// <summary>
	/// ローカル座標をセット
	/// </summary>
	/// <param name="location">		セットする座標 </param>
	/// <param name="bWorldUpdate">	ワールド行列を更新するか (デフォルトで true) </param>
	void SetLocalLocation(const D3DXVECTOR3* const location, bool bWorldUpdate = true);

	/// <summary>
	/// ローカル座標をセット
	/// </summary>
	/// <param name="(x, y, z)">	セットする座標 </param>
	/// <param name="bWorldUpdate">	ワールド行列を更新するか (デフォルトで true) </param>
	void SetLocalLocation(float x, float y, float z, bool bWorldUpdate = true);

	/// <summary>
	/// ローカル座標( x のみ)をセット
	/// </summary>
	/// <param name="x">			セットする x 座標 </param>
	/// <param name="bWorldUpdate">	ワールド行列を更新するか (デフォルトで true) </param>
	void SetLocalLocationX(float x, bool bWorldUpdate = true);

	/// <summary>
	/// ローカル座標( y のみ)をセット
	/// </summary>
	/// <param name="y">			セットする y 座標 </param>
	/// <param name="bWorldUpdate">	ワールド行列を更新するか (デフォルトで true) </param>
	void SetLocalLocationY(float y, bool bWorldUpdate = true);

	/// <summary>
	/// ローカル座標( z のみ)をセット
	/// </summary>
	/// <param name="z">			セットする z 座標 </param>
	/// <param name="bWorldUpdate">	ワールド行列を更新するか (デフォルトで true) </param>
	void SetLocalLocationZ(float z, bool bWorldUpdate = true);

	/// <summary>
	/// ローカル座標に加算
	/// </summary>
	/// <param name="location">		加算する座標値 </param>
	/// <param name="bWorldUpdate">	ワールド行列を更新するか (デフォルトで true) </param>
	void AddLocalLocation(const D3DXVECTOR3* const location, bool bWorldUpdate = true);

	/// <summary>
	/// ローカル座標に加算
	/// </summary>
	/// <param name="(x, y, z)">	加算する座標値 </param>
	/// <param name="bWorldUpdate">	ワールド行列を更新するか (デフォルトで true) </param>
	void AddLocalLocation(float x, float y, float z, bool bWorldUpdate = true);


public:
	/***** rotation, quaternion *****/

	/*** world ***/

	/// ワールド行列のオイラー角回転量を取得 (誤差有)
	Rotation GetWorldRotation() const;

	/// クォータニオン(world)を取得
	D3DXQUATERNION GetWorldQuaternion() const;

	/// <summary>
	/// ワールド行列の回転をセット
	/// </summary>
	/// <param name="rotation">		セットする回転 </param>
	/// <param name="bLocalUpdate">	ローカル行列を更新するか (デフォルトで true) </param>
	void SetWorldRotation(const Rotation* const rotation, bool bLocalUpdate = true);

	/// <summary>
	/// ワールド行列の回転をセット
	/// </summary>
	/// <param name="(yaw, pitch, roll)">	セットする回転 </param>
	/// <param name="bLocalUpdate">			ローカル行列を更新するか (デフォルトで true) </param>
	void SetWorldRotation(float yaw, float pitch, float roll, bool bLocalUpdate = true);

	/// <summary>
	/// ワールド行列の回転に加算
	/// </summary>
	/// <param name="rotation">		加算する回転量 </param>
	/// <param name="bLocalUpdate">	ローカル行列を更新するか (デフォルトで true) </param>
	void AddWorldRotation(const Rotation* const rotation, bool bLocalUpdate = true);

	/// <summary>
	/// ワールド行列の回転に加算
	/// </summary>
	/// <param name="(yaw, pitch, roll)">	加算する回転量 </param>
	/// <param name="bLocalUpdate">			ローカル行列を更新するか (デフォルトで true) </param>
	void AddWorldRotation(float yaw, float pitch, float roll, bool bLocalUpdate = true);

	/// <summary>
	/// 回転軸と回転量からワールド行列を回転
	/// </summary>
	/// <param name="(x, y, z)	">	回転軸 </param>
	/// <param name="w	">			回転量 </param>
	/// <param name="bLocalUpdate"> ローカル行列を更新するか (デフォルトで true)</param>
	void WorldRotateAroundAxis(float x, float y, float z, float w, bool bLocalUpdate = true);

	/// <summary>
	/// 回転軸と回転量からワールド行列を回転
	/// </summary>
	/// <param name="axis	">		回転軸 </param>
	/// <param name="w	">			回転量 </param>
	/// <param name="bLocalUpdate">	ローカル行列を更新するか (デフォルトで true) </param>
	void WorldRotateAroundAxis(const D3DXVECTOR3* axis, float w, bool bLocalUpdate = true);

	/// <summary>
	/// ワールド行列の回転をクォータニオンでセット
	/// </summary>
	/// <param name="(x, y, z, w)">	セットするクォータニオン </param>
	/// <param name="bLocalUpdate">	ローカル行列を更新するか (デフォルトで true) </param>
	void SetWorldQuaternion(float x, float y, float z, float w, bool bLocalUpdate = true);

	/// <summary>
	/// ワールド行列の回転をクォータニオンでセット
	/// </summary>
	/// <param name="(axis, w)">	セットするクォータニオン </param>
	/// <param name="bLocalUpdate">	ローカル行列を更新するか (デフォルトで true) </param>
	void SetWorldQuaternion(const D3DXVECTOR3* axis, float w, bool bLocalUpdate = true);

	/// <summary>
	/// ワールド行列の回転をクォータニオンでセット
	/// </summary>
	/// <param name="quat">			セットするクォータニオン</param>
	/// <param name="bLocalUpdate">	ローカル行列を更新するか (デフォルトで true) </param>
	void SetWorldQuaternion(const D3DXQUATERNION* quat, bool bLocalUpdate = true);

	/*** local ***/
	
	/// ローカル行列のオイラー角回転量を取得 (誤差有)
	Rotation       GetLocalRotation()   const;

	/// ローカル行列のクォータニオンを取得
	D3DXQUATERNION GetLocalQuaternion() const;

	/// <summary>
	/// ローカル行列の回転をセット
	/// </summary>
	/// <param name="rotation">		セットする回転 </param>
	/// <param name="bWorldUpdate">	ワールド行列を更新するか (デフォルトで true) </param>
	void SetLocalRotation(const Rotation* const rotation, bool bWorldUpdate = true);

	/// <summary>
	/// ローカル行列の回転をセット
	/// </summary>
	/// <param name="(yaw, pitch, roll)">	セットする回転 </param>
	/// <param name="bWorldUpdate">			ワールド行列を更新するか (デフォルトで true) </param>
	void SetLocalRotation(float yaw, float pitch, float roll, bool bWorldUpdate = true);

	/// <summary>
	/// ローカル行列の回転に加算
	/// </summary>
	/// <param name="rotation">		加算する回転量 </param>
	/// <param name="bWorldUpdate">	ワールド行列を更新するか (デフォルトで true)	</param>
	void AddLocalRotation(const Rotation* const rotation, bool bWorldUpdate = true);

	/// <summary>
	/// ローカル行列の回転に加算
	/// </summary>
	/// <param name="(yaw, pitch, roll)">	加算する回転量 </param>
	/// <param name="bWorldUpdate">			ワールド行列を更新するか (デフォルトで true) </param>
	void AddLocalRotation(float yaw, float pitch, float roll, bool bWorldUpdate = true);

	/// <summary>
	/// 回転軸と回転量からローカル行列を回転
	/// </summary>
	/// <param name="(x, y, z)	">	回転軸 </param>
	/// <param name="w	">			回転量 </param>
	/// <param name="bWorldUpdate">	ワールド行列を更新するか (デフォルトで true) </param>
	void LocalRotateAroundAxis(float x, float y, float z, float w, bool bWorldUpdate = true);

	/// <summary>
	/// 回転軸と回転量からローカル行列を回転
	/// </summary>
	/// <param name="axis	">		回転軸 </param>
	/// <param name="w	">			回転量 </param>
	/// <param name="bWorldUpdate">	ワールド行列を更新するか (デフォルトで true) </param>
	void LocalRotateAroundAxis(const D3DXVECTOR3* axis, float w, bool bWorldUpdate = true);

	/// <summary>
	/// ローカル行列の回転をクォータニオンでセット
	/// </summary>
	/// <param name="(x, y, z, w)">	セットするクォータニオン </param>
	/// <param name="bWorldUpdate">	ワールド行列を更新するか (デフォルトで true) </param>
	void SetLocalQuaternion(float x, float y, float z, float w, bool bWorldUpdate = true);

	/// <summary>
	/// ローカル行列の回転をクォータニオンでセット
	/// </summary>
	/// <param name="(axis, w)">	セットするクォータニオン </param>
	/// <param name="bWorldUpdate">	ワールド行列を更新するか (デフォルトで true)</param>
	void SetLocalQuaternion(const D3DXVECTOR3* axis, float w, bool bWorldUpdate = true);

	/// <summary>
	/// ローカル行列の回転をクォータニオンでセット
	/// </summary>
	/// <param name="quat">			セットするクォータニオン </param>
	/// <param name="bWorldUpdate">	ワールド行列を更新するか (デフォルトで true) </param>
	void SetLocalQuaternion(const D3DXQUATERNION* quat, bool bWorldUpdate = true);


public:
	/***** scale *****/

	/*** world ***/

	/// ワールド行列のスケールを取得
	D3DXVECTOR3 GetWorldScale() const;

	/// <summary>
	/// ワールド行列のスケールをセット
	/// </summary>
	/// <param name="scale">		セットするスケール </param>
	/// <param name="bLocalUpdate">	ローカル行列を更新するか (デフォルトで true) </param>
	void SetWorldScale(const D3DXVECTOR3* const scale, bool bLocalUpdate = true);

	/// <summary>
	/// ワールド行列のスケールをセット
	/// </summary>
	/// <param name="(x, y, z)">	セットするスケール </param>
	/// <param name="bLocalUpdate">	ローカル行列を更新するか (デフォルトで true) </param>
	void SetWorldScale(float x, float y, float z, bool bLocalUpdate = true);

	/// <summary>
	/// ワールド行列のスケール( x のみ)をセット
	/// </summary>
	/// <param name="x">			セットする x スケール </param>
	/// <param name="bLocalUpdate">	ローカル行列を更新するか (デフォルトで true) </param>
	void SetWorldScaleX(float x, bool bLocalUpdate = true);

	/// <summary>
	/// ワールド行列のスケール( y のみ)をセット
	/// </summary>
	/// <param name="y">			セットする y スケール </param>
	/// <param name="bLocalUpdate">	ローカル行列を更新するか (デフォルトで true) </param>
	void SetWorldScaleY(float y, bool bLocalUpdate = true);

	/// <summary>
	/// ワールド行列のスケール( z のみ)をセット
	/// </summary>
	/// <param name="z">			セットする z スケール </param>
	/// <param name="bLocalUpdate">	ローカル行列を更新するか (デフォルトで true) </param>
	void SetWorldScaleZ(float z, bool bLocalUpdate = true);

	/// <summary>
	/// ワールド行列のスケールに加算
	/// </summary>
	/// <param name="scale">		加算するスケール </param>
	/// <param name="bLocalUpdate">	ローカル行列を更新するか (デフォルトで true) </param>
	void AddWorldScale(const D3DXVECTOR3* const scale, bool bLocalUpdate = true);

	/// <summary>
	/// ワールド行列のスケールに加算
	/// </summary>
	/// <param name="(x, y, z)">	加算するスケール </param>
	/// <param name="bLocalUpdate">	ローカル行列を更新するか (デフォルトで true) </param>
	void AddWorldScale(float x, float y, float z, bool bLocalUpdate = true);

	/*** local ***/
	
	/// ローカル行列のスケールを取得
	D3DXVECTOR3 GetLocalScale() const;

	/// <summary>
	/// ローカル行列のスケールをセット
	/// </summary>
	/// <param name="scale">		セットするスケール </param>
	/// <param name="bWorldUpdate">	ワールド行列を更新するか (デフォルトで true) </param>
	void SetLocalScale(const D3DXVECTOR3* const scale, bool bWorldUpdate = true);

	/// <summary>
	/// ローカル行列のスケールをセット
	/// </summary>
	/// <param name="(x, y, z)">	セットするスケール </param>
	/// <param name="bWorldUpdate">	ワールド行列を更新するか (デフォルトで true) </param>
	void SetLocalScale(float x, float y, float z, bool bWorldUpdate = true);

	/// <summary>
	/// ローカル行列のスケール( x のみ)をセット
	/// </summary>
	/// <param name="x">			セットする x スケール </param>
	/// <param name="bWorldUpdate">	ワールド行列を更新するか (デフォルトで true) </param>
	void SetLocalScaleX(float x, bool bWorldUpdate = true);

	/// <summary>
	/// ローカル行列のスケール( y のみ)をセット
	/// </summary>
	/// <param name="y">			セットする y スケール </param>
	/// <param name="bWorldUpdate">	ワールド行列を更新するか (デフォルトで true) </param>
	void SetLocalScaleY(float y, bool bWorldUpdate = true);

	/// <summary>
	/// ローカル行列のスケール( z のみ)をセット
	/// </summary>
	/// <param name="z">			セットする z スケール </param>
	/// <param name="bWorldUpdate">	ワールド行列を更新するか (デフォルトで true) </param>
	void SetLocalScaleZ(float z, bool bWorldUpdate = true);

	/// <summary>
	/// ローカル行列のスケールに加算
	/// </summary>
	/// <param name="scale">		加算するスケール </param>
	/// <param name="bWorldUpdate">	ワールド行列を更新するか (デフォルトで true) </param>
	void AddLocalScale(const D3DXVECTOR3* const scale, bool bWorldUpdate = true);

	/// <summary>
	/// ローカル行列のスケールに加算
	/// </summary>
	/// <param name="(x, y, z)">	加算するスケール </param>
	/// <param name="bWorldUpdate">	ワールド行列を更新するか (デフォルトで true) </param>
	void AddLocalScale(float x, float y, float z, bool bWorldUpdate = true);


public:
	/***** direction *****/

	// z direction ( world )
	D3DXVECTOR3 GetForwardVector() const;

	// y direction ( world )
	D3DXVECTOR3 GetUpVector() const;

	// x direction ( world )
	D3DXVECTOR3 GetRightVector() const;

	// z direction ( local )
	D3DXVECTOR3 GetLocalForwardVector() const;

	// y direction ( local )
	D3DXVECTOR3 GetLocalUpVector() const;

	// x direction ( local )
	D3DXVECTOR3 GetLocalRightVector() const;

public:
	/****** matrix updater *****/

	// ワールド行列を更新
	void UpdateWorldMatrix(bool bCallEventUpdated = true);

	// ローカル行列を更新
	void UpdateLocalMatrix(bool bCallEventUpdated = true);


protected:
	// worldMatrix を直接変更したら、UpdateLocalMatrix() も呼ぶこと
	D3DXMATRIX worldMatrix;

	// localMatrix を直接変更したら、UpdateWorldMatrix() も呼ぶこと
	D3DXMATRIX localMatrix;


private:

	// 子供たち
	std::vector<Transform*> childrenTransforms;

	// 親
	Transform* parentTransform;


private:

	// 行列が更新されたときに呼ばれる
	virtual void EventTransformUpdated() {};

};