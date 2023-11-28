#include "Texture2D.hlsli"

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

cbuffer Iro : register(b1)
{
    float4 color;
};


cbuffer Monochrome : register(b2)
{
    float2 monochromePibot;
    float monochromeRate;
}

ConstantBuffer<DirectionalLight> gDirectionalLight : register(b3);

float4 main(VertexOutput input) : SV_TARGET
{
    float NdotL = dot(normalize(input.normal),-gDirectionalLight.direction);
    float cos = pow(NdotL * 0.5f + 0.5f,2.0f);

    //float cos = saturate(dot(normalize(input.normal),-gDirectionalLight.direction));
    float4 textureColor = gTexture.Sample(gSampler, input.texcoord) * color;
    
    textureColor.rgb = textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;

    return textureColor;
}