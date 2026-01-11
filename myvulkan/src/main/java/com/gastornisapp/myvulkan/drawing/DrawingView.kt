package com.gastornisapp.myvulkan.drawing

import android.annotation.SuppressLint
import android.content.Context
import android.view.MotionEvent
import android.view.SurfaceHolder
import android.view.SurfaceView
import com.gastornisapp.myvulkan.Utility
import com.gastornisapp.myvulkan.VulkanContext

@SuppressLint("ViewConstructor")
class DrawingView(context: Context, val vulkanContext: VulkanContext) : SurfaceView(context),
    SurfaceHolder.Callback {

    private var lastX = Float.Companion.NaN
    private var lastY: Float = Float.Companion.NaN

    private var renderer: DrawingRenderer? = null

    init {
        holder.addCallback(this)
    }

    override fun surfaceCreated(holder: SurfaceHolder) {

        renderer = DrawingRenderer()
        renderer?.init(vulkanContext = vulkanContext, surface = holder.surface)
        renderer?.start()
    }

    override fun surfaceChanged(holder: SurfaceHolder, format: Int, width: Int, height: Int) {
        val rotationDegree = Utility.rotationToDegrees(context.display.rotation)
        renderer?.onSurfaceChanged(
            surface = holder.surface,
            deviceRotationDegree = rotationDegree
        )
    }

    override fun surfaceDestroyed(holder: SurfaceHolder) {
        renderer?.stop()
        renderer?.destroy()
    }

    override fun onTouchEvent(event: MotionEvent): Boolean {
        val x = event.x
        val y = event.y

        when (event.action) {
            MotionEvent.ACTION_DOWN -> {

            }

            MotionEvent.ACTION_MOVE -> {
                if (!java.lang.Float.isNaN(lastX)) {
                    val dx = x - lastX
                    val dy = y - lastY
                    if (dx * dx + dy * dy < MIN_DISTANCE * MIN_DISTANCE) {
                        return true // 間引き
                    }
                }
                lastX = x
                lastY = y
                renderer?.notifyTouchMoveEvent(x = x / width, y = y / height)
            }

            MotionEvent.ACTION_UP, MotionEvent.ACTION_CANCEL -> {

                renderer?.notifyTouchUpEvent()
            }
        }
        return true
    }

    companion object {
        const val MIN_DISTANCE: Float = 50.0f
    }

}
