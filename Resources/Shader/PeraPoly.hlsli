Texture2D<float4> tex : register(t0); // 通常テクスチャ
SamplerState smp : register(s0); // サンプラー

struct Output {
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
}