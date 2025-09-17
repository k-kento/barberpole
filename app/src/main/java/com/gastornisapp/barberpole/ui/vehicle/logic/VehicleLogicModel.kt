package com.gastornisapp.barberpole.ui.vehicle.logic

import com.gastornisapp.barberpole.ui.ViewBounds

sealed class VehicleLogicModel(
    val id: Int,
    val scale: Float,
) {

    var velocity: Float = 0.0004f
        private set

    var pressed: Boolean = false

    var distance: Float = 0f
        private set

    var posX: Float = 0f
        private set

    var posY: Float = 0f
        private set

    var orientation: Orientation = Orientation.Left
        private set

    var color: FloatArray = colors.random()
        private set

    // MVP のキャッシュ TODO VehicleLogicModel で mvp Matrixを持たない方がいい
    val mvpMatrix = FloatArray(16)

    /**
     * 幅
     */
    val scaledWidth: Float = 2f * scale

    /**
     * 高さ
     */
    val scaledHeight: Float = 2f * scale

    abstract val colors: Array<FloatArray>

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

    fun updatePosition(loop: Int, newDistance: Float, laneHeight: Float, viewBounds: ViewBounds) {
        // 偶数ループなら左向き、奇数なら右向き
        val isLeft = (loop and 1) == 0
        val direction = if (isLeft) -1 else 1
        val posX = newDistance - loop * viewBounds.width

        this.posX = direction * posX
        this.posY = laneHeight * loop - laneHeight / 2f
        this.orientation = if (isLeft) Orientation.Left else Orientation.Right
        this.distance = newDistance
    }

    fun handleTouchDown(touchX: Float, touchY: Float): Boolean {
        // 車の位置とサイズ（中心座標と半サイズ）で矩形当たり判定
        val halfWidth = scaledWidth / 2f
        val halfHeight = scaledHeight / 2f

        val left = posX - halfWidth
        val right = posX + halfWidth
        val top = posY + halfHeight
        val bottom = posY - halfHeight

        if (touchX in left..right && touchY in bottom..top) {
            pressed = true
        }

        return pressed
    }

    fun reset(viewBounds: ViewBounds) {
        distance = -1 * viewBounds.right
        orientation = Orientation.Left
        color = colors.random()
    }

    enum class Orientation(val value: Int) {
        Left(-1),
        Right(1)
    }
}
