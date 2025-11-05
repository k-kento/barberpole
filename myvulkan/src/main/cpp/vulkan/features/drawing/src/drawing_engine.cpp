#include "drawing_engine.hpp"
#include "drawing_renderer.hpp"

DrawingEngine::DrawingEngine(VulkanContext &vkContext,
                             std::shared_ptr<Surface> surface) : Engine() {


    postTask([this,
                     &vkContext,
                     surface]() {

        std::vector<std::unique_ptr<FrameContext>> frameContexts;
        auto viewBounds = ViewBounds::fromSize(surface->getWidth(), surface->getHeight());
        for (uint32_t i = 0; i < SurfaceContext::MAX_FRAMES_IN_FLIGHT; ++i) {
            frameContexts.push_back(std::unique_ptr<FrameContext>(new FrameContext(vkContext)));
        }

        auto surfaceContext = std::make_unique<SurfaceContext>(vkContext,
                                                               surface,
                                                               std::move(frameContexts));

        auto renderer = std::make_unique<DrawingRenderer>(vkContext,
                                                          std::move(surfaceContext));

        setRenderer(std::move(renderer));
    });
}
