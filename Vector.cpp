/*!
 * @brief	ベクトルクラス。
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
*@brief	行列からクォータニオンを作成。
*/
void EngineMath::Quaternion::SetRotation(const EngineMath::Matrix& m)
{
	DirectX::XMStoreFloat4(&vec, DirectX::XMQuaternionRotationMatrix(m));
}
/*!
*@brief	fromベクトルからtoベクトルに回転させるクォータニオンを作成。
*/
void EngineMath::Quaternion::SetRotation(EngineMath::Vector3 from, EngineMath::Vector3 to)
{
	from.Normalize();
	to.Normalize();
	auto t = VectorDot(from, to);
	EngineMath::Vector3 rotAxis;
	if (t > 0.998f) {
		//ほぼ同じ向きなので単位クォータニオンにする。
		*this = Quaternion::Identity;
	}
	else if (t < -0.998f) {
		//ほぼ逆向きなので、
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
