package com.gastornisapp.barberpole.ui.barberpole

import android.content.Context
import android.opengl.GLSurfaceView
import androidx.compose.ui.graphics.Color

class BarberPoleView(context: Context) : GLSurfaceView(context) {

    private var renderer: BarberPoleRenderer?

    init {
        setEGLContextClientVersion(2)
        renderer = BarberPoleRenderer()
        setRenderer(renderer)
    }

    fun play() {
        queueEvent {
            renderer?.isPlaying = true
        }
    }

    fun pause() {
        queueEvent {
            renderer?.isPlaying = false
        }
    }

    fun setSpeed(value: Float) {
        queueEvent {
            if (value != 0f) {
                renderer?.speed = value
            }
        }
    }

    fun setOrientation(orientation: Orientation) {
        queueEvent {
            val value = when (orientation) {
                Orientation.Left -> false
                Orientation.Right -> true
            }
            renderer?.setOrientation(value)
        }
    }

    fun setColors(firstColor: Color, secondColor: Color) {
        queueEvent {
            renderer?.setColors(
                firstColor = rgbToFloatArray(firstColor),
                secondColor = rgbToFloatArray(secondColor)
            )
        }
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