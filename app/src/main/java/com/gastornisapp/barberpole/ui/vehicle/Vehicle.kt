package com.gastornisapp.barberpole.ui.vehicle

import android.opengl.Matrix
import com.gastornisapp.barberpole.ui.ScreenInfo

class Vehicle(val id: Int) {

    val modelMatrix = FloatArray(16)

    fun updateModelMatrix(screenInfo: ScreenInfo) {
        Matrix.setIdentityM(modelMatrix, 0)

        val translateX = posX * screenInfo.width / 2f
        val translateY = posY * screenInfo.height / 2f

        val orientation = if (orientation == Orientation.left) -1 else 1
        Matrix.translateM(modelMatrix, 0, modelMatrix, 0, translateX, translateY, 0f)
        Matrix.scaleM(modelMatrix, 0, orientation * VEHICLE_SCALE, VEHICLE_SCALE, 1f)
    }

    var velocity: Float = 0.0005f
    var pressed: Boolean = false
    var distance: Float = 0f
    var posX: Float = 0f
    var posY: Float = 0f
    var orientation: Orientation = Orientation.left
    val color: FloatArray = colors.random()

    companion object {
        // HSB S=65 B=100
        val colors = arrayOf(
            floatArrayOf(1.0f, 0.901f, 0.349f), // #FFE659
            floatArrayOf(0.349f, 1.0f, 0.643f), // #59FFA4
            floatArrayOf(1.0f, 0.361f, 0.349f), // #FF5C59
            floatArrayOf(0.392f, 0.349f, 1.0f), // #6459FF
            floatArrayOf(0.431f, 0.431f, 0.431f), // #6E6E6E
        )

        /**
         * 拡大率
         */
        private const val VEHICLE_SCALE = 0.25f

        /**
         * 幅
         */
        const val VEHICLE_WIDTH = 2 * VEHICLE_SCALE

        /**
         * 高さ
         */
        const val VEHICLE_HEIGHT = 2 * VEHICLE_SCALE
    }

    enum class Orientation {
        left,
        right
    }
}