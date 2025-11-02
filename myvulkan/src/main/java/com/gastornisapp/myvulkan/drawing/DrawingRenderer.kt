package com.gastornisapp.myvulkan.drawing

import android.view.Surface
import com.gastornisapp.myvulkan.VulkanContext

class DrawingRenderer {

    private var nativeHandle: Long = 0L

    fun init(vulkanContext: VulkanContext, surface: Surface, deviceRotationDegree: Int) {
        if (nativeHandle == 0L) {
            nativeHandle = nativeInit(
                surface = surface,
                contextHandle = vulkanContext.nativeHandle,
                deviceRotationDegree = deviceRotationDegree,
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

    fun notifyTouchMoveEvent(x: Float, y: Float) {
        if (nativeHandle != 0L) {
            nativeNotifyTouchMoveEvent(nativeHandle, x, y)
        }
    }

    fun notifyTouchUpEvent() {
        if (nativeHandle != 0L) {
            nativeNotifyTouchUpEvent(nativeHandle)
        }
    }

    private external fun nativeInit(surface: Surface, contextHandle: Long, deviceRotationDegree: Int): Long
    private external fun nativeStart(nativeHandle: Long)
    private external fun nativeStop(nativeHandle: Long)
    private external fun nativeDestroy(nativeHandle: Long)
    private external fun nativeNotifyTouchMoveEvent(nativeHandle: Long, x: Float, y: Float)
    private external fun nativeNotifyTouchUpEvent(nativeHandle: Long)
}
