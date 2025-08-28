package com.gastornisapp.barberpole.ui.swarm

import com.gastornisapp.barberpole.math.MathUtil
import com.gastornisapp.barberpole.math.toRadian
import kotlin.math.atan2
import kotlin.math.cos
import kotlin.math.sin

class Boid(
    x: Float,
    y: Float,
    vx: Float,
    vy: Float,
    val color: FloatArray
) {

    var x: Float = x
        private set
    var y: Float = y
        private set

    var angle: Float = 0f
        private set

    var vx: Float = vx
        private set
    var vy: Float = vy
        private set

    // 角度平滑化のための履歴（最新5フレーム）
    private val angleHistory = ArrayDeque<Float>(5)

    /**
     * Boid の位置・速度・角度を更新する
     * @param vx 新しい速度 x 成分
     * @param vy 新しい速度 y 成分
     */
    fun update(vx: Float, vy: Float) {

        this.vx = vx
        this.vy = vy

        x += vx
        y += vy

        // 十分な速度がある場合のみ角度更新
        if (MathUtil.norm(vx, vy) > 0.004f) {
            val newAngle = atan2(vy, vx)

            // 履歴管理（古い角度を削除して最新5フレームを保持）
            if (angleHistory.size == 5) {
                angleHistory.removeFirst()
            }
            angleHistory.addLast(newAngle)

            // 正弦・余弦平均で滑らかにする
            var sumSin = 0f
            var sumCos = 0f
            for (a in angleHistory) {
                sumSin += sin(a)
                sumCos += cos(a)
            }
            angle = atan2(sumSin / angleHistory.size, sumCos / angleHistory.size)
        }
    }

    /**
     * 他の Boid が視野角内に存在するか判定する
     */
    fun isInFieldOfView(other: Boid): Boolean {
        // 相手との相対ベクトル
        val dx = other.x - this.x
        val dy = other.y - this.y

        // 速度ベクトルを正規化
        val (fx, fy) = MathUtil.normalize(this.vx, this.vy)
        if (fx == 0f && fy == 0f) {
            return true // 停止中なら全方位視野とみなす
        }

        // 相手へのベクトルを正規化
        val (dxn, dyn) = MathUtil.normalize(dx, dy)

        // 内積（cos θ）
        val dot = MathUtil.dot(fx, fy, dxn, dyn)

        // 浮動小数点数演算の誤差を考慮
        return FIELD_OF_VIEW_COS < dot
    }

    companion object {
        /**
         * 視野角
         */
        private val FIELD_OF_VIEW_RADIAN = 120f.toRadian()
        private val FIELD_OF_VIEW_COS = cos(FIELD_OF_VIEW_RADIAN)
    }
}