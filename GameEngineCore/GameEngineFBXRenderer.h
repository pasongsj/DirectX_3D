#pragma once
#include "GameEngineRenderer.h"
#include "GameEngineFBXMesh.h"


class FbxExAniData;
class GameEngineFBXMesh;
class GameEngineFBXAnimation;
class GameEngineFBXRenderer;
class GameEngineFBXAnimationInfo : public std::enable_shared_from_this<GameEngineFBXAnimationInfo>
{
public:
	GameEngineFBXRenderer* ParentRenderer;
	// SetFBX 본을 가지고 있는 매쉬
	std::shared_ptr<GameEngineFBXMesh> Mesh;
	// 애니메이션을 가지고 있는 FBX
	std::shared_ptr<GameEngineFBXAnimation> Aniamtion;
	FbxExAniData* FBXAnimationData;

	std::vector<float> FrameTime;
	float PlayTime = 0.0f;
	float CurFrameTime = 0.0f;
	float Inter = 0.1f;

	std::vector<unsigned int> Frames;
	UINT CurFrame = 0;
	UINT Start = -1;
	UINT End = -1;

	bool bOnceStart = true;
	bool bOnceEnd = true;
	bool Loop = true;
	bool EndValue = false;

	// Event

	void Init(std::shared_ptr<GameEngineFBXMesh> _Mesh, std::shared_ptr<GameEngineFBXAnimation> _Animation, const std::string_view& _Name, int _Index);
	void Reset();
	bool IsEnd();
	void Update(float _DeltaTime);

public:
	GameEngineFBXAnimationInfo()
		: Start(0)
		, End(0)
		, bOnceStart(false)
		, bOnceEnd(false)
	{
	}

	~GameEngineFBXAnimationInfo()
	{
	}
};

class AnimationCreateParams
{
public:
	float Inter = 0.0f;
	bool Loop = true;
	int Start = -1;
	int End = -1;
	std::vector<float> FrameTime = std::vector<float>();
};


struct AnimationBoneData
{
public:
	int Index = -1;
	float4 Scale;
	float4 RotQuaternion;
	float4 Pos;
	float4 RotEuler;
};

// 설명 :
class GameEngineFBXRenderer : public GameEngineRenderer
{
	friend class GameEngineFBXAnimationInfo;

public:
	// constrcuter destructer
	GameEngineFBXRenderer();
	~GameEngineFBXRenderer();

	// delete Function
	GameEngineFBXRenderer(const GameEngineFBXRenderer& _Other) = delete;
	GameEngineFBXRenderer(GameEngineFBXRenderer&& _Other) noexcept = delete;
	GameEngineFBXRenderer& operator=(const GameEngineFBXRenderer& _Other) = delete;
	GameEngineFBXRenderer& operator=(GameEngineFBXRenderer&& _Other) noexcept = delete;

	virtual void SetFBXMesh(const std::string& _Name, std::string _Material);
	virtual void SetFBXMesh(const std::string& _Name, std::string _Material, size_t MeshIndex);
	virtual std::shared_ptr<GameEngineRenderUnit> SetFBXMesh(const std::string& _Name, std::string _Material, size_t MeshIndex, size_t _SubSetIndex);


	bool IsAnimationEnd()
	{
		if (nullptr == CurAnimation)
		{
			MsgAssert("CurAnimation이 nullptr입니다");
		}
		return CurAnimation->IsEnd();
	}

	std::map<std::pair<size_t, size_t>, std::shared_ptr<GameEngineRenderUnit>> GetUnTexturedUnit()
	{
		return UnTexturedUnit;
	}
	inline std::shared_ptr<GameEngineFBXMesh> GetFBXMesh()
	{
		return FBXMesh;
	}

	void CreateFBXAnimation(const std::string& _AnimationFBXName, const AnimationCreateParams& _Params, int _Index = 0)
	{
		CreateFBXAnimation(_AnimationFBXName, _AnimationFBXName, _Params, _Index);
	}

	void CreateFBXAnimation(const std::string& _AnimationName, const std::string& _AnimationFBXName, const AnimationCreateParams& _Params = {0.05f,true}, int _Index = 0);

	std::vector<std::vector<std::shared_ptr<GameEngineRenderUnit>>>& GetAllRenderUnit()
	{
		return Unit;
	}

	void Update(float _DeltaTime) override;

	void PauseSwtich();

	inline void PauseOn()
	{
		Pause = true;
	}
	inline void PauseOff()
	{
		Pause = false;
	}

	void ChangeAnimation(const std::string& _AnimationName, bool _Force = false);

	void CalculateUnitPos()
	{
		float4 f4MinPos = float4::ZERO;
		float4 f4MaxPos = float4::ZERO;
		float4 f4Scale = float4::ZERO;
		float4 ResultPos = float4::ZERO;
		float4 Quat = Quat.EulerDegToQuaternion();
		float4x4 RenderUnitMat = float4x4::Zero;
		for (size_t i = 0; i < Unit.size(); i++)
		{
			f4MinPos = FBXMesh->GetRenderUnit(i)->MinBoundBox;
			f4MaxPos = FBXMesh->GetRenderUnit(i)->MaxBoundBox;
			f4Scale = FBXMesh->GetRenderUnit(i)->BoundScaleBox;
			ResultPos = (f4MinPos + f4MaxPos) * 0.5f;

			RenderUnitMat.Compose(f4Scale, Quat, ResultPos);
			RenderUnitMat *= GetTransform()->GetWorldMatrixRef();
			//RenderUnitMat *= GetTransform()->GetLocalWorldMatrixRef();

			float4 Pos = float4(RenderUnitMat._30, RenderUnitMat._31, RenderUnitMat._32, RenderUnitMat._33);

			for (size_t j = 0; j < Unit[i].size(); j++)
			{
				Unit[i][j]->SetUnitPos(Pos);
				Unit[i][j]->SetUnitScale(f4Scale);
			}
		}
	}

	float4 GetMeshScale()
	{
		if (ResultMeshScale != float4::ZERO)
		{
			return ResultMeshScale;
		}
		float4 f4MinBox = float4::ZERO;
		float4 f4MaxBox = float4::ZERO;
		float4 ResultBox = float4::ZERO;
		for (size_t i = 0; i < FBXMesh->GetRenderUnitCount(); i++)
		{
			float4 f4TempMinBox = float4::ZERO;
			float4 f4TempMaxBox = float4::ZERO;
			f4TempMinBox = FBXMesh->GetRenderUnit(i)->MinBoundBox;
			f4TempMaxBox = FBXMesh->GetRenderUnit(i)->MaxBoundBox;
			if (f4MinBox.x > f4TempMinBox.x)
			{
				f4MinBox.x = f4TempMinBox.x;
			}
			if (f4MinBox.y > f4TempMinBox.y)
			{
				f4MinBox.y = f4TempMinBox.y;
			}
			if (f4MinBox.z > f4TempMinBox.z)
			{
				f4MinBox.z = f4TempMinBox.z;
			}

			if (f4MaxBox.x < f4TempMaxBox.x)
			{
				f4MaxBox.x = f4TempMaxBox.x;
			}
			if (f4MaxBox.y < f4TempMaxBox.y)
			{
				f4MaxBox.y = f4TempMaxBox.y;
			}
			if (f4MaxBox.z < f4TempMaxBox.z)
			{
				f4MaxBox.z = f4TempMaxBox.z;
			}
		}
		ResultBox.x = f4MaxBox.x - f4MinBox.x;
		ResultBox.y = f4MaxBox.y - f4MinBox.y;
		ResultBox.z = f4MaxBox.z - f4MinBox.z;

		ResultMeshScale = ResultBox;

		return ResultBox;
	}
protected:
	// void Render(float _DeltaTime) override;

private:
	bool Pause = false;

	// 그게 불가능하다.
	// 맨처음 세팅해준 메인 매쉬와완전히 연관되어 있는 매쉬여야만 가능하다.
	float4 ResultMeshScale = float4::ZERO;
	std::shared_ptr<GameEngineFBXMesh> FBXMesh;
	std::map<std::pair<size_t, size_t>, std::shared_ptr<GameEngineRenderUnit>> UnTexturedUnit;

	// vertexbuffer1       indexbuffer          
	// 0번매쉬의            0번째 서브셋
	std::vector<         std::vector<std::shared_ptr<GameEngineRenderUnit>>> Unit;

	std::map<std::string, std::shared_ptr<GameEngineFBXAnimationInfo>> Animations;
	std::shared_ptr<GameEngineFBXAnimationInfo> CurAnimation = nullptr;

	// 처음에는 그냥 들고만 있다가
	// 애니메이션이 되는순간
	// 확장되어야 한다.
	// 본개수 만큼
	// 앞쪽에 키는 매쉬 개수 x 본개수의 뜻을 가지게 된다.

	// Structure Buffer랑 링크가 되는 녀석.
	std::vector<float4x4> AnimationBoneMatrixs;


	std::vector<AnimationBoneData> AnimationBoneDatas;;
};

