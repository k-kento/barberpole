package com.gastornisapp.barberpole.ui.page.barberpole

import android.content.Context
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.graphics.toArgb
import androidx.lifecycle.Lifecycle
import com.gastornisapp.barberpole.ui.common.LifecycleAwareGLSurfaceView

class BarberPoleView(context: Context, lifecycle: Lifecycle) : LifecycleAwareGLSurfaceView(context, lifecycle) {

    private var renderer: BarberPoleRenderer?

    private var isPlaying = false
    private var speed = 0f
    private var orientation: Orientation = Orientation.Left
    private var firstColor = Color.White
    private var secondColor = Color.Black

    init {
        setEGLContextClientVersion(3)
        renderer = BarberPoleRenderer(context = this.context.applicationContext)
        setRenderer(renderer)
    }

    fun setPlaying(isPlaying: Boolean) {
        if (this.isPlaying == isPlaying) return
        this.isPlaying = isPlaying
        queueEvent {
            renderer?.isPlaying = isPlaying
        }
    }

    fun setSpeed(value: Float) {
        if (speed == value) return
        speed = value
        queueEvent {
            renderer?.speed = value
        }
    }

    fun setOrientation(orientation: Orientation) {
        if (orientation == this.orientation) return
        this.orientation = orientation
        queueEvent {
            renderer?.orientation = orientation
        }
    }

    fun setColors(firstColor: Color, secondColor: Color) {
        if (firstColor == this.firstColor && secondColor == this.secondColor) return
        this.firstColor = firstColor
        this.secondColor = secondColor
        queueEvent {
            renderer?.setColors(
                firstColor = firstColor.toArgb(),
                secondColor = secondColor.toArgb()
            )
        }
    }

    fun release() {
        renderer?.release()
    }
}
