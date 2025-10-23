package com.gastornisapp.myvulkan.kaleidoscope

import android.annotation.SuppressLint
import android.content.Context
import android.view.Surface
import android.view.SurfaceHolder
import android.view.SurfaceView
import com.gastornisapp.myvulkan.VulkanContext

@SuppressLint("ViewConstructor")
class KaleidoscopeView(context: Context, val vulkanContext: VulkanContext) : SurfaceView(context),
    SurfaceHolder.Callback {

    private var kaleidoscopeRenderer: KaleidoscopeRenderer? = null

    var rotationState: RotationState = RotationState.None
        set(value) {
            kaleidoscopeRenderer?.setRorationState(value)
        }

    var imagePath: KaleidoscopeImage? = null
        set(value) {
            if (field != value) {
                field = value
                value?.let { img -> kaleidoscopeRenderer?.setImage(img) }
            }
        }

    init {
        holder.addCallback(this)
    }

    override fun surfaceCreated(holder: SurfaceHolder) {
        val rotationDegree = rotationToDegrees(context.display.rotation)
        kaleidoscopeRenderer = KaleidoscopeRenderer()
        kaleidoscopeRenderer?.init(context = context.applicationContext, vulkanContext = vulkanContext, surface = holder.surface, rotationDegree)
        kaleidoscopeRenderer?.start()
    }

    override fun surfaceChanged(holder: SurfaceHolder, format: Int, width: Int, height: Int) {
        // TODO
    }

    override fun surfaceDestroyed(holder: SurfaceHolder) {
        kaleidoscopeRenderer?.stop()
        kaleidoscopeRenderer?.destroy()
    }

    private fun rotationToDegrees(rotation: Int): Int {
        return when(rotation) {
            Surface.ROTATION_0 -> 0
            Surface.ROTATION_90 -> 90
            Surface.ROTATION_180 -> 180
            Surface.ROTATION_270 -> 270
            else -> 0 // 万一のため
        }
    }
}
