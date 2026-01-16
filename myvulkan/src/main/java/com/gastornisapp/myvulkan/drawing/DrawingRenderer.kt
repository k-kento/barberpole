package com.gastornisapp.myvulkan.drawing

import android.view.Surface
import com.gastornisapp.myvulkan.VulkanContext

enum class BrushType(val value: Int) {
    Normal(0),
    Rainbow(1),
    Glow(2),
    Star(3),
    Circle(4)
}

class DrawingRenderer {

    private var nativeHandle: Long = 0L

    fun init(vulkanContext: VulkanContext, surface: Surface) {
        if (nativeHandle == 0L) {
            nativeHandle = nativeInit(contextHandle = vulkanContext.nativeHandle, surface = surface)
        }
    }

    fun onSurfaceChanged(surface: Surface, deviceRotationDegree: Int) {
        if (nativeHandle != 0L) {
            nativeOnSurfaceChanged(nativeHandle = nativeHandle, surface = surface, deviceRotationDegree = deviceRotationDegree)
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

    fun setBrushType(brushType: BrushType) {
        if (nativeHandle != 0L) {
            nativeSetBrushType(nativeHandle, brushType.value)
        }
    }

    private external fun nativeInit(surface: Surface, contextHandle: Long): Long
    private external fun nativeOnSurfaceChanged(nativeHandle: Long, surface: Surface, deviceRotationDegree: Int)
    private external fun nativeStart(nativeHandle: Long)
    private external fun nativeStop(nativeHandle: Long)
    private external fun nativeDestroy(nativeHandle: Long)
    private external fun nativeNotifyTouchMoveEvent(nativeHandle: Long, x: Float, y: Float)
    private external fun nativeNotifyTouchUpEvent(nativeHandle: Long)
    private external fun nativeSetBrushType(nativeHandle: Long, brushType: Int)
}
