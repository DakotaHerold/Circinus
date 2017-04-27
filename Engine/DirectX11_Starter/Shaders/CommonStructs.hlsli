
struct Input
{
	float3 position		: POSITION;
	float3 normal		: NORMAL;
	float3 tangent		: TANGENT;
	float2 uv			: TEXCOORD;
};

struct V2F
{
	float4 position		: SV_POSITION;
	float3 normal		: NORMAL;
	float3 worldPos		: POSITION;
	float3 tangent		: TANGENT;
	float2 uv			: TEXCOORD;
};

struct VertexShaderInputSkyBox
{
	float3 position		: POSITION;     // XYZ position
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD;
};

struct VertexToPixelSkyBox
{
	float4 position		: SV_POSITION;	// XYZW position (System Value Position)
	float3 uv			: TEXCOORD;
};