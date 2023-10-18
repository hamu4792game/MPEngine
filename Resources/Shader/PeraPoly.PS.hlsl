#include "peraPolygon.hlsli"
Texture2D<float4> gTexture : register(t0); // 通常テクスチャ
SamplerState gSampler : register(s0); // サンプラー


//  ピクセルシェーダー
float4 main(Output input) : SV_Target{
    float4 textureColor = gTexture.Sample(gSampler, input.uv);

    // 平均化した値
    float Y = (textureColor.x + textureColor.y + textureColor.z) / 3.0f;
    // グレースケール変換
    float4 gray = float4(Y,Y,Y,textureColor.a);
    // レトロビット調
    float4 retoro = float4(textureColor.rgb - fmod(textureColor.rgb, 0.25f),textureColor.a);
    // セピア調 高輝度変換が苦手なフィルタ値 (107.0f,74.0f,43.0f)
    float4 sepia = float4(clamp(Y * (107.0f/107.0f),0.0f,255.0f),clamp(Y * ( 74.0f/107.0f),0.0f,255.0f),
    clamp(Y * ( 43.0f/107.0f),0.0f,255.0f),textureColor.a);
    
    return sepia;
}