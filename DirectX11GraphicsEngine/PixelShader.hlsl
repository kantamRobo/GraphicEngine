// 0�Ԃ̃e�N�X�`���X���b�g���g�p����
Texture2D g_texture : register(t0);
// 0�Ԃ̃T���v���X���b�g���g�p����
SamplerState g_sampler : register(s0);

float4 ps_main() : SV_TARGET
{
	 // �e�N�X�`������F���擾
    float4 texColor = g_texture.Sample(g_sampler, float2(0.6, 0.6));
 
    // �e�N�X�`���̐F���o��
    return texColor;
}