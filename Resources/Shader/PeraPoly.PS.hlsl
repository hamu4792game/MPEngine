#include "PeraPoly.hlsli"

//  ピクセルシェーダー
float4 main(Output input) : SV_Target{
    return float4(input.uv,1,1);
}