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

        auto surfaceContext = std::make_unique<SurfaceContext>(vkContext, surface);

        auto renderer = std::make_unique<KaleidoscopeRenderer>(vkContext,
                                                               surface->getWidth(),
                                                               surface->getHeight(),
                                                               deviceRotationDegree,
                                                               texturePath,
                                                               std::move(surfaceContext));

        setRenderer(std::move(renderer));
    });
}
