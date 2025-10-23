package com.gastornisapp.myvulkan.kaleidoscope

import android.content.Context
import android.view.Surface
import com.gastornisapp.myvulkan.VulkanContext

class KaleidoscopeRenderer {

    private var nativeHandle: Long = 0L

    fun init(context: Context, vulkanContext: VulkanContext, surface: Surface) {
        if (nativeHandle == 0L) {
            nativeHandle = nativeInit(
                surface = surface,
                contextHandle = vulkanContext.nativeHandle,
                KaleidoscopeImage.getImages(context).first().getPath()
            )
        }
    }

    fun start() {
        if (nativeHandle != 0L) {
            nativeStart(nativeHandle)
        }
    }

    fun stop() {
        if (nativeHandle != 0L) {
            nativeStop(nativeHandle)
        }
    }

    fun destroy() {
        if (nativeHandle != 0L) {
            nativeDestroy(nativeHandle)
            nativeHandle = 0L
        }
    }

    fun setRorationState(rotationState: RotationState) {
        if (nativeHandle != 0L) {
            nativeSetRotationState(nativeHandle, rotationState.value)
        }
    }

    fun setImage(image: KaleidoscopeImage) {
        if (nativeHandle != 0L) {
            nativeSetImage(nativeHandle, image.getPath())
        }
    }

    private external fun nativeInit(surface: Surface, contextHandle: Long, filePath: String): Long
    private external fun nativeStart(nativeHandle: Long)
    private external fun nativeStop(nativeHandle: Long)
    private external fun nativeDestroy(nativeHandle: Long)
    private external fun nativeSetRotationState(nativeHandle: Long, rotationState: Int)
    private external fun nativeSetImage(nativeHandle: Long, filePath: String)
}
