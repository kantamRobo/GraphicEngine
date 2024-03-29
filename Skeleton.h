#pragma once
#include <list>
#include <functional>
#include <vector>
#include <memory>
#include <WinUser.h>
class Bone {
public:
	//コンストラクタ
	Bone(const wchar_t* boneName,
		const Matrix& bindPose,
		const Matrix& invBindPose,
		int parentBoneNo,
		int boneId) :
		m_boneName(boneName),
		m_bindPose(bindPose),
		m_invBindPose(invBindPose),
		m_parentBoneNo(parentBoneNo),
		m_worldMatrix(bindPose),
		m_boneId(boneId)
	{

	}
		
	void SetLocalMatrix(const Matrix& m)
	{
		m_localMatrix = m;
	}

	//ローカル行列を取得
	const Matrix& GetLocalMatrix()const
	{
		return m_localMatrix;

	}
	//ワールド行列を取得
	const Matrix& GetWorldMatrix()const
	{
		return m_worldMatrix;
	}

	//バインドポーズの行列を取得
	const Matrix& GetBindPoseMatrix()const
	{
		return m_bindPose;
	}

	//バインドポーズの逆行列を取得
	const Matrix& GetInvBindPoseMatrix()const
	{
		return m_invBindPose;

	}

	//親のボーン番号を取得
	int GetParentBoneNo()const
	{
		return m_parentBoneNo;

	}

	//ボーン番号を取得
	int GetNo()const
	{
		return m_boneId;
	}

	//子供を追加

	void AddChild(Bone* bone)
	{
		m_children.push_back(bone);

	}

	//子供を取得
	std::list<Bone*>& GetChildren()
	{
		return m_children;
	}

	const Matrix& GetOffsetLocalMatrix()const
	{
		return m_offsetLocalMatrix;
	}


	bool InitSkeleton(const char* tksFilePath)
	{
		//tksファイルをロードする。
		if (m_tksFile.Load(tksFilePath)) {
			//ボーン行列を構築する。
			BuildBoneMatrices();
			return true;
		}
		return false;
	}


/*!
	*@brief	名前の取得。
	*/
const wchar_t* GetName() const
{
	return m_boneName.c_str();
}
/*!
*@brief	このボーンのワールド空間での位置と回転とスケールを計算する。
*@param[out]	trans		平行移動量の格納先。
*@param[out]	rot			回転量の格納先。
*@param[out]	scale		拡大率の格納先。
*/
void CalcWorldTRS(Vector3& trans, Quaternion& rot, Vector3& scale);
private:
	std::wstring	m_boneName;
	int		     m_parentBoneNo = -1;
	int			 m_boneId = -1;
	Matrix       m_bindPose;
	Matrix       m_invBindPose;
	Matrix       m_localMatrix;
	Matrix       m_worldMatrix;
	Matrix       m_offsetMatrix;
	Matrix			m_offsetLocalMatrix;
	Vector3      m_position;
	Vector3      m_scale;
	Quaternion   m_rotation;
	std::list<Bone*> m_children;
};

class Skeleton
{
public:
	using OnPostProcessSkeletonUpdate = std::function<void()>;

	Skeleton();
	~Skeleton();

	//ボーンのローカル行列を設定
	void SetBoneLocalMatrix(int boneNo, const Matrix& m)
	{
		if (boneNo > (int)m_bones.size() - 1)
		{
			MessageBoxA(nullptr, "boneNo is over m_bones.size() ", "error", MB_OK);
			return;
		}
		m_bones[boneNo]->SetLocalMatrix(m);
	}

	//ボーンの数を取得
	int GetNumBones()const
	{
		return static_cast<int>(m_bones.size());
	}

	//初期化

	bool Init(const char* FilePath);

	bool IsInited()const
	{
		return m_isInited;
	}

	//ボーン行列の構築
	void BuildBoneMatrices();

	//ボーンの名前からボーン番号を検索

	int  FindBoneID(const wchar_t* boneName)const
	{
		for (int i = 0; i < static_cast<int>(m_bones.size()); i++)
		{
			if (wcscmp(m_bones[i]->GetName(), boneName) == 0) {
				return i;
			}
		}
		//見つからなかった
		return -1;
	}

	/// <summary>
	/// ボーン番号からボーンを取得。
	/// </summary>
	/// <param name="boneNo">ボーン番号</param>
	/// <returns>ボーン</returns>
	Bone* GetBone(int boneNo) const
	{
		return m_bones[boneNo].get();
	}

	/// <summary>
	/// ボーン行列の先頭アドレスを取得。
	/// </summary>
	/// <returns></returns>
	Matrix* GetBoneMatricesTopAddress() const
	{
		return m_boneMatrixs.get();
	}

	/// <summary>
	/// アニメーションが再生されているマークを付ける。
	/// </summary>
	void SetMarkPlayAnimation()
	{
		m_isPlayAnimation = true;
	}

public:
	//更新
	void Update(const Matrix& mWorld);

	//ボーンのワールド行列の更新関数

	static void UpdateBoneWorldMatrix(Bone& bone, const Matrix& parentMatrix);

	private:
		static const int BONE_MAX = 512;
		using BonePtr = std::unique_ptr<Bone>;
		std::vector<BonePtr> m_bones;
		std::unique_ptr<Matrix[]>	m_boneMatrixs;
		bool m_isInited = false;
		bool m_isPlayAnimation = false;
	};

}