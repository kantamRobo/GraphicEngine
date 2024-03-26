// 0番のテクスチャスロットを使用する
Texture2D g_texture : register(t0);
// 0番のサンプラスロットを使用する
SamplerState g_sampler : register(s0);

float4 ps_main() : SV_TARGET
{
	 // テクスチャから色を取得
    float4 texColor = g_texture.Sample(g_sampler, float2(0.6, 0.6));
 
    // テクスチャの色を出力
    return texColor;
}