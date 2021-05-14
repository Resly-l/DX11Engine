float4 main(float depth : DEPTH) : SV_TARGET
{
	float mappedDepth = 0.01f / (1.01 - depth);
	return float4(depth, depth, depth, 1.0f);
	return float4(mappedDepth, mappedDepth, mappedDepth, 1.0f);
}