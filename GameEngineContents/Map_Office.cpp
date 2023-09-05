#include "PrecompileHeader.h"
#include "Map_Office.h"

#include "PhysXTriangleComponent.h"
#include "PhysXBoxComponent.h"
#include "OfficeLevel.h"

Map_Office::Map_Office()
{
}

Map_Office::~Map_Office()
{
}

void Map_Office::Start()
{
	// ������Ʈ �ʱ�ȭ 
	InitComponent();
	Set_StaticObject();
	Set_ActiveObject();
}

void Map_Office::Update(float _DeltaTime)
{
	
}

void Map_Office::InitComponent()
{
	// �� ������ 
	m_pRenderer = CreateComponent<GameEngineFBXRenderer>();
	m_pRenderer->SetFBXMesh("Map_Office.fbx", "MeshTexture");
	m_pRenderer->GetTransform()->SetLocalRotation(m_MapRot);

	// �׺�޽� ��ġȮ�ο� ������ 
	m_pNaviRenderer = CreateComponent<GameEngineFBXRenderer>();
	m_pNaviRenderer->SetFBXMesh("Map_Office_Navi.fbx", "MeshTexture");
	m_pNaviRenderer->GetTransform()->SetLocalRotation(m_MapRot);
	m_pNaviRenderer->GetTransform()->SetLocalPosition(m_MapPos);

	// float4 UnitScale = m_pRenderer->GetFBXMesh()->GetRenderUnit(0)->BoundScaleBox;
	// float4 scale = m_pRenderer->GetMeshScale();
	// physx::PxVec3 vscale = UnitScale.PhysXVec3Return();

	// ������Ʈ 
	m_pTriangleComp = CreateComponent<PhysXTriangleComponent>();
	m_pTriangleComp->SetObjectGround();
	m_pTriangleComp->SetPhysxMaterial(0.f, 0.f, 0.f);
	m_pTriangleComp->CreatePhysXActors("Map_Office_Navi.fbx", true);
	m_pTriangleComp->GetStatic()->setGlobalPose(float4::PhysXTransformReturn(m_MapRot, m_MapPos));
}

void Map_Office::Set_StaticObject()
{
	
}

void Map_Office::Set_ActiveObject()
{
}
