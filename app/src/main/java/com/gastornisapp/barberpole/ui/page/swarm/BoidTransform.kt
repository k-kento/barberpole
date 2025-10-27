package com.gastornisapp.barberpole.ui.page.swarm

import android.opengl.Matrix
import com.gastornisapp.barberpole.common.toDegrees

class BoidTransform {
    val modelMatrix = FloatArray(16)

    fun updateMatrix(boid: Boid) {
        val x = boid.x
        val y = boid.y
        val angle = boid.angle.toDegrees()

        Matrix.setIdentityM(modelMatrix, 0)
        Matrix.translateM(modelMatrix, 0, x, y, 0f)
        Matrix.rotateM(modelMatrix, 0, angle, 0f, 0f, 1f)
        Matrix.scaleM(modelMatrix, 0, SCALE, SCALE, 1f)
    }

    companion object {
        const val SCALE = 0.2f
    }
}
