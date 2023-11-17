// DO NOT MODIFY! This file is autogenerated by gn_to_bp.py.
// If need to change a define, modify SkUserConfigManual.h
#pragma once
#include "SkUserConfigManual.h"

#ifndef GRAPHITE_TEST_UTILS
#define GRAPHITE_TEST_UTILS
#endif

#ifndef SKCMS_DISABLE_HSW
#define SKCMS_DISABLE_HSW
#endif

#ifndef SKCMS_DISABLE_SKX
#define SKCMS_DISABLE_SKX
#endif

#ifndef SKSL_ENABLE_TRACING
#define SKSL_ENABLE_TRACING
#endif

#ifndef SK_ALLOW_STATIC_GLOBAL_INITIALIZERS
#define SK_ALLOW_STATIC_GLOBAL_INITIALIZERS 1
#endif

#ifndef SK_BUILD_FOR_SKQP
#define SK_BUILD_FOR_SKQP
#endif

#ifndef SK_CODEC_DECODES_JPEG
#define SK_CODEC_DECODES_JPEG
#endif

#ifndef SK_CODEC_DECODES_PNG
#define SK_CODEC_DECODES_PNG
#endif

#ifndef SK_CODEC_DECODES_WEBP
#define SK_CODEC_DECODES_WEBP
#endif

#ifndef SK_DEFAULT_TYPEFACE_IS_EMPTY
#define SK_DEFAULT_TYPEFACE_IS_EMPTY
#endif

#ifndef SK_DISABLE_LEGACY_DEFAULT_TYPEFACE
#define SK_DISABLE_LEGACY_DEFAULT_TYPEFACE
#endif

#ifndef SK_ENABLE_DISCRETE_GPU
#define SK_ENABLE_DISCRETE_GPU
#endif

#ifndef SK_ENABLE_DUMP_GPU
#define SK_ENABLE_DUMP_GPU
#endif

#ifndef SK_ENABLE_PRECOMPILE
#define SK_ENABLE_PRECOMPILE
#endif

#ifndef SK_GAMMA_APPLY_TO_A8
#define SK_GAMMA_APPLY_TO_A8
#endif

#ifndef SK_GAMMA_CONTRAST
#define SK_GAMMA_CONTRAST 0.0
#endif

#ifndef SK_GAMMA_EXPONENT
#define SK_GAMMA_EXPONENT 1.4
#endif

#ifndef SK_GANESH
#define SK_GANESH
#endif

#ifndef SK_GL
#define SK_GL
#endif

#ifndef SK_GRAPHITE
#define SK_GRAPHITE
#endif

#ifndef SK_HAS_WUFFS_LIBRARY
#define SK_HAS_WUFFS_LIBRARY
#endif

#ifndef SK_PDF_USE_HARFBUZZ_SUBSET
#define SK_PDF_USE_HARFBUZZ_SUBSET
#endif

#ifndef SK_TOOLS_REQUIRE_RESOURCES
#define SK_TOOLS_REQUIRE_RESOURCES
#endif

#ifndef SK_USE_VMA
#define SK_USE_VMA
#endif

#ifndef SK_VULKAN
#define SK_VULKAN
#endif

#ifndef SK_XML
#define SK_XML
#endif

#ifndef SK_BUILD_FOR_ANDROID
    #error "SK_BUILD_FOR_ANDROID must be defined!"
#endif
#if defined(SK_BUILD_FOR_IOS) || defined(SK_BUILD_FOR_MAC) || \
    defined(SK_BUILD_FOR_UNIX) || defined(SK_BUILD_FOR_WIN)
    #error "Only SK_BUILD_FOR_ANDROID should be defined!"
#endif

#undef SK_BUILD_FOR_ANDROID_FRAMEWORK
