#include <metal_stdlib>
#include <simd/simd.h>
using namespace metal;
struct Uniforms {
    float a;
    float b;
    float c;
    float4 d;
    float4 e;
    float4 f;
};
struct Inputs {
};
struct Outputs {
    float4 sk_FragColor [[color(0)]];
};
fragment Outputs fragmentMain(Inputs _in [[stage_in]], constant Uniforms& _uniforms [[buffer(0)]], bool _frontFacing [[front_facing]], float4 _fragCoord [[position]]) {
    Outputs _out;
    (void)_out;
    _out.sk_FragColor.x = ((((_uniforms.c) * (_uniforms.b) < 0) ? 1 : -1) * (_uniforms.a));
    _out.sk_FragColor = faceforward(_uniforms.d, _uniforms.e, _uniforms.f);
    return _out;
}
