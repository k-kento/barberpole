package ui.swarm

import android.content.Context
import android.opengl.GLSurfaceView
import android.view.SurfaceHolder

class FishView(context: Context) : GLSurfaceView(context) {
    private val renderer: FishRenderer

    init {
        setEGLContextClientVersion(3)
        renderer = FishRenderer(context)
        setRenderer(renderer)
    }

    override fun surfaceDestroyed(holder: SurfaceHolder) {
        renderer.release()
        super.surfaceDestroyed(holder)
    }
}
