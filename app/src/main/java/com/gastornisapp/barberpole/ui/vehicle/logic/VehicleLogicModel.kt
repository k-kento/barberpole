package com.gastornisapp.barberpole.ui.vehicle.logic

import com.gastornisapp.barberpole.ui.colorCodeToFloatArray
import com.gastornisapp.barberpole.ui.vehicle.VehicleRenderModel

abstract class VehicleLogicModel(
    val id: Int,
    private val renderModel: VehicleRenderModel,
    private val scale: Float,
) {

    var velocity: Float = 0.0004f
    var pressed: Boolean = false
    var distance: Float = 0f
    var posX: Float = 0f
    var posY: Float = 0f
    var orientation: Orientation = Orientation.Left
    var color: FloatArray = colors.random()

    /**
     * 幅
     */
    val scaledWidth: Float = VehicleRenderModel.WIDTH * scale

    /**
     * 高さ
     */
    val scaledHeight: Float = VehicleRenderModel.HEIGHT * scale

    fun render() {
        renderModel.updateModelMatrix(posX = posX, posY = posY, orientation = orientation.value, scale = scale)
        renderModel.draw(color = color)
    }

    fun checkFollowingDistance(frontVehicleLogicModel: VehicleLogicModel): Boolean {
        // 中心間距離
        val distanceBetweenCenters = frontVehicleLogicModel.distance - distance
        // 衝突しないための最低距離
        val minDistance = scaledWidth / 2f + frontVehicleLogicModel.scaledWidth / 2f

        return distanceBetweenCenters < minDistance
    }

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
    }

    enum class Orientation(val value: Int) {
        Left(-1),
        Right(1)
    }
}