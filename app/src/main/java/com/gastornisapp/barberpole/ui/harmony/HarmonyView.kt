package com.gastornisapp.barberpole.ui.harmony

import android.content.Context
import android.opengl.GLSurfaceView

class HarmonyView(context: Context) : GLSurfaceView(context) {
    private val renderer: HarmonyRenderer

    init {
        setEGLContextClientVersion(3)
        renderer = HarmonyRenderer(context)
        setRenderer(renderer)
        renderMode = RENDERMODE_CONTINUOUSLY
    }
}
