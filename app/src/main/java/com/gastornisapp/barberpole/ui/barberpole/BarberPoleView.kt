package com.gastornisapp.barberpole.ui.barberpole

import android.content.Context
import android.opengl.GLSurfaceView
import androidx.compose.ui.graphics.Color

class BarberPoleView(context: Context) : GLSurfaceView(context) {

    private var renderer: BarberPoleRenderer?

    private var isResumed = false
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

    fun setResumed(isResumed: Boolean) {
        if (this.isResumed == isResumed) return
        this.isResumed = isResumed
        if (isResumed) {
            onResume()
        } else {
            onPause()
        }
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
            val value = when (orientation) {
                Orientation.Left -> false
                Orientation.Right -> true
            }
            renderer?.orientation = value
        }
    }

    fun setColors(firstColor: Color, secondColor: Color) {
        if (firstColor == this.firstColor && secondColor == this.secondColor) return
        this.firstColor = firstColor
        this.secondColor = secondColor
        queueEvent {
            renderer?.setColors(
                firstColor = rgbToFloatArray(firstColor),
                secondColor = rgbToFloatArray(secondColor)
            )
        }
    }

    override fun onDetachedFromWindow() {
        super.onDetachedFromWindow()
        renderer?.release()
    }
}

private fun rgbToFloatArray(color: Color): FloatArray {
    val red = color.red
    val green = color.green
    val blue = color.blue
    return floatArrayOf(red, green, blue, 0f)
}

enum class Orientation {
    Left,
    Right;

    fun toggle(): Orientation {
        return when (this) {
            Left -> Right
            Right -> Left
        }
    }
}