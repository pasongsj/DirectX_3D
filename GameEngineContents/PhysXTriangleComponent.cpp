#include "PrecompileHeader.h"
#include "PhysXTriangleComponent.h"

//#include "GameEngineLevel.h"
#include <GameEngineCore/GameEngineFBXMesh.h>

PhysXTriangleComponent::PhysXTriangleComponent()
{
}

PhysXTriangleComponent::~PhysXTriangleComponent()
{
}

//_MeshName(불러올 매시의 이름), _Scene, _Physics, _Cooking (InitPhysics 에서 불러온 값), _InverseIndex(Index를 역순으로 할지에 대한 bool값), 
// _GeoMetryScale(스케일값), _GeoMetryRot(로테이션값)
void PhysXTriangleComponent::CreatePhysXActors(const std::string& _MeshName,bool _InverseIndex, float _Ratio /*= 1.f*/, float4 _GeoMetryRot /*= { 0.0f, 0.0f }*/)
{
	m_pPhysics = GetPhysics();
	m_pScene = GetScene();
	m_pCooking = GetCooking();


	CustomFBXLoad(_MeshName, _Ratio, _InverseIndex);
	float4 tmpQuat = _GeoMetryRot.EulerDegToQuaternion();

	// 부모 액터로부터 위치 생성
	physx::PxTransform localTm
	(
		physx::PxVec3
		(
			ParentActor.lock()->GetTransform()->GetWorldPosition().x,
			ParentActor.lock()->GetTransform()->GetWorldPosition().y,
			ParentActor.lock()->GetTransform()->GetWorldPosition().z
		),
		physx::PxQuat
		(
			tmpQuat.x,
			tmpQuat.y,
			tmpQuat.z,
			tmpQuat.w
		)
	);

	// 마찰, 탄성계수
	m_pMaterial = m_pPhysics->createMaterial(m_fStaticFriction, m_fDynamicFriction, m_fResitution);

	// 충돌체의 종류
	m_pRigidStatic = m_pPhysics->createRigidStatic(localTm);

	int RenderinfoCount = static_cast<int>(Mesh->GetRenderUnitCount());

	for (size_t i = 0; i < RenderinfoCount; i++)
	{
		//Vertex의 값을 Desc에 넣어준다
		//Size, 단위의 바이트사이즈, 데이터의 시작값
		physx::PxTriangleMeshDesc meshDesc;
		meshDesc.points.count = static_cast<physx::PxU32>(VertexVec[i].size());
		meshDesc.points.stride = sizeof(physx::PxVec3);
		meshDesc.points.data = &VertexVec[i][0];

		unsigned int IndexVecSize = static_cast<unsigned int>(IndexVec[i].size() / 3);

		//Index의 값을 Desc에 넣어준다
		//Triangle의 갯수(Index의 1/3개수), 단위의 바이트사이즈, 데이터의 시작값
		meshDesc.triangles.count = IndexVecSize;
		meshDesc.triangles.stride = 3 * sizeof(physx::PxU32);
		meshDesc.triangles.data = &IndexVec[i][0];

		physx::PxDefaultMemoryOutputStream writeBuffer;
		physx::PxTriangleMeshCookingResult::Enum* result = nullptr;
		bool status = m_pCooking->cookTriangleMesh(meshDesc, writeBuffer, result);
		if (!status)
		{
			MsgAssert("매쉬를 불러와 피직스X 충돌체를 만드는데 실패했습니다 TriMesh");
		}

		physx::PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
		physx::PxTriangleMesh* TriangleMesh = m_pPhysics->createTriangleMesh(readBuffer);
		//createExclusiveShapefh RigidStatic에 Shape를 넣어준다.
		m_pShape = physx::PxRigidActorExt::createExclusiveShape(*m_pRigidStatic, physx::PxTriangleMeshGeometry(TriangleMesh), *m_pMaterial);
		//피벗 설정
		physx::PxVec3 Pivot(m_f4DynamicPivot.x, m_f4DynamicPivot.y, m_f4DynamicPivot.z);
		m_pShape->setLocalPose(physx::PxTransform(Pivot));

		// invalid parameter : PxShape::setFlag(s): triangle mesh and heightfield triggers are not supported!
		// Triangle에서는 Trigger를 사용할 수 없음
		//shape_->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);

		//충돌할때 필요한 필터 데이터
			//충돌할때 필요한 필터 데이터
		if (true == m_bObstacle)
		{
			m_pShape->setSimulationFilterData
			(
				physx::PxFilterData
				(
					static_cast<physx::PxU32>(PhysXFilterGroup::Obstacle),
					static_cast<physx::PxU32>(PhysXFilterGroup::PlayerDynamic),
					0,
					0
				)
			);
		}
		else if (true == m_bGround)
		{
			m_pShape->setSimulationFilterData
			(
				physx::PxFilterData
				(
					static_cast<physx::PxU32>(PhysXFilterGroup::Ground),
					static_cast<physx::PxU32>(PhysXFilterGroup::PlayerDynamic),
					0,
					0
				)
			);
		}
		/*else if (true == IsWall)
		{
			m_pShape->setSimulationFilterData
			(
				physx::PxFilterData
				(
					static_cast<physx::PxU32>(PhysXFilterGroup::Wall),
					static_cast<physx::PxU32>(PhysXFilterGroup::Player),
					0,
					0
				)
			);
		}
		else if (true == IsSlope)
		{
			m_pShape->setSimulationFilterData
			(
				physx::PxFilterData
				(
					static_cast<physx::PxU32>(PhysXFilterGroup::Slope),
					static_cast<physx::PxU32>(PhysXFilterGroup::Player),
					0,
					0
				)
			);
		}*/
		else
		{
			std::string ParentName = GetActor()->GetName().data();
			MsgTextBox("충돌 플래그가 설정되지 않았습니다. " + ParentName);
		}
	}


	m_pShape->userData = GetActor();
	// Scene에 액터 추가
	if (true == m_bAggregateObj)
	{
		AddActorAggregate(m_pRigidStatic);
	}
	else
	{
		m_pScene->addActor(*m_pRigidStatic);
	}

}

void PhysXTriangleComponent::Start()
{
	// 부모의 정보의 저장
	ParentActor = GetActor()->DynamicThis<GameEngineActor>();
}

void PhysXTriangleComponent::Update(float _DeltaTime)
{
	if (true == PositionSetFromParentFlag)
	{
		float4 tmpQuat = ParentActor.lock()->GetTransform()->GetWorldRotation().EulerDegToQuaternion();

		physx::PxTransform tmpPxTransform
		(
			ParentActor.lock()->GetTransform()->GetWorldPosition().x,
			ParentActor.lock()->GetTransform()->GetWorldPosition().y,
			ParentActor.lock()->GetTransform()->GetWorldPosition().z,
			physx::PxQuat
			(
				tmpQuat.x,
				tmpQuat.y,
				tmpQuat.z,
				tmpQuat.w
			)
		);

		// 부모의 Transform정보를 바탕으로 PhysX Actor의 트랜스폼을 갱신
		m_pRigidStatic->setGlobalPose(tmpPxTransform);
		// TODO::회전도 처리해야함. DegreeToQuat
	}
}

void PhysXTriangleComponent::CustomFBXLoad(const std::string& _MeshName, float _Ratio, bool _InverseIndex)
{
	//매쉬를 찾는다
	std::shared_ptr<GameEngineFBXMesh> FindFBXMesh = GameEngineFBXMesh::Find(_MeshName);
	if (FindFBXMesh == nullptr)
	{
		MsgAssert("해당 매쉬는 존재하지 않습니다. " + _MeshName);
	}

	Mesh = FindFBXMesh;

	//랜더유닛카운트를 불러와 백터에 reserve를 한다
	size_t RenderinfoCount = Mesh->GetRenderUnitCount();

	VertexVec.reserve(RenderinfoCount + 1);
	IndexVec.reserve(RenderinfoCount + 1);

	for (size_t i = 0; i < RenderinfoCount; i++)
	{
		//i 번째 GetRenderUnit에서 RenderUnitInfo를 Get한다
		FbxRenderUnitInfo* RenderUnitInfo = Mesh->GetRenderUnit(i);

		std::vector<GameEngineVertex> MeshVertexs = RenderUnitInfo->Vertexs;
		std::vector<unsigned int> Indexes = RenderUnitInfo->Indexs[0];

		size_t VertexSize = MeshVertexs.size();
		size_t IndexSize = Indexes.size();
		std::vector<physx::PxVec3> InstVertVec;
		std::vector<unsigned int> InstIndexVec;
		//Vertex와 Index 정보를 VertexVec, IndexVec에 저장한다
		for (size_t j = 0; j < VertexSize; j++)
		{
			InstVertVec.push_back(physx::PxVec3(MeshVertexs[j].POSITION.x, MeshVertexs[j].POSITION.y, MeshVertexs[j].POSITION.z) * _Ratio);
		}

		if (_InverseIndex == true)
		{
			for (size_t j = 0; j < IndexSize; j++)
			{
				InstIndexVec.push_back(physx::PxU32(Indexes[j]));
			}
		}
		if (_InverseIndex == false)
		{
			for (int j = static_cast<int>(IndexSize) - 1; j >= 0; --j)
			{
				InstIndexVec.push_back(physx::PxU32(Indexes[j]));
			}
		}

		VertexVec.push_back(InstVertVec);
		IndexVec.push_back(InstIndexVec);
	}
}
