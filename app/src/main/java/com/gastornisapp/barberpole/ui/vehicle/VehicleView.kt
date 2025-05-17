package com.gastornisapp.barberpole.ui.vehicle

import android.content.Context
import android.opengl.GLSurfaceView

class VehicleView(context: Context) : GLSurfaceView(context) {

    private var renderer: VehicleRenderer?

    init {
        setEGLContextClientVersion(3)
        renderer = VehicleRenderer(this.context.applicationContext)
        setRenderer(renderer)
    }
}
