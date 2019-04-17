cbuffer TransformBuffer : register(b0)
{
    matrix WORLD;
    matrix VIEW;
    matrix PROJ;
};

struct VertexInput
{
	float4 Position : POSITION0;
	float4 Color : COLOR0;
};

struct PixelInput
{
	//SV = 컴퓨터에 찍을 위치지정
	float4 Position : SV_POSITION0;
	float4 Color : COLOR0;
};

PixelInput VS(VertexInput input)
{
	PixelInput output;
    output.Position = mul(input.Position, WORLD);
    output.Position = mul(output.Position, VIEW);
    output.Position = mul(output.Position, PROJ);
	output.Color = input.Color;

	return output;

}

//OM에들어가는 도화지 총8장중 어디에쓸건지
float4 PS(PixelInput input) :SV_Target
{
	return input.Color;
}