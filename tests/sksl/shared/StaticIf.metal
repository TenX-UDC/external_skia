#include <metal_stdlib>
#include <simd/simd.h>
using namespace metal;
struct Uniforms {
    float4 colorRed;
    float4 colorGreen;
};
struct Inputs {
};
struct Outputs {
    float4 sk_FragColor [[color(0)]];
};


fragment Outputs fragmentMain(Inputs _in [[stage_in]], constant Uniforms& _uniforms [[buffer(0)]], bool _frontFacing [[front_facing]], float4 _fragCoord [[position]]) {
    Outputs _out;
    (void)_out;
    float4 result = _uniforms.colorRed;
    {
        result = _uniforms.colorGreen;
    }
    _out.sk_FragColor = result;
    return _out;
}
