
struct VS_IN
{
    float4 pos : POSITION0;
    
};
 
struct VS_OUT
{
    float4 pos : SV_POSITION;
    
};
 
cbuffer ConstantBuffer
{
    float4x4 World; //ワールド変換行列
    float4x4 View; //ビュー変換行列
    float4x4 Projection; //透視射影変換行列
}
 
VS_OUT vs_main(VS_IN input)
{
    VS_OUT output;
 
    output.pos = mul(input.pos, World);
    output.pos = mul(output.pos, View);
    output.pos = mul(output.pos, Projection);
  
    return output;
}