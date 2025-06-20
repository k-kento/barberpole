package com.gastornisapp.barberpole.ui.harmony

import android.opengl.Matrix

data class ButtonLogicModel(
    val x: Float,
    val y: Float,
    var isPressed: Boolean = false
) {
    val modelMatrix = FloatArray(16)

    fun updateModelMatrix() {
        Matrix.setIdentityM(modelMatrix, 0)
        Matrix.translateM(modelMatrix, 0, x, y, 0f)
        Matrix.scaleM(modelMatrix, 0, SCALE, SCALE, 1f)
    }

    companion object {
        const val SCALE = 0.25f
    }
}