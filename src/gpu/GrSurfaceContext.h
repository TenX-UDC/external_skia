/*
 * Copyright 2016 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef GrSurfaceContext_DEFINED
#define GrSurfaceContext_DEFINED

#include "include/core/SkFilterQuality.h"
#include "include/core/SkImage.h"
#include "include/core/SkRect.h"
#include "include/core/SkRefCnt.h"
#include "include/core/SkSurface.h"
#include "src/gpu/GrClientMappedBufferManager.h"
#include "src/gpu/GrColorInfo.h"
#include "src/gpu/GrDataUtils.h"
#include "src/gpu/GrImageInfo.h"
#include "src/gpu/GrSurfaceProxy.h"
#include "src/gpu/GrSurfaceProxyView.h"

class GrAuditTrail;
class GrDrawingManager;
class GrRecordingContext;
class GrRenderTargetContext;
class GrRenderTargetProxy;
class GrSingleOwner;
class GrSurface;
class GrSurfaceProxy;
class GrTextureProxy;
struct SkIPoint;
struct SkIRect;

/**
 * A helper object to orchestrate commands for a particular surface
 */
class GrSurfaceContext {
public:
    // If the passed in GrSurfaceProxy is renderable this will return a GrRenderTargetContext,
    // otherwise it will return a GrSurfaceContext.
    static std::unique_ptr<GrSurfaceContext> Make(GrRecordingContext*,
                                                  GrSurfaceProxyView readView,
                                                  GrColorType, SkAlphaType, sk_sp<SkColorSpace>);

    static std::unique_ptr<GrSurfaceContext> Make(GrRecordingContext*, SkISize dimensions,
                                                  const GrBackendFormat&, GrRenderable,
                                                  int renderTargetSampleCnt, GrMipmapped,
                                                  GrProtected, GrSurfaceOrigin, GrColorType,
                                                  SkAlphaType, sk_sp<SkColorSpace>, SkBackingFit,
                                                  SkBudgeted);

    // If it is known that the GrSurfaceProxy is not renderable, you can directly call the the ctor
    // here to make a GrSurfaceContext on the stack.
    GrSurfaceContext(GrRecordingContext*, GrSurfaceProxyView readView, GrColorType, SkAlphaType,
                     sk_sp<SkColorSpace>);

    virtual ~GrSurfaceContext() = default;

    GrRecordingContext* recordingContext() { return fContext; }

    const GrColorInfo& colorInfo() const { return fColorInfo; }
    GrImageInfo imageInfo() const { return {fColorInfo, fReadView.proxy()->dimensions()}; }

    GrSurfaceOrigin origin() const { return fReadView.origin(); }
    GrSwizzle readSwizzle() const { return fReadView.swizzle(); }
    // TODO: See if it makes sense for this to return a const& instead and require the callers to
    // make a copy (which refs the proxy) if needed.
    GrSurfaceProxyView readSurfaceView() { return fReadView; }

    SkISize dimensions() const { return fReadView.dimensions(); }
    int width() const { return fReadView.proxy()->width(); }
    int height() const { return fReadView.proxy()->height(); }

    const GrCaps* caps() const;

    /**
     * Reads a rectangle of pixels from the render target context.
     * @param dContext      The direct context to use
     * @param dstInfo       image info for the destination
     * @param dst           destination pixels for the read
     * @param rowBytes      bytes in a row of 'dst'
     * @param srcPt         offset w/in the surface context from which to read
     *                      is a GrDirectContext and fail otherwise.
     */
    bool readPixels(GrDirectContext* dContext,
                    const GrImageInfo& dstInfo,
                    void* dst,
                    size_t rowBytes,
                    SkIPoint srcPt);

    using ReadPixelsCallback = SkImage::ReadPixelsCallback;
    using ReadPixelsContext  = SkImage::ReadPixelsContext;
    using RescaleGamma       = SkImage::RescaleGamma;

    // GPU implementation for SkImage:: and SkSurface::asyncRescaleAndReadPixels.
    void asyncRescaleAndReadPixels(GrDirectContext*,
                                   const SkImageInfo& info,
                                   const SkIRect& srcRect,
                                   RescaleGamma rescaleGamma,
                                   SkFilterQuality rescaleQuality,
                                   ReadPixelsCallback callback,
                                   ReadPixelsContext callbackContext);

    // GPU implementation for SkImage:: and SkSurface::asyncRescaleAndReadPixelsYUV420.
    void asyncRescaleAndReadPixelsYUV420(GrDirectContext*,
                                         SkYUVColorSpace yuvColorSpace,
                                         sk_sp<SkColorSpace> dstColorSpace,
                                         const SkIRect& srcRect,
                                         SkISize dstSize,
                                         RescaleGamma rescaleGamma,
                                         SkFilterQuality rescaleQuality,
                                         ReadPixelsCallback callback,
                                         ReadPixelsContext context);

    /**
     * Writes a rectangle of pixels [srcInfo, srcBuffer, srcRowbytes] into the
     * renderTargetContext at the specified position.
     * @param dContext      The direct context to use
     * @param srcInfo       image info for the source pixels
     * @param src           source for the write
     * @param rowBytes      bytes in a row of 'src'
     * @param dstPt         offset w/in the surface context at which to write
     */
    bool writePixels(GrDirectContext* dContext,
                     const GrImageInfo& srcInfo,
                     const void* src,
                     size_t rowBytes,
                     SkIPoint dstPt);

    GrSurfaceProxy* asSurfaceProxy() { return fReadView.proxy(); }
    const GrSurfaceProxy* asSurfaceProxy() const { return fReadView.proxy(); }
    sk_sp<GrSurfaceProxy> asSurfaceProxyRef() { return fReadView.refProxy(); }

    GrTextureProxy* asTextureProxy() { return fReadView.asTextureProxy(); }
    const GrTextureProxy* asTextureProxy() const { return fReadView.asTextureProxy(); }
    sk_sp<GrTextureProxy> asTextureProxyRef() { return fReadView.asTextureProxyRef(); }

    GrRenderTargetProxy* asRenderTargetProxy() { return fReadView.asRenderTargetProxy(); }
    const GrRenderTargetProxy* asRenderTargetProxy() const {
        return fReadView.asRenderTargetProxy();
    }
    sk_sp<GrRenderTargetProxy> asRenderTargetProxyRef() {
        return fReadView.asRenderTargetProxyRef();
    }

    virtual GrRenderTargetContext* asRenderTargetContext() { return nullptr; }

    /**
     * Rescales the contents of srcRect. The gamma in which the rescaling occurs is controlled by
     * RescaleGamma. It is always in the original gamut. The result is converted to the color type
     * and color space of info after rescaling. Note: this currently requires that the info have a
     * different size than srcRect. Though, it could be relaxed to allow non-scaling color
     * conversions.
     */
    std::unique_ptr<GrRenderTargetContext> rescale(const GrImageInfo& info,
                                                   GrSurfaceOrigin,
                                                   SkIRect srcRect,
                                                   SkImage::RescaleGamma,
                                                   SkFilterQuality);

    GrAuditTrail* auditTrail();

#if GR_TEST_UTILS
    bool testCopy(GrSurfaceProxy* src, const SkIRect& srcRect, const SkIPoint& dstPoint) {
        return this->copy(src, srcRect, dstPoint);
    }

    bool testCopy(GrSurfaceProxy* src) {
        return this->copy(src, SkIRect::MakeSize(src->dimensions()), {0, 0});
    }
#endif

protected:
    GrDrawingManager* drawingManager();
    const GrDrawingManager* drawingManager() const;

    SkDEBUGCODE(void validate() const;)

    SkDEBUGCODE(GrSingleOwner* singleOwner() const;)

    GrRecordingContext* fContext;

    GrSurfaceProxyView fReadView;

    // Inserts a transfer, part of the implementation of asyncReadPixels and
    // asyncRescaleAndReadPixelsYUV420().
    struct PixelTransferResult {
        using ConversionFn = void(void* dst, const void* mappedBuffer);
        // If null then the transfer could not be performed. Otherwise this buffer will contain
        // the pixel data when the transfer is complete.
        sk_sp<GrGpuBuffer> fTransferBuffer;
        // If this is null then the transfer buffer will contain the data in the requested
        // color type. Otherwise, when the transfer is done this must be called to convert
        // from the transfer buffer's color type to the requested color type.
        std::function<ConversionFn> fPixelConverter;
    };
    PixelTransferResult transferPixels(GrColorType colorType, const SkIRect& rect);

    // The async read step of asyncRescaleAndReadPixels()
    void asyncReadPixels(GrDirectContext*,
                         const SkIRect& srcRect,
                         SkColorType,
                         ReadPixelsCallback,
                         ReadPixelsContext);

private:
    friend class GrSurfaceProxy; // for copy

    SkDEBUGCODE(virtual void onValidate() const {})

    /**
     * Copy 'src' into the proxy backing this context. This call will not do any draw fallback.
     * Currently only writePixels and replaceRenderTarget call this directly. All other copies
     * should go through GrSurfaceProxy::Copy.
     * @param src       src of pixels
     * @param dstPoint  the origin of the 'srcRect' in the destination coordinate space
     * @return          true if the copy succeeded; false otherwise
     *
     * Note: Notionally, 'srcRect' is clipped to 'src's extent with 'dstPoint' being adjusted.
     *       Then the 'srcRect' offset by 'dstPoint' is clipped against the dst's extent.
     *       The end result is only valid src pixels and dst pixels will be touched but the copied
     *       regions will not be shifted. The 'src' must have the same origin as the backing proxy
     *       of fSurfaceContext.
     */
    bool copy(GrSurfaceProxy* src, const SkIRect& srcRect, const SkIPoint& dstPoint);

    class AsyncReadResult;

    GrColorInfo fColorInfo;

    using INHERITED = SkRefCnt;
};

#endif
