#pragma once
#include "Component.h"
#include <string>
class TestCom :
	public Component
{
public:
	TestCom();
	~TestCom();
	void Update() override;
};

