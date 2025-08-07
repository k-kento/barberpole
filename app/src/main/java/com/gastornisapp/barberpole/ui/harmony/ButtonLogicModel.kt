package com.gastornisapp.barberpole.ui.harmony

import android.opengl.Matrix

data class ButtonLogicModel(
    val x: Float,
    val y: Float,
    var isPressed: Boolean = false
) {
    val modelMatrix = FloatArray(16)

    /**
     * エフェクト用
     */
    val effectModelMatrix = FloatArray(16)

    fun updateModelMatrix() {
        Matrix.setIdentityM(modelMatrix, 0)
        Matrix.translateM(modelMatrix, 0, x, y, 0f)

        System.arraycopy(modelMatrix, 0, effectModelMatrix, 0, 16)

        Matrix.scaleM(modelMatrix, 0, SCALE, SCALE, 1f)
        Matrix.scaleM(effectModelMatrix, 0, SCALE * 1.3f, SCALE * 1.3f, 1f)
    }

    companion object {
        const val SCALE = 0.25f
        val color = floatArrayOf(0f, 1f, 0f, 1f)
    }
}