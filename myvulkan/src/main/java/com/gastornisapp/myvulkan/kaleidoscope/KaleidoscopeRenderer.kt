package com.gastornisapp.myvulkan.kaleidoscope

import android.view.Surface
import com.gastornisapp.myvulkan.VulkanContext

class KaleidoscopeRenderer {

    private var nativeHandle: Long = 0L

    fun init(context: VulkanContext, surface: Surface) {
        if (nativeHandle == 0L) {
            nativeHandle = nativeInit(surface = surface, contextHandle = context.nativeHandle)
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
            nativeSetRotationState(nativeHandle,rotationState.value)
        }
    }

    private external fun nativeInit(surface: Surface, contextHandle: Long): Long
    private external fun nativeStart(nativeHandle: Long)
    private external fun nativeStop(nativeHandle: Long)
    private external fun nativeDestroy(nativeHandle: Long)
    private external fun nativeSetRotationState(nativeHandle: Long, rotationState: Int)
}
