#include "render_pass.h"
#include "log.h"
#include <stdexcept>

RenderPass::RenderPass(VkDevice device) : mDevice(device) {
}

RenderPass::~RenderPass() {
    destroy();
}

RenderPass::RenderPass(RenderPass &&other) noexcept
        : mDevice(other.mDevice), mRenderPass(other.mRenderPass) {
    other.mRenderPass = VK_NULL_HANDLE;
}

RenderPass &RenderPass::operator=(RenderPass &&other) noexcept {
    if (this != &other) {
        destroy();
        mDevice = other.mDevice;
        mRenderPass = other.mRenderPass;
        other.mRenderPass = VK_NULL_HANDLE;
    }
    return *this;
}

bool RenderPass::init(VkFormat colorFormat) {
    /* カラーバッファアタッチメントを作成 */
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = colorFormat;                               // 画面に出力する画像のフォーマット
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;                    // サンプリング回数（MSAAなし）
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;               // サブパス開始時にクリアする
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;             // サブパス終了時に結果をメモリに書き戻す
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;    // ステンシルは無視
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;  // ステンシルは無視
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;          // レンダーパス開始時のレイアウト
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;      // レンダーパス終了後に画面表示用に移行

    VkAttachmentReference colorRef{};
    colorRef.attachment = 0;
    colorRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    /* Subpass の設定 */
    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;    // グラフィックスパイプラインで使用
    subpass.colorAttachmentCount = 1;                               // カラーアタッチメントは1つ
    subpass.pColorAttachments = &colorRef;

    VkRenderPassCreateInfo rpInfo{};
    rpInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    rpInfo.attachmentCount = 1;
    rpInfo.pAttachments = &colorAttachment;
    rpInfo.subpassCount = 1;
    rpInfo.pSubpasses = &subpass;

    auto result = vkCreateRenderPass(mDevice, &rpInfo, nullptr, &mRenderPass);
    if (result != VK_SUCCESS) {
        LOGE("Failed to init render pass");
        return false;
    }
    return true;
}

void RenderPass::destroy() {
    if (mRenderPass != VK_NULL_HANDLE) {
        vkDestroyRenderPass(mDevice, mRenderPass, nullptr);
        mRenderPass = VK_NULL_HANDLE;
    }
}
