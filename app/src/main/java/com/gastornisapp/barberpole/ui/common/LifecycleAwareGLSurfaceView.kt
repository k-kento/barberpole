package com.gastornisapp.barberpole.ui.common

import android.content.Context
import android.opengl.GLSurfaceView
import android.util.Log
import androidx.lifecycle.DefaultLifecycleObserver
import androidx.lifecycle.Lifecycle
import androidx.lifecycle.LifecycleOwner

abstract class LifecycleAwareGLSurfaceView(
    context: Context,
    val lifecycle: Lifecycle,
) : GLSurfaceView(context) {

    private val observer = object : DefaultLifecycleObserver {
        override fun onResume(owner: LifecycleOwner) {
            Log.d("LifecycleAwareGLSurfaceView", "onResume")
            this@LifecycleAwareGLSurfaceView.onResume()
        }

        override fun onPause(owner: LifecycleOwner) {
            Log.d("LifecycleAwareGLSurfaceView", "onPause")
            this@LifecycleAwareGLSurfaceView.onPause()
        }
    }

    override fun onAttachedToWindow() {
        super.onAttachedToWindow()
        lifecycle.addObserver(observer)
    }

    override fun onDetachedFromWindow() {
        lifecycle.removeObserver(observer)
        super.onDetachedFromWindow()
    }
}
