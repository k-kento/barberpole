package com.gastornisapp.barberpole.ui.vehicle

import android.opengl.Matrix
import com.gastornisapp.barberpole.ui.ScreenInfo
import com.gastornisapp.barberpole.ui.colorCodeToFloatArray

class Vehicle(val id: Int) {

    val modelMatrix = FloatArray(16)

    fun updateModelMatrix(screenInfo: ScreenInfo) {
        Matrix.setIdentityM(modelMatrix, 0)

        // モデル座標をスクリーンの幅に合わせた座標に変換
        val translateX = posX * screenInfo.width / 2f
        val translateY = posY * screenInfo.height / 2f

        val orientation = if (orientation == Orientation.Left) -1 else 1
        Matrix.translateM(modelMatrix, 0, modelMatrix, 0, translateX, translateY, 0f)
        Matrix.scaleM(modelMatrix, 0, orientation * VEHICLE_SCALE, VEHICLE_SCALE, 1f)
    }

    var velocity: Float = 0.0005f
    var pressed: Boolean = false
    var distance: Float = 0f
    var posX: Float = 0f
    var posY: Float = 0f
    var orientation: Orientation = Orientation.Left
    var vehicleType: VehicleType = VehicleType.Car
    var color: FloatArray = colors.random()

    companion object {
        // HSB S=65 B=100
        val colors = arrayOf(
            colorCodeToFloatArray(0xFF1500), // 赤
            colorCodeToFloatArray(0x1100FF), // 青
            colorCodeToFloatArray(0x08FF00), // 緑
            colorCodeToFloatArray(0xED00FF), // 紫
            colorCodeToFloatArray(0x00D7FF), // 水色
            colorCodeToFloatArray(0xFFF301), // 黄
            colorCodeToFloatArray(0xFFA400), // オレンジ
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
        Left,
        Right
    }

    enum class VehicleType {
        Car,
        Bus
    }
}