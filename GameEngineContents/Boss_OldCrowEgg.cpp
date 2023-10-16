#include "PreCompileHeader.h"

#include "Boss_OldCrowEgg.h"

Boss_OldCrowEgg::Boss_OldCrowEgg() 
{
}

Boss_OldCrowEgg::~Boss_OldCrowEgg() 
{
}

void Boss_OldCrowEgg::Start()
{
	Renderer = CreateComponent<ContentFBXRenderer>();

	Renderer->SetFBXMesh("OldCrow.FBX", "ContentAniMeshDeffered");
}


void Boss_OldCrowEgg::Update(float _DeltaTime)
{

}