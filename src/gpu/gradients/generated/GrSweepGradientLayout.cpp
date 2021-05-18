/*
 * Copyright 2018 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

/**************************************************************************************************
 *** This file was autogenerated from GrSweepGradientLayout.fp; do not modify.
 **************************************************************************************************/
#include "GrSweepGradientLayout.h"

#include "src/core/SkUtils.h"
#include "src/gpu/GrTexture.h"
#include "src/gpu/glsl/GrGLSLFragmentProcessor.h"
#include "src/gpu/glsl/GrGLSLFragmentShaderBuilder.h"
#include "src/gpu/glsl/GrGLSLProgramBuilder.h"
#include "src/sksl/SkSLCPP.h"
#include "src/sksl/SkSLUtil.h"
class GrGLSLSweepGradientLayout : public GrGLSLFragmentProcessor {
public:
    GrGLSLSweepGradientLayout() {}
    void emitCode(EmitArgs& args) override {
        GrGLSLFPFragmentBuilder* fragBuilder = args.fFragBuilder;
        const GrSweepGradientLayout& _outer = args.fFp.cast<GrSweepGradientLayout>();
        (void)_outer;
        auto bias = _outer.bias;
        (void)bias;
        auto scale = _outer.scale;
        (void)scale;
        biasVar = args.fUniformHandler->addUniform(
                &_outer, kFragment_GrShaderFlag, kHalf_GrSLType, "bias");
        scaleVar = args.fUniformHandler->addUniform(
                &_outer, kFragment_GrShaderFlag, kHalf_GrSLType, "scale");
        fragBuilder->codeAppendf(
                R"SkSL(half angle = sk_Caps.atan2ImplementedAsAtanYOverX ? half(2.0 * atan(-%s.y, length(%s) - %s.x)) : half(atan(-%s.y, -%s.x));
half t = ((angle * 0.15915493667125702 + 0.5) + %s) * %s;
return half4(t, 1.0, 0.0, 0.0);
)SkSL",
                args.fSampleCoord,
                args.fSampleCoord,
                args.fSampleCoord,
                args.fSampleCoord,
                args.fSampleCoord,
                args.fUniformHandler->getUniformCStr(biasVar),
                args.fUniformHandler->getUniformCStr(scaleVar));
    }

private:
    void onSetData(const GrGLSLProgramDataManager& pdman,
                   const GrFragmentProcessor& _proc) override {
        const GrSweepGradientLayout& _outer = _proc.cast<GrSweepGradientLayout>();
        {
            pdman.set1f(biasVar, (_outer.bias));
            pdman.set1f(scaleVar, (_outer.scale));
        }
    }
    UniformHandle biasVar;
    UniformHandle scaleVar;
};
std::unique_ptr<GrGLSLFragmentProcessor> GrSweepGradientLayout::onMakeProgramImpl() const {
    return std::make_unique<GrGLSLSweepGradientLayout>();
}
void GrSweepGradientLayout::onGetGLSLProcessorKey(const GrShaderCaps& caps,
                                                  GrProcessorKeyBuilder* b) const {}
bool GrSweepGradientLayout::onIsEqual(const GrFragmentProcessor& other) const {
    const GrSweepGradientLayout& that = other.cast<GrSweepGradientLayout>();
    (void)that;
    if (bias != that.bias) return false;
    if (scale != that.scale) return false;
    return true;
}
GrSweepGradientLayout::GrSweepGradientLayout(const GrSweepGradientLayout& src)
        : INHERITED(kGrSweepGradientLayout_ClassID, src.optimizationFlags())
        , bias(src.bias)
        , scale(src.scale) {
    this->cloneAndRegisterAllChildProcessors(src);
    this->setUsesSampleCoordsDirectly();
}
std::unique_ptr<GrFragmentProcessor> GrSweepGradientLayout::clone() const {
    return std::make_unique<GrSweepGradientLayout>(*this);
}
#if GR_TEST_UTILS
SkString GrSweepGradientLayout::onDumpInfo() const {
    return SkStringPrintf("(bias=%f, scale=%f)", bias, scale);
}
#endif
GR_DEFINE_FRAGMENT_PROCESSOR_TEST(GrSweepGradientLayout);
#if GR_TEST_UTILS
std::unique_ptr<GrFragmentProcessor> GrSweepGradientLayout::TestCreate(GrProcessorTestData* d) {
    SkScalar scale = GrGradientShader::RandomParams::kGradientScale;
    SkPoint center;
    center.fX = d->fRandom->nextRangeScalar(0.0f, scale);
    center.fY = d->fRandom->nextRangeScalar(0.0f, scale);

    GrGradientShader::RandomParams params(d->fRandom);
    auto shader = params.fUseColors4f ? SkGradientShader::MakeSweep(center.fX,
                                                                    center.fY,
                                                                    params.fColors4f,
                                                                    params.fColorSpace,
                                                                    params.fStops,
                                                                    params.fColorCount)
                                      : SkGradientShader::MakeSweep(center.fX,
                                                                    center.fY,
                                                                    params.fColors,
                                                                    params.fStops,
                                                                    params.fColorCount);
    GrTest::TestAsFPArgs asFPArgs(d);
    std::unique_ptr<GrFragmentProcessor> fp = as_SB(shader)->asFragmentProcessor(asFPArgs.args());
    SkASSERT_RELEASE(fp);
    return fp;
}
#endif

std::unique_ptr<GrFragmentProcessor> GrSweepGradientLayout::Make(const SkSweepGradient& grad,
                                                                 const GrFPArgs& args) {
    SkMatrix matrix;
    if (!grad.totalLocalMatrix(args.fPreLocalMatrix)->invert(&matrix)) {
        return nullptr;
    }
    matrix.postConcat(grad.getGradientMatrix());
    return GrMatrixEffect::Make(matrix,
                                std::unique_ptr<GrFragmentProcessor>(new GrSweepGradientLayout(
                                        grad.getTBias(), grad.getTScale())));
}
