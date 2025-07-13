package com.gastornisapp.barberpole.ui.percussion

import android.opengl.Matrix
import com.gastornisapp.barberpole.ui.utils.adjustPressedColor
import com.gastornisapp.barberpole.ui.utils.colorCodeToFloatArray

class PercussionLogicModel(
    val x: Float,
    val y: Float,
    val scale: Float,
    val type: PercussionType,
    var isPressed: Boolean = false,
) {
    val modelMatrix: FloatArray = createModelMatrix()
    val effectModelMatrix: FloatArray = createEffectModelMatrix()
    val imageModelMatrix: FloatArray = createImageModelMatrix()
    val backgroundColor: FloatArray = colorCodeToFloatArray(type.color)
    val pressedBackgroundColor: FloatArray = colorCodeToFloatArray(adjustPressedColor(type.color))
    val iconColor: FloatArray = colorCodeToFloatArray(0xFFFFFF)
    val iconScale: Float
        get() = scale * 0.5f

    private fun createModelMatrix(): FloatArray {
        val matrix = FloatArray(16)
        Matrix.setIdentityM(matrix, 0)
        Matrix.translateM(matrix, 0, x, y, 0f)
        Matrix.scaleM(matrix, 0, scale, scale, 1f)
        return matrix
    }

    private fun createEffectModelMatrix(): FloatArray {
        val matrix = FloatArray(16)
        Matrix.setIdentityM(matrix, 0)
        Matrix.translateM(matrix, 0, x, y, 0f)
        Matrix.scaleM(matrix, 0, scale * 1.3f, scale * 1.3f, 1f)
        return matrix
    }

    private fun createImageModelMatrix(): FloatArray {
        val matrix = FloatArray(16)
        Matrix.setIdentityM(matrix, 0)
        Matrix.translateM(matrix, 0, x, y, 0f)
        Matrix.scaleM(matrix, 0, iconScale, iconScale, 1f)
        return matrix
    }
}