#pragma once
#include "PhysXLevel.h"

// ���� :
class MapTestLevel : public PhysXLevel
{
public:
	// constrcuter destructer
	MapTestLevel();
	~MapTestLevel();

	// delete Function
	MapTestLevel(const MapTestLevel& _Other) = delete;
	MapTestLevel(MapTestLevel&& _Other) noexcept = delete;
	MapTestLevel& operator=(const MapTestLevel& _Other) = delete;
	MapTestLevel& operator=(MapTestLevel&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

	void LevelChangeStart() override;
	void LevelChangeEnd() override;

private:

};