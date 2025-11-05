#include "kaleidoscope_engine.hpp"
#include "kaleidoscope_renderer.hpp"
#include "kaleidoscope_frame_context.hpp"


KaleidoscopeEngine::KaleidoscopeEngine(VulkanContext &vkContext,
                                       std::shared_ptr<Surface> surface,
                                       uint32_t deviceRotationDegree,
                                       std::string &texturePath) : Engine() {


    postTask([this,
                     &vkContext,
                     surface,
                     deviceRotationDegree,
                     texturePath = std::move(texturePath)]() {

        std::vector<std::unique_ptr<FrameContext>> frameContexts;
        auto viewBounds = ViewBounds::fromSize(surface->getWidth(), surface->getHeight());
        for (uint32_t i = 0; i < SurfaceContext::MAX_FRAMES_IN_FLIGHT; ++i) {
            frameContexts.push_back(std::unique_ptr<FrameContext>(new KaleidoscopeFrameContext(vkContext, viewBounds)));
        }

        auto surfaceContext = std::make_unique<SurfaceContext>(vkContext,
                                                               surface,
                                                               std::move(frameContexts));

        auto renderer = std::make_unique<KaleidoscopeRenderer>(vkContext,
                                                               surface->getWidth(),
                                                               surface->getHeight(),
                                                               deviceRotationDegree,
                                                               texturePath,
                                                               std::move(surfaceContext));

        setRenderer(std::move(renderer));
    });
}
