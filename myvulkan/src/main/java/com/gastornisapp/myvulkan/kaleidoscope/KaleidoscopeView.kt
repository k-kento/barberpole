package com.gastornisapp.myvulkan.kaleidoscope

import android.annotation.SuppressLint
import android.content.Context
import android.view.SurfaceHolder
import android.view.SurfaceView
import com.gastornisapp.myvulkan.VulkanContext

@SuppressLint("ViewConstructor")
class KaleidoscopeView(context: Context, val vulkanContext: VulkanContext) : SurfaceView(context),
    SurfaceHolder.Callback {

    private lateinit var kaleidoscopeRenderer: KaleidoscopeRenderer

    var rotationState: RotationState = RotationState.None
        set(value) {
            if (::kaleidoscopeRenderer.isInitialized) {
                kaleidoscopeRenderer.setRorationState(value)
            }
        }

    init {
        holder.addCallback(this)
    }

    override fun surfaceCreated(holder: SurfaceHolder) {
        kaleidoscopeRenderer = KaleidoscopeRenderer()
        kaleidoscopeRenderer.init(context = vulkanContext, surface = holder.surface)
        kaleidoscopeRenderer.start()
    }

    override fun surfaceChanged(holder: SurfaceHolder, format: Int, width: Int, height: Int) {
        // TODO
    }

    override fun surfaceDestroyed(holder: SurfaceHolder) {
        kaleidoscopeRenderer.stop()
        kaleidoscopeRenderer.destroy()
    }
}
