#include "TestCom.h"




TestCom::TestCom()
{
	type = ComponentType::TestComponent;
}


TestCom::~TestCom()
{
}

void TestCom::Update()
{
	std::cout << type << std::endl;
}
