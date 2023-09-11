#include "PreCompileHeader.h"
#include "MonsterAnimationTest.h"

MonsterAnimationTest::MonsterAnimationTest()
{
}

MonsterAnimationTest::~MonsterAnimationTest()
{
}

void MonsterAnimationTest::Start()
{
	Renderer = CreateComponent<GameEngineFBXRenderer>();
	Renderer->SetFBXMesh("_E_BAT_Black Variant_MESH.FBX", "MeshAniTexture");
	Renderer->CreateFBXAnimation("IDLE", "_E_BAT_Black Variant_IDLE.fbx");
	Renderer->CreateFBXAnimation("FLY", "_E_BAT_Black Variant_FLY.fbx");
	Renderer->CreateFBXAnimation("BITE", "_E_BAT_Black Variant_BITE.fbx");
	Renderer->CreateFBXAnimation("IDLE_FLOOR", "_E_BAT_Black Variant_IDLE_FLOOR.fbx");
	Renderer->CreateFBXAnimation("SHOCK", "_E_BAT_Black Variant_SHOCK.fbx");
	//Renderer->ChangeAnimation("BITE");
	AnimationName.push_back("IDLE");
	AnimationName.push_back("FLY");
	AnimationName.push_back("BITE");
	AnimationName.push_back("IDLE_FLOOR");
	AnimationName.push_back("SHOCK");
	GetTransform()->SetLocalScale(float4::ONE * 50.0f);
	//GetTransform()->SetLocalRotation({ 0.0f,180.0f,0.0f });

	if (false == GameEngineInput::IsKey("PressN"))
	{
		GameEngineInput::CreateKey("PressN", 'N');
	}

}

void MonsterAnimationTest::Update(float _DeltaTime)
{
	if (true == GameEngineInput::IsDown("PressN"))
	{
		Renderer->ChangeAnimation(AnimationName[index++]);
		if (index >= AnimationName.size())
		{
			index = 0;
		}
	}
}
