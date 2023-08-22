/*!
 * @brief	�x�N�g���N���X�B
 */


#include "Math.h"

#include "stdafx.h"

#include "Vector.h"





const EngineMath::Vector4 EngineMath::Vector4::White = { 1.0f, 1.0f, 1.0f, 1.0f };
const EngineMath::Vector4 EngineMath::Vector4::Black = { 0.0f, 0.0f, 0.0f, 1.0f };
const EngineMath::Vector4 EngineMath::Vector4::Yellow = { 1.0f, 1.0f, 0.0f, 1.0f };
const EngineMath::Vector4 EngineMath::Vector4::Gray = { 0.5f, 0.5f, 0.5f, 1.0f };

const EngineMath::Vector2 EngineMath::Vector2::Zero = { 0.0f,  0.0f };
const EngineMath::Vector3 EngineMath::Vector3::Zero = { 0.0f,  0.0f,  0.0f };
const EngineMath::Vector3 EngineMath::Vector3::Right = { 1.0f,  0.0f,  0.0f };
const EngineMath::Vector3 EngineMath::Vector3::Left = { -1.0f,  0.0f,  0.0f };
const EngineMath::Vector3 EngineMath::Vector3::Up = { 0.0f,  1.0f,  0.0f };
const EngineMath::Vector3 EngineMath::Vector3::Down = { 0.0f, -1.0f,  0.0f };
const EngineMath::Vector3 EngineMath::Vector3::Front = { 0.0f,   0.0f,  1.0f };
const EngineMath::Vector3 EngineMath::Vector3::Back = { 0.0f,   0.0f, -1.0f };

const EngineMath::Vector3 EngineMath::Vector3::AxisX = { 1.0f,  0.0f,  0.0f };
const EngineMath::Vector3 EngineMath::Vector3::AxisY = { 0.0f,  1.0f,  0.0f };
const EngineMath::Vector3 EngineMath::Vector3::AxisZ = { 0.0f,  0.0f,  1.0f };
const EngineMath::Vector3 EngineMath::Vector3::One = { 1.0f, 1.0f, 1.0f };
const EngineMath::Quaternion EngineMath::Quaternion::Identity = { 0.0f,  0.0f, 0.0f, 1.0f };

/*!
*@brief	�s�񂩂�N�H�[�^�j�I�����쐬�B
*/
void EngineMath::Quaternion::SetRotation(const EngineMath::Matrix& m)
{
	DirectX::XMStoreFloat4(&vec, DirectX::XMQuaternionRotationMatrix(m));
}
/*!
*@brief	from�x�N�g������to�x�N�g���ɉ�]������N�H�[�^�j�I�����쐬�B
*/
void EngineMath::Quaternion::SetRotation(EngineMath::Vector3 from, EngineMath::Vector3 to)
{
	from.Normalize();
	to.Normalize();
	auto t = VectorDot(from, to);
	EngineMath::Vector3 rotAxis;
	if (t > 0.998f) {
		//�قړ��������Ȃ̂ŒP�ʃN�H�[�^�j�I���ɂ���B
		*this = Quaternion::Identity;
	}
	else if (t < -0.998f) {
		//�قڋt�����Ȃ̂ŁA
		if (fabsf(to.x) < 1.0f) {
			//
			rotAxis = Cross(EngineMath::Vector3::AxisX, to);
		}
		else {
			rotAxis = Cross(EngineMath::Vector3::AxisY, to);
		}
	}
	else {
		rotAxis = Cross(from, to);
	}
	rotAxis.Normalize();
	SetRotation(rotAxis, acosf(t));
}
