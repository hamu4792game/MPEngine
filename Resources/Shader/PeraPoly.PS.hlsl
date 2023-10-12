#include "peraPolygon.hlsli"
Texture2D<float4> tex : register(t0); // 通常テクスチャ
SamplerState smp : register(s0); // サンプラー


//  ピクセルシェーダー
float4 main(Output input) : SV_Target{
    return tex.Sample(smp,input.uv);
}