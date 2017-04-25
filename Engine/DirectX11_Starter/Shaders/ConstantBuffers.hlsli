
cbuffer InstanceConstants : register (b0)
{
	matrix	matWorld;
	matrix	matWorld_IT;
}

cbuffer FrameConstants : register (b1)
{
	matrix	matView;
	matrix	matProj;
	float3	camPos;
}

