package com.gastornisapp.barberpole.barberpole

import android.content.Context
import android.opengl.GLSurfaceView
import android.util.Log
import androidx.compose.ui.graphics.Color
import androidx.lifecycle.Lifecycle
import androidx.lifecycle.LifecycleEventObserver

class BarberPoleView(context: Context) : GLSurfaceView(context) {

    private var renderer: BarberPoleRenderer?

    var lifecycle: Lifecycle? = null
        set(value) {
            field?.removeObserver(observer)
            value?.addObserver(observer)
            field = value
        }

    private val observer = LifecycleEventObserver { _, event ->
        when (event) {
            Lifecycle.Event.ON_PAUSE -> {
                Log.d("BarberPoleView", "onPause")
                onPause()
            }

            Lifecycle.Event.ON_RESUME -> {
                Log.d("BarberPoleView", "onResume")
                onResume()
            }

            else -> {}
        }
    }

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