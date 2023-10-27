#pragma once
#include "TriggerBase.h"


// ���� :
class Crate : public TriggerBase
{
public:
	// constrcuter destructer
	Crate();
	~Crate();

	// delete Function
	Crate(const Crate& _Other) = delete;
	Crate(Crate&& _Other) noexcept = delete;
	Crate& operator=(const Crate& _Other) = delete;
	Crate& operator=(Crate&& _Other) noexcept = delete;


protected:
	void Start() override;
	void Update(float _DeltaTime) override;

	void InitComponent() override;
	void InitAnimation();

private:
	void SetFSMFUNC();

	std::shared_ptr<class ContentFBXRenderer> m_pRenderer1 = nullptr;
	std::shared_ptr<class ContentFBXRenderer> m_pRenderer2 = nullptr;
	std::shared_ptr<class ContentFBXRenderer> m_pRenderer3 = nullptr;
};
