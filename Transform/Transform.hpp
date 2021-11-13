#include <vector>
#include <d3dx9.h>
#include "utils.hpp"
#pragma once

struct Rotation
{
	float yaw,
		  pitch,
		  roll;

	// ���
	Rotation& operator = (const Rotation& rh)
	{
		this->yaw   = rh.yaw;
		this->pitch = rh.pitch;
		this->roll  = rh.roll;
		return *this;
	}

	// �Z�p
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

	// ����
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

	//�P��
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

	// ��r
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

	// �L���X�g
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
	///  ��]����x�N�g�����擾 �� : Rotation(0,0,0) => D3DXVECTOR3(1,0,0)
	/// </summary>
	/// <param name="mode"> 0 : Forward(�f�t�H���g)</param>
	/// <param name="mode"> 1 : up</param>
	/// <param name="mode"> 2 : left</param>
	/// <returns> mode �ɉ������x�N�g�� </returns>
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
			vx = round( (-sp*sr*sy + cr*cy) * roundNum ) / roundNum;  //������ log(roundNum) ���Ŏl�̌ܓ�
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
			vx = round( (-sp*sr*sy + cr*cy) * roundNum ) / roundNum;  //������ log(roundNum) ���Ŏl�̌ܓ�
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
	
	// ���g���ʓx�@�ɕϊ�
	Rotation ToRadian()
	{
		float convert = 3.141592654f / 180.0f;

		return Rotation(this->yaw * convert, this->pitch * convert, this->roll * convert);
	}

	// ���g�̃��W�A���l��Ԃ�
	Rotation GetRadian() const
	{
		float convert = 3.141592654f / 180.0f;

		return Rotation(this->yaw * convert, this->pitch * convert, this->roll * convert);
	}

	// ���g��x���@�ɕϊ�
	Rotation ToDegree()
	{
		float convert = 180.0f / 3.141592654f;

		return Rotation(this->yaw *= convert, this->pitch *= convert, this->roll *= convert);
	}

	// ���g�̓x��Ԃ�
	Rotation GetDegree() const
	{
		float convert = 3.141592654f / 180.0f;

		return Rotation(this->yaw * convert, this->pitch * convert, this->roll * convert);
	}
	

	// -PI ~ +PI �͈̔͂ɐ��K��
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

	// ���K�����ꂽRotation��Ԃ�
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
	
	// �N�H�[�^�j�I�� -> �I�C���[�p
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
	/***** �e�q�֘A *****/

	// �e���擾
    Transform* GetParent() const;

	// �e���w�肷�� ( parent = nullptr �Őe�q���� )
	void SetParent(Transform* const parent);

	// �q���������擾
	const std::vector<Transform*>& GetChildren() const;

	// �e�q�ɂȂ� ( parent = nullptr �Őe�q���� )
	void BecomeParents(Transform* const parent);

	// �e�q����������
	void BreakParents();

	// �q��ǉ�
	bool AddChild(Transform* const child);

	// �q������
	bool RemoveChild(Transform* const child);

	// ��c�� ������ancestor �����݂��邩
	bool CheckAncestor(Transform* const ancestor);

	// �e�������Ă邩
	bool HasParent();

	// �q�������Ă邩
	bool HasChild();

public:
	/***** matrix *****/

	// ���[���h�s����擾
	const D3DXMATRIX& GetWorldMatrix()  const;

	// ���[���h�s����Z�b�g�A���[�J���s����X�V
	void SetWorldMatrix(const D3DXMATRIX* const worldMatrix);
	
	// ���[�J���s����擾
	const D3DXMATRIX& GetLocalMatrix()  const;

	// ���[�J���s����Z�b�g�A���[���h�s����X�V
	void SetLocalMatrix(const D3DXMATRIX* const localMatrix);

	// ���[���h��]�s����擾
	D3DXMATRIX GetWorldRotationMatrix() const;

	// ���[�J����]�s����擾
	D3DXMATRIX GetLocalRotationMatrix() const;

	// ���[���h�ϊ��s��𐶐�

	/// <summary>
	/// ���[���h�ϊ��s��𐶐�
	/// </summary>
	/// <param name="location">	���W </param>
	/// <param name="rotation">	�I�C���[�p </param>
	/// <param name="scale">	�X�P�[�� </param>
	/// <returns> ���[���h�ϊ��s�� </returns>
	static D3DXMATRIX CreateWorldTranslationMatrix
	(
		const D3DXVECTOR3* const location,
		const Rotation*    const rotation,
		const D3DXVECTOR3* const scale
	);

	/// <summary>
	/// ���[���h�ϊ��s��𐶐�
	/// </summary>
	/// <param name="location">			���W </param>
	/// <param name="rotationMatrix">	��]�s�� </param>
	/// <param name="scale">			�X�P�[�� </param>
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

	/// ���[���h���W���擾
	D3DXVECTOR3 GetWorldLocation() const;

	/// <summary>
	/// ���[���h���W���Z�b�g
	/// </summary>
	/// <param name="location">		�Z�b�g������W </param>
	/// <param name="bLocalUpdate">	���[�J���s����X�V���邩 (�f�t�H���g�� true) </param>
	void SetWorldLocation(const D3DXVECTOR3* const location, bool bLocalUpdate = true);

	/// <summary>
	/// ���[���h���W���Z�b�g
	/// </summary>
	/// <param name="(x, y, z)">	�Z�b�g������W </param>
	/// <param name="bLocalUpdate">	���[�J���s����X�V���邩 (�f�t�H���g�� true) </param>
	void SetWorldLocation(float x, float y, float z, bool bLocalUpdate = true);

	/// <summary>
	/// ���[���h���W( x �̂�)���Z�b�g
	/// </summary>
	/// <param name="x">			�Z�b�g���� x ���W </param>
	/// <param name="bLocalUpdate">	���[�J���s����X�V���邩 (�f�t�H���g�� true) </param>
	void SetWorldLocationX(float x, bool bLocalUpdate = true);

	/// <summary>
	/// ���[���h���W( y �̂�)���Z�b�g
	/// </summary>
	/// <param name="y">			�Z�b�g���� y ���W </param>
	/// <param name="bLocalUpdate">	���[�J���s����X�V���邩 (�f�t�H���g�� true) </param>
	void SetWorldLocationY(float y, bool bLocalUpdate = true);

	/// <summary>
	/// ���[���h���W( z �̂�)���Z�b�g
	/// </summary>
	/// <param name="z">			�Z�b�g���� z ���W </param>
	/// <param name="bLocalUpdate">	���[�J���s����X�V���邩 (�f�t�H���g�� true) </param>
	void SetWorldLocationZ(float z, bool bLocalUpdate = true);

	/// <summary>
	/// ���[���h���W�ɉ��Z
	/// </summary>
	/// <param name="location">		���Z������W�l </param>
	/// <param name="bLocalUpdate">	���[�J���s����X�V���邩 (�f�t�H���g�� true) </param>
	void AddWorldLocation(const D3DXVECTOR3* const location, bool bLocalUpdate = true);

	/// <summary>
	/// ���[���h���W�ɉ��Z
	/// </summary>
	/// <param name="(x, y, z)">	���Z������W�l </param>
	/// <param name="bLocalUpdate">	���[�J���s����X�V���邩 (�f�t�H���g�� true) </param>
	void AddWorldLocation(float x, float y, float z, bool bLocalUpdate = true);

	//******************************************************************************************
	// local
	//******************************************************************************************

	/// ���[�J�����W���擾
	D3DXVECTOR3 GetLocalLocation() const;

	/// <summary>
	/// ���[�J�����W���Z�b�g
	/// </summary>
	/// <param name="location">		�Z�b�g������W </param>
	/// <param name="bWorldUpdate">	���[���h�s����X�V���邩 (�f�t�H���g�� true) </param>
	void SetLocalLocation(const D3DXVECTOR3* const location, bool bWorldUpdate = true);

	/// <summary>
	/// ���[�J�����W���Z�b�g
	/// </summary>
	/// <param name="(x, y, z)">	�Z�b�g������W </param>
	/// <param name="bWorldUpdate">	���[���h�s����X�V���邩 (�f�t�H���g�� true) </param>
	void SetLocalLocation(float x, float y, float z, bool bWorldUpdate = true);

	/// <summary>
	/// ���[�J�����W( x �̂�)���Z�b�g
	/// </summary>
	/// <param name="x">			�Z�b�g���� x ���W </param>
	/// <param name="bWorldUpdate">	���[���h�s����X�V���邩 (�f�t�H���g�� true) </param>
	void SetLocalLocationX(float x, bool bWorldUpdate = true);

	/// <summary>
	/// ���[�J�����W( y �̂�)���Z�b�g
	/// </summary>
	/// <param name="y">			�Z�b�g���� y ���W </param>
	/// <param name="bWorldUpdate">	���[���h�s����X�V���邩 (�f�t�H���g�� true) </param>
	void SetLocalLocationY(float y, bool bWorldUpdate = true);

	/// <summary>
	/// ���[�J�����W( z �̂�)���Z�b�g
	/// </summary>
	/// <param name="z">			�Z�b�g���� z ���W </param>
	/// <param name="bWorldUpdate">	���[���h�s����X�V���邩 (�f�t�H���g�� true) </param>
	void SetLocalLocationZ(float z, bool bWorldUpdate = true);

	/// <summary>
	/// ���[�J�����W�ɉ��Z
	/// </summary>
	/// <param name="location">		���Z������W�l </param>
	/// <param name="bWorldUpdate">	���[���h�s����X�V���邩 (�f�t�H���g�� true) </param>
	void AddLocalLocation(const D3DXVECTOR3* const location, bool bWorldUpdate = true);

	/// <summary>
	/// ���[�J�����W�ɉ��Z
	/// </summary>
	/// <param name="(x, y, z)">	���Z������W�l </param>
	/// <param name="bWorldUpdate">	���[���h�s����X�V���邩 (�f�t�H���g�� true) </param>
	void AddLocalLocation(float x, float y, float z, bool bWorldUpdate = true);


public:
	/***** rotation, quaternion *****/

	/*** world ***/

	/// ���[���h�s��̃I�C���[�p��]�ʂ��擾 (�덷�L)
	Rotation GetWorldRotation() const;

	/// �N�H�[�^�j�I��(world)���擾
	D3DXQUATERNION GetWorldQuaternion() const;

	/// <summary>
	/// ���[���h�s��̉�]���Z�b�g
	/// </summary>
	/// <param name="rotation">		�Z�b�g�����] </param>
	/// <param name="bLocalUpdate">	���[�J���s����X�V���邩 (�f�t�H���g�� true) </param>
	void SetWorldRotation(const Rotation* const rotation, bool bLocalUpdate = true);

	/// <summary>
	/// ���[���h�s��̉�]���Z�b�g
	/// </summary>
	/// <param name="(yaw, pitch, roll)">	�Z�b�g�����] </param>
	/// <param name="bLocalUpdate">			���[�J���s����X�V���邩 (�f�t�H���g�� true) </param>
	void SetWorldRotation(float yaw, float pitch, float roll, bool bLocalUpdate = true);

	/// <summary>
	/// ���[���h�s��̉�]�ɉ��Z
	/// </summary>
	/// <param name="rotation">		���Z�����]�� </param>
	/// <param name="bLocalUpdate">	���[�J���s����X�V���邩 (�f�t�H���g�� true) </param>
	void AddWorldRotation(const Rotation* const rotation, bool bLocalUpdate = true);

	/// <summary>
	/// ���[���h�s��̉�]�ɉ��Z
	/// </summary>
	/// <param name="(yaw, pitch, roll)">	���Z�����]�� </param>
	/// <param name="bLocalUpdate">			���[�J���s����X�V���邩 (�f�t�H���g�� true) </param>
	void AddWorldRotation(float yaw, float pitch, float roll, bool bLocalUpdate = true);

	/// <summary>
	/// ��]���Ɖ�]�ʂ��烏�[���h�s�����]
	/// </summary>
	/// <param name="(x, y, z)	">	��]�� </param>
	/// <param name="w	">			��]�� </param>
	/// <param name="bLocalUpdate"> ���[�J���s����X�V���邩 (�f�t�H���g�� true)</param>
	void WorldRotateAroundAxis(float x, float y, float z, float w, bool bLocalUpdate = true);

	/// <summary>
	/// ��]���Ɖ�]�ʂ��烏�[���h�s�����]
	/// </summary>
	/// <param name="axis	">		��]�� </param>
	/// <param name="w	">			��]�� </param>
	/// <param name="bLocalUpdate">	���[�J���s����X�V���邩 (�f�t�H���g�� true) </param>
	void WorldRotateAroundAxis(const D3DXVECTOR3* axis, float w, bool bLocalUpdate = true);

	/// <summary>
	/// ���[���h�s��̉�]���N�H�[�^�j�I���ŃZ�b�g
	/// </summary>
	/// <param name="(x, y, z, w)">	�Z�b�g����N�H�[�^�j�I�� </param>
	/// <param name="bLocalUpdate">	���[�J���s����X�V���邩 (�f�t�H���g�� true) </param>
	void SetWorldQuaternion(float x, float y, float z, float w, bool bLocalUpdate = true);

	/// <summary>
	/// ���[���h�s��̉�]���N�H�[�^�j�I���ŃZ�b�g
	/// </summary>
	/// <param name="(axis, w)">	�Z�b�g����N�H�[�^�j�I�� </param>
	/// <param name="bLocalUpdate">	���[�J���s����X�V���邩 (�f�t�H���g�� true) </param>
	void SetWorldQuaternion(const D3DXVECTOR3* axis, float w, bool bLocalUpdate = true);

	/// <summary>
	/// ���[���h�s��̉�]���N�H�[�^�j�I���ŃZ�b�g
	/// </summary>
	/// <param name="quat">			�Z�b�g����N�H�[�^�j�I��</param>
	/// <param name="bLocalUpdate">	���[�J���s����X�V���邩 (�f�t�H���g�� true) </param>
	void SetWorldQuaternion(const D3DXQUATERNION* quat, bool bLocalUpdate = true);

	/*** local ***/
	
	/// ���[�J���s��̃I�C���[�p��]�ʂ��擾 (�덷�L)
	Rotation       GetLocalRotation()   const;

	/// ���[�J���s��̃N�H�[�^�j�I�����擾
	D3DXQUATERNION GetLocalQuaternion() const;

	/// <summary>
	/// ���[�J���s��̉�]���Z�b�g
	/// </summary>
	/// <param name="rotation">		�Z�b�g�����] </param>
	/// <param name="bWorldUpdate">	���[���h�s����X�V���邩 (�f�t�H���g�� true) </param>
	void SetLocalRotation(const Rotation* const rotation, bool bWorldUpdate = true);

	/// <summary>
	/// ���[�J���s��̉�]���Z�b�g
	/// </summary>
	/// <param name="(yaw, pitch, roll)">	�Z�b�g�����] </param>
	/// <param name="bWorldUpdate">			���[���h�s����X�V���邩 (�f�t�H���g�� true) </param>
	void SetLocalRotation(float yaw, float pitch, float roll, bool bWorldUpdate = true);

	/// <summary>
	/// ���[�J���s��̉�]�ɉ��Z
	/// </summary>
	/// <param name="rotation">		���Z�����]�� </param>
	/// <param name="bWorldUpdate">	���[���h�s����X�V���邩 (�f�t�H���g�� true)	</param>
	void AddLocalRotation(const Rotation* const rotation, bool bWorldUpdate = true);

	/// <summary>
	/// ���[�J���s��̉�]�ɉ��Z
	/// </summary>
	/// <param name="(yaw, pitch, roll)">	���Z�����]�� </param>
	/// <param name="bWorldUpdate">			���[���h�s����X�V���邩 (�f�t�H���g�� true) </param>
	void AddLocalRotation(float yaw, float pitch, float roll, bool bWorldUpdate = true);

	/// <summary>
	/// ��]���Ɖ�]�ʂ��烍�[�J���s�����]
	/// </summary>
	/// <param name="(x, y, z)	">	��]�� </param>
	/// <param name="w	">			��]�� </param>
	/// <param name="bWorldUpdate">	���[���h�s����X�V���邩 (�f�t�H���g�� true) </param>
	void LocalRotateAroundAxis(float x, float y, float z, float w, bool bWorldUpdate = true);

	/// <summary>
	/// ��]���Ɖ�]�ʂ��烍�[�J���s�����]
	/// </summary>
	/// <param name="axis	">		��]�� </param>
	/// <param name="w	">			��]�� </param>
	/// <param name="bWorldUpdate">	���[���h�s����X�V���邩 (�f�t�H���g�� true) </param>
	void LocalRotateAroundAxis(const D3DXVECTOR3* axis, float w, bool bWorldUpdate = true);

	/// <summary>
	/// ���[�J���s��̉�]���N�H�[�^�j�I���ŃZ�b�g
	/// </summary>
	/// <param name="(x, y, z, w)">	�Z�b�g����N�H�[�^�j�I�� </param>
	/// <param name="bWorldUpdate">	���[���h�s����X�V���邩 (�f�t�H���g�� true) </param>
	void SetLocalQuaternion(float x, float y, float z, float w, bool bWorldUpdate = true);

	/// <summary>
	/// ���[�J���s��̉�]���N�H�[�^�j�I���ŃZ�b�g
	/// </summary>
	/// <param name="(axis, w)">	�Z�b�g����N�H�[�^�j�I�� </param>
	/// <param name="bWorldUpdate">	���[���h�s����X�V���邩 (�f�t�H���g�� true)</param>
	void SetLocalQuaternion(const D3DXVECTOR3* axis, float w, bool bWorldUpdate = true);

	/// <summary>
	/// ���[�J���s��̉�]���N�H�[�^�j�I���ŃZ�b�g
	/// </summary>
	/// <param name="quat">			�Z�b�g����N�H�[�^�j�I�� </param>
	/// <param name="bWorldUpdate">	���[���h�s����X�V���邩 (�f�t�H���g�� true) </param>
	void SetLocalQuaternion(const D3DXQUATERNION* quat, bool bWorldUpdate = true);


public:
	/***** scale *****/

	/*** world ***/

	/// ���[���h�s��̃X�P�[�����擾
	D3DXVECTOR3 GetWorldScale() const;

	/// <summary>
	/// ���[���h�s��̃X�P�[�����Z�b�g
	/// </summary>
	/// <param name="scale">		�Z�b�g����X�P�[�� </param>
	/// <param name="bLocalUpdate">	���[�J���s����X�V���邩 (�f�t�H���g�� true) </param>
	void SetWorldScale(const D3DXVECTOR3* const scale, bool bLocalUpdate = true);

	/// <summary>
	/// ���[���h�s��̃X�P�[�����Z�b�g
	/// </summary>
	/// <param name="(x, y, z)">	�Z�b�g����X�P�[�� </param>
	/// <param name="bLocalUpdate">	���[�J���s����X�V���邩 (�f�t�H���g�� true) </param>
	void SetWorldScale(float x, float y, float z, bool bLocalUpdate = true);

	/// <summary>
	/// ���[���h�s��̃X�P�[��( x �̂�)���Z�b�g
	/// </summary>
	/// <param name="x">			�Z�b�g���� x �X�P�[�� </param>
	/// <param name="bLocalUpdate">	���[�J���s����X�V���邩 (�f�t�H���g�� true) </param>
	void SetWorldScaleX(float x, bool bLocalUpdate = true);

	/// <summary>
	/// ���[���h�s��̃X�P�[��( y �̂�)���Z�b�g
	/// </summary>
	/// <param name="y">			�Z�b�g���� y �X�P�[�� </param>
	/// <param name="bLocalUpdate">	���[�J���s����X�V���邩 (�f�t�H���g�� true) </param>
	void SetWorldScaleY(float y, bool bLocalUpdate = true);

	/// <summary>
	/// ���[���h�s��̃X�P�[��( z �̂�)���Z�b�g
	/// </summary>
	/// <param name="z">			�Z�b�g���� z �X�P�[�� </param>
	/// <param name="bLocalUpdate">	���[�J���s����X�V���邩 (�f�t�H���g�� true) </param>
	void SetWorldScaleZ(float z, bool bLocalUpdate = true);

	/// <summary>
	/// ���[���h�s��̃X�P�[���ɉ��Z
	/// </summary>
	/// <param name="scale">		���Z����X�P�[�� </param>
	/// <param name="bLocalUpdate">	���[�J���s����X�V���邩 (�f�t�H���g�� true) </param>
	void AddWorldScale(const D3DXVECTOR3* const scale, bool bLocalUpdate = true);

	/// <summary>
	/// ���[���h�s��̃X�P�[���ɉ��Z
	/// </summary>
	/// <param name="(x, y, z)">	���Z����X�P�[�� </param>
	/// <param name="bLocalUpdate">	���[�J���s����X�V���邩 (�f�t�H���g�� true) </param>
	void AddWorldScale(float x, float y, float z, bool bLocalUpdate = true);

	/*** local ***/
	
	/// ���[�J���s��̃X�P�[�����擾
	D3DXVECTOR3 GetLocalScale() const;

	/// <summary>
	/// ���[�J���s��̃X�P�[�����Z�b�g
	/// </summary>
	/// <param name="scale">		�Z�b�g����X�P�[�� </param>
	/// <param name="bWorldUpdate">	���[���h�s����X�V���邩 (�f�t�H���g�� true) </param>
	void SetLocalScale(const D3DXVECTOR3* const scale, bool bWorldUpdate = true);

	/// <summary>
	/// ���[�J���s��̃X�P�[�����Z�b�g
	/// </summary>
	/// <param name="(x, y, z)">	�Z�b�g����X�P�[�� </param>
	/// <param name="bWorldUpdate">	���[���h�s����X�V���邩 (�f�t�H���g�� true) </param>
	void SetLocalScale(float x, float y, float z, bool bWorldUpdate = true);

	/// <summary>
	/// ���[�J���s��̃X�P�[��( x �̂�)���Z�b�g
	/// </summary>
	/// <param name="x">			�Z�b�g���� x �X�P�[�� </param>
	/// <param name="bWorldUpdate">	���[���h�s����X�V���邩 (�f�t�H���g�� true) </param>
	void SetLocalScaleX(float x, bool bWorldUpdate = true);

	/// <summary>
	/// ���[�J���s��̃X�P�[��( y �̂�)���Z�b�g
	/// </summary>
	/// <param name="y">			�Z�b�g���� y �X�P�[�� </param>
	/// <param name="bWorldUpdate">	���[���h�s����X�V���邩 (�f�t�H���g�� true) </param>
	void SetLocalScaleY(float y, bool bWorldUpdate = true);

	/// <summary>
	/// ���[�J���s��̃X�P�[��( z �̂�)���Z�b�g
	/// </summary>
	/// <param name="z">			�Z�b�g���� z �X�P�[�� </param>
	/// <param name="bWorldUpdate">	���[���h�s����X�V���邩 (�f�t�H���g�� true) </param>
	void SetLocalScaleZ(float z, bool bWorldUpdate = true);

	/// <summary>
	/// ���[�J���s��̃X�P�[���ɉ��Z
	/// </summary>
	/// <param name="scale">		���Z����X�P�[�� </param>
	/// <param name="bWorldUpdate">	���[���h�s����X�V���邩 (�f�t�H���g�� true) </param>
	void AddLocalScale(const D3DXVECTOR3* const scale, bool bWorldUpdate = true);

	/// <summary>
	/// ���[�J���s��̃X�P�[���ɉ��Z
	/// </summary>
	/// <param name="(x, y, z)">	���Z����X�P�[�� </param>
	/// <param name="bWorldUpdate">	���[���h�s����X�V���邩 (�f�t�H���g�� true) </param>
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

	// ���[���h�s����X�V
	void UpdateWorldMatrix(bool bCallEventUpdated = true);

	// ���[�J���s����X�V
	void UpdateLocalMatrix(bool bCallEventUpdated = true);


protected:
	// worldMatrix �𒼐ڕύX������AUpdateLocalMatrix() ���ĂԂ���
	D3DXMATRIX worldMatrix;

	// localMatrix �𒼐ڕύX������AUpdateWorldMatrix() ���ĂԂ���
	D3DXMATRIX localMatrix;


private:

	// �q������
	std::vector<Transform*> childrenTransforms;

	// �e
	Transform* parentTransform;


private:

	// �s�񂪍X�V���ꂽ�Ƃ��ɌĂ΂��
	virtual void EventTransformUpdated() {};

};