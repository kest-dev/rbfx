//
// Copyright (c) 2017-2020 the rbfx project.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#pragma once

#include "../Graphics/Drawable.h"
#include "../RenderPipeline/BatchCompositor.h"
#include "../RenderPipeline/BatchRenderer.h"
#include "../RenderPipeline/CameraProcessor.h"
#include "../RenderPipeline/RenderPipelineDefs.h"
#include "../RenderPipeline/DrawableProcessor.h"
#include "../RenderPipeline/InstancingBuffer.h"
#include "../RenderPipeline/LightProcessor.h"
#include "../RenderPipeline/ShadowMapAllocator.h"

namespace Urho3D
{

class DrawCommandQueue;
class RenderPipelineInterface;
class RenderSurface;
class Viewport;

/// Scene processor for RenderPipeline
class URHO3D_API SceneProcessor : public Object, public LightProcessorCallback
{
    URHO3D_OBJECT(SceneProcessor, Object);

public:
    SceneProcessor(RenderPipelineInterface* renderPipeline, const ea::string& shadowTechnique,
        ShadowMapAllocator* shadowMapAllocator, InstancingBuffer* instancingBuffer);
    ~SceneProcessor() override;

    /// Setup. Slow, called rarely.
    /// @{
    void SetPasses(ea::vector<SharedPtr<BatchCompositorPass>> passes);
    void SetSettings(const SceneProcessorSettings& settings);
    /// @}

    /// Called on every frame.
    /// @{
    bool Define(const CommonFrameInfo& frameInfo);
    void SetRenderCamera(Camera* camera);
    void SetRenderCameras(ea::span<Camera* const> cameras);

    void Update();
    void RenderShadowMaps();
    /// @}

    /// Getters
    /// @{
    const FrameInfo& GetFrameInfo() const { return frameInfo_; }
    /// Return whether the pass object in callback corresponds to internal pass.
    bool IsInternalPass(Object* pass) const { return batchCompositor_ == pass; }
    const auto& GetLightVolumeBatches() const { return batchCompositor_->GetLightVolumeBatches(); }
    CameraProcessor* GetCameraProcessor() const { return cameraProcessor_; }
    DrawableProcessor* GetDrawableProcessor() const { return drawableProcessor_; }
    BatchCompositor* GetBatchCompositor() const { return batchCompositor_; }
    BatchRenderer* GetBatchRenderer() const { return batchRenderer_; }
    /// @}

private:
    /// Callbacks from RenderPipeline
    /// @{
    void OnUpdateBegin(const CommonFrameInfo& frameInfo);
    void OnRenderEnd(const CommonFrameInfo& frameInfo);
    /// @}

    /// LightProcessorCallback implementation
    /// @{
    bool IsLightShadowed(Light* light) override;
    ShadowMapRegion AllocateTransientShadowMap(const IntVector2& size) override;
    /// @}

    void DrawOccluders();

    RenderPipelineInterface* renderPipeline_{};

    /// Shared objects
    /// @{
    SharedPtr<ShadowMapAllocator> shadowMapAllocator_;
    SharedPtr<InstancingBuffer> instancingBuffer_;
    SharedPtr<DrawCommandQueue> drawQueue_;
    /// @}

    /// Owned objects
    /// @{
    SharedPtr<CameraProcessor> cameraProcessor_;
    SharedPtr<DrawableProcessor> drawableProcessor_;
    SharedPtr<BatchCompositor> batchCompositor_;
    SharedPtr<BatchRenderer> batchRenderer_;
    SharedPtr<OcclusionBuffer> occlusionBuffer_;
    /// @}

    SceneProcessorSettings settings_;

    FrameInfo frameInfo_;
    bool flipCameraForRendering_{};

    OcclusionBuffer* currentOcclusionBuffer_{};
    ea::vector<Drawable*> occluders_;
    ea::vector<Drawable*> drawables_;

    ea::vector<PipelineBatchByState> tempSortedShadowBatches_;
};

}