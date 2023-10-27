#include "PrecompileHeader.h"
#include "Frog_Septre.h"

#include "PhysXBoxComponent.h"

Frog_Septre::Frog_Septre() 
{
}

Frog_Septre::~Frog_Septre() 
{
}

void Frog_Septre::Start()
{
	TriggerBase::Start();
	InitAnimation();
	InitComponent();
	SetFSMFUNC();
}

void Frog_Septre::Update(float _DeltaTime)
{
	TriggerBase::Update(_DeltaTime);
}

void Frog_Septre::InitComponent()
{
	float4 MeshScale = m_pRenderer->GetMeshScale();

	m_pPhysXComponent = CreateComponent<PhysXBoxComponent>();
	m_pPhysXComponent->SetPhysxMaterial(0.0f, 0.0f, 0.0f);
	m_pPhysXComponent->CreatePhysXActors(MeshScale.PhysXVec3Return(), float4::ZERONULL, true);
	m_pPhysXComponent->SetFilterData(PhysXFilterGroup::Obstacle);
	m_pPhysXComponent->SetPositionSetFromParentFlag(true);

	MeshScale.y = 1.f;
	m_pPhysXComponent->CreateSubShape(SubShapeType::BOX, MeshScale * 10.f, float4(0, 10, 0));
	m_pPhysXComponent->SetSubShapeFilter(PhysXFilterGroup::LeverTrigger);
	m_pPhysXComponent->AttachShape();
}

void Frog_Septre::InitAnimation()
{
	m_pRenderer = CreateComponent<ContentFBXRenderer>();
	m_pRenderer->SetFBXMesh("FrogSeptre_Static.FBX", "ContentMeshDeffered");



	int a = 0;
}

void Frog_Septre::SetFSMFUNC()
{
	SetChangeFSMCallBack([this]
		{
			//StateDuration = 0.0f;
			//StateChecker = false;
		});

	SetFSM(TriggerState::OFF,
		[this]
		{
		},
		[this](float Delta)
		{
			if (true == TriggerHitCheck())
			{
				SetNextState(TriggerState::PROGRESS);
			};
		},
		[this]
		{
		}
	);

	SetFSM(TriggerState::PROGRESS,
		[this]
		{
			if (nullptr != m_TriggerFunc)
			{
				m_TriggerFunc();
			}
			m_pRenderer->SetGlowToUnit(0, 0, "swampPillarMask.png");
			auto units = m_pRenderer->GetAllRenderUnit();
			m_pRenderer->SetUnitDiffuseColorIntensity(0, 0, 4.0f);
		},
		[this](float Delta)
		{
			if (true)// glow�Ѱ� ����Ʈ ������ �ѱ��
			{
				SetNextState(TriggerState::ON);
			}
		},
		[this]
		{
		}
	);

	SetFSM(TriggerState::ON,
		[this]
		{
		},
		[this](float Delta)
		{
		},
		[this]
		{
		}
	);
}