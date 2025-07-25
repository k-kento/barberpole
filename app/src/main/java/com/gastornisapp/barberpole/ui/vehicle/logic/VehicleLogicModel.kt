package com.gastornisapp.barberpole.ui.vehicle.logic

import com.gastornisapp.barberpole.ui.ViewBounds
import com.gastornisapp.barberpole.ui.utils.colorCodeToFloatArray
import com.gastornisapp.barberpole.ui.vehicle.VehicleRendererModel

sealed class VehicleLogicModel(
    val id: Int,
    val scale: Float,
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
    val scaledWidth: Float = VehicleRendererModel.WIDTH * scale

    /**
     * 高さ
     */
    val scaledHeight: Float = VehicleRendererModel.HEIGHT * scale

    /**
     * 前方車両との追従距離をチェックする
     *
     * @param frontVehicle 前方の車両
     * @param viewBounds   画面情報（幅など）
     * @return true: 追従距離が十分に確保されている / false: 接近しすぎている
     */
    fun isFollowingDistanceSafe(frontVehicle: VehicleLogicModel, viewBounds: ViewBounds): Boolean {
        // 中心点同士の距離（この車両の距離との差）
        val centerToCenterDistance = frontVehicle.distance - this.distance

        // 2台の車両サイズから計算した「接触ギリギリ距離」
        val combinedHalfWidths = (this.scaledWidth + frontVehicle.scaledWidth) / 2f

        // 衝突を避けるために必要な最小距離（画面幅の1/20を目安とする）
        val requiredClearance = viewBounds.width / 20f

        // 実際の間隔（中心間距離 - 車体サイズ）が必要最低距離より大きいかで判定
        val actualClearance = centerToCenterDistance - combinedHalfWidths

        return requiredClearance < actualClearance
    }

    companion object {
        val colors = arrayOf(
            colorCodeToFloatArray(0xF44336),
            colorCodeToFloatArray(0xFF9800),
            colorCodeToFloatArray(0xFFEB3B),
            colorCodeToFloatArray(0x4CAF50),
            colorCodeToFloatArray(0x2196F3),
            colorCodeToFloatArray(0x3F51B5),
            colorCodeToFloatArray(0x9C27B0),
        )
    }

    enum class Orientation(val value: Int) {
        Left(-1),
        Right(1)
    }
}