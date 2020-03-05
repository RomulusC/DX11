struct SVOut 
{
	float4 color : Color;
	float4 pos : SV_Position;
};


SVOut main(float3 pos : Position, float4 color : Color)
{
	SVOut sv;
	sv.color = color;
	sv.pos = float4(pos, 1.0f);

	return sv;
}