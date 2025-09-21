package com.gastornisapp.myvulkan.kaleidoscope

import android.content.Context
import android.view.SurfaceHolder
import android.view.SurfaceView
import com.gastornisapp.myvulkan.Renderer
import com.gastornisapp.myvulkan.VulkanContext

class KaleidoscopeView(context: Context, val vulkanContext: VulkanContext) : SurfaceView(context),
    SurfaceHolder.Callback {

    private lateinit var renderer: Renderer

    init {
        holder.addCallback(this)
    }

    override fun surfaceCreated(holder: SurfaceHolder) {
        renderer = Renderer()
        renderer.init(context = vulkanContext, surface = holder.surface)
        renderer.start()
    }

    override fun surfaceChanged(holder: SurfaceHolder, format: Int, width: Int, height: Int) {
        // optionally handle resize
    }

    override fun surfaceDestroyed(holder: SurfaceHolder) {
        renderer.stop()
        renderer.destroy()
    }

}