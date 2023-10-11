#include "PeraPoly.hlsli"

//  頂点シェーダー
Output main(float4 pos : POSITION,float2 uv : TEXCOORD) {
    Output output;
    output.potision = pos;
    output.uv = uv;
    return output;
}