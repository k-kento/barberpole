package com.gastornisapp.barberpole.ui.swarm

import com.gastornisapp.barberpole.math.MathUtil
import com.gastornisapp.barberpole.math.MathUtil.limit
import com.gastornisapp.barberpole.math.MathUtil.normalize
import com.gastornisapp.barberpole.ui.ViewBounds
import com.gastornisapp.barberpole.ui.utils.ColorUtil
import com.gastornisapp.barberpole.ui.utils.colorToFloatArray
import kotlin.math.PI
import kotlin.math.cos
import kotlin.math.max
import kotlin.math.sin
import kotlin.math.sqrt
import kotlin.random.Random

/**
 * Boid 群れのシミュレーションを管理するクラス
 */
class BoidSimulation() {

    val boids: List<Boid> = createBoids()

    /** 画面の表示範囲 */
    var viewBounds = ViewBounds(0f, 0f, 0f, 0f)

    private val tempFloatArray = FloatArray(2)

    /**
     * 個別の Boid を更新
     */
    fun update(self: Boid, tapPoint: Pair<Float, Float>? = null) {
        var dvSeparationX = 0f
        var dvSeparationY = 0f

        var dvAlignmentX = 0f
        var dvAlignmentY = 0f

        var dvCohesionX = 0f
        var dvCohesionY = 0f

        // 近隣の Boid の数
        var neighborCount = 0

        for (other in boids) {
            if (self == other) continue
            if (self.type != other.type) continue
            val distanceSquared = MathUtil.distanceSquared(x1 = self.x, y1 = self.y, x2 = other.x, y2 = other.y)
            if (!self.isInFieldOfView(other)) continue

            // 分離
            if (distanceSquared < SEPARATION_DISTANCE * SEPARATION_DISTANCE) {
                dvSeparationX += self.x - other.x
                dvSeparationY += self.y - other.y
            }

            // 整列
            if (distanceSquared < ALIGNMENT_DISTANCE * ALIGNMENT_DISTANCE) {
                dvAlignmentX += other.vx
                dvAlignmentY += other.vy
            }

            // 凝集
            if (distanceSquared < COHESION_DISTANCE * COHESION_DISTANCE) {
                dvCohesionX += other.x
                dvCohesionY += other.y
            }

            neighborCount++
        }

        if (0 < neighborCount) {
            // Alignment：近隣平均とのズレを補正
            dvAlignmentX /= neighborCount
            dvAlignmentY /= neighborCount
            dvAlignmentX = dvAlignmentX - self.vx
            dvAlignmentY = dvAlignmentY - self.vy

            // Cohesion：群れの中心方向
            dvCohesionX /= neighborCount
            dvCohesionY /= neighborCount
            dvCohesionX = dvCohesionX - self.x
            dvCohesionY = dvCohesionY - self.y
        }

        tempFloatArray[0] = dvSeparationX
        tempFloatArray[1] = dvSeparationY
        dvSeparationX = tempFloatArray[0]
        dvSeparationY = tempFloatArray[1]

        tempFloatArray[0] = dvAlignmentX
        tempFloatArray[1] = dvAlignmentY
        normalize(tempFloatArray)
        limit(tempFloatArray, MAX_FORCE)
        dvAlignmentX = tempFloatArray[0]
        dvAlignmentY = tempFloatArray[1]

        tempFloatArray[0] = dvCohesionX
        tempFloatArray[1] = dvCohesionY
        limit(tempFloatArray, MAX_FORCE)
        dvCohesionX = tempFloatArray[0]
        dvCohesionY = tempFloatArray[1]

        val radius = max(viewBounds.width, viewBounds.height) / 2
        boundaryForce(x = self.x, y = self.y, radius = radius, out = tempFloatArray)
        val boundingForceX = tempFloatArray[0]
        val boundingForceY = tempFloatArray[1]

        var repulsonForceX = 0f
        var repulsonForceY = 0f
        // 逃げる力
        if (tapPoint != null) {
            repulsonForce(x = self.x, y = self.y, tapPoint, out = tempFloatArray)
            repulsonForceX = tempFloatArray[0]
            repulsonForceY = tempFloatArray[1]
        }

        // 重み付き合成
        val dvTotalX = dvSeparationX * SEPARATION_WEIGHT +
                dvAlignmentX * ALIGNMENT_WEIGHT +
                dvCohesionX * COHESION_WEIGHT +
                boundingForceX * BOUNDARY_WEIGHT +
                repulsonForceX

        val dvTotalY = dvSeparationY * SEPARATION_WEIGHT +
                dvAlignmentY * ALIGNMENT_WEIGHT +
                dvCohesionY * COHESION_WEIGHT +
                boundingForceY * BOUNDARY_WEIGHT +
                repulsonForceY * REPULSON_WEIGHT

        var vx = self.vx + dvTotalX
        var vy = self.vy + dvTotalY

        var velLength = sqrt(vx * vx + vy * vy)

        // 反発力により、Boid が逃げた時に、徐々に速度を落とす。
        if (velLength > NORMAL_VELOCITY) {
            vx *= 0.95f
            vy *= 0.95f
            velLength *= 0.95f
        }

        // 速度を [MIN, MAX] に制限
        if (velLength > MAX_VELOCITY) {
            // 上限を超えたら制限
            vx = vx / velLength * MAX_VELOCITY
            vy = vy / velLength * MAX_VELOCITY
        } else if (velLength < MIN_VELOCITY && velLength > 0f) {
            // 下限を下回ったら引き上げ
            vx = vx / velLength * MIN_VELOCITY
            vy = vy / velLength * MIN_VELOCITY
        }

        // Boid に適用
        self.update(vx = vx, vy = vy)
    }

    /**
     * 反発力
     */
    private fun repulsonForce(
        x: Float,
        y: Float,
        point: Pair<Float, Float>,
        out: FloatArray
    ) {
        val dx = x - point.first
        val dy = y - point.second
        val distSq = dx * dx + dy * dy
        val radius = REPULSON_DISTANCE

        if (distSq < radius * radius && distSq > 0.0001f) {
            val dist = sqrt(distSq)
            // 正規化ベクトル
            val nx = dx / dist
            val ny = dy / dist
            // 近いほど強い (1 - d/r)
            val force = (1f - dist / radius)
            out[0] = nx * force
            out[1] = ny * force
        }
    }

    /**
     * 円形領域（半径 radius）の外に出たら中心方向へ引っ張る
     */
    private fun boundaryForce(
        x: Float,
        y: Float,
        radius: Float,
        out: FloatArray
    ) {
        val distSq = x * x + y * y
        val radiusSq = radius * radius

        if (distSq <= radiusSq) {
            // 内側なら力ゼロ
            out[0] = 0f
            out[1] = 0f
            return
        }

        out[0] = -x * MAX_FORCE
        out[1] = -y * MAX_FORCE
    }

    companion object {

        // ==========================
        // 運動特性
        // ==========================
        /**
         * 最大速度(距離/フレーム)
         */
        private const val MAX_VELOCITY = 0.06f
        private const val NORMAL_VELOCITY = 0.04f
        private const val MIN_VELOCITY = 0.015f

        /**
         * 最大加速度(一フレームあたりどれほど速度を変化させるか)
         */
        private const val MAX_FORCE = 0.0003f

        private const val INITIAL_SPEED = 0.02f


        // ==========================
        // 群れ行動パラメータ
        // ==========================

        private const val SEPARATION_DISTANCE = BoidTransform.SCALE * 1.1

        private const val ALIGNMENT_DISTANCE = 0.7f

        private const val COHESION_DISTANCE = 1.3f

        private const val REPULSON_DISTANCE = 0.7f

        /** 分離の力の重み */
        private const val SEPARATION_WEIGHT = 0.01f

        /** 整列の力の重み*/
        private const val ALIGNMENT_WEIGHT = 0.6f

        /** 凝集の力の重み */
        private const val COHESION_WEIGHT = 0.8f

        /** 中央に集まる力の重み */
        private const val BOUNDARY_WEIGHT = 0.4f

        /** 逃げる力の重み */
        private const val REPULSON_WEIGHT = 2.8f

        // ==========================
        // シミュレーション設定
        // ==========================
        private const val BOIDS_COUNT = 18

        private fun createBoids(): List<Boid> {
            val a = createGroup(0, 45f)
            val b = createGroup(1, 200f)
            val c = createGroup(2, 310f)
            return listOf(a, b, c).flatMap { it }
        }

        private fun createGroup(type: Int, baseHue: Float): List<Boid> {
            val colors = ColorUtil.generateSimilarColors(baseHue = baseHue, BOIDS_COUNT)

            // 群れの初期半径
            val r = 1f

            // 群れの初期方向
            val baseAngle = Random.nextFloat() * 2f * Math.PI.toFloat()

            return List(BOIDS_COUNT) { index ->
                // 位置は中心から半径内ランダム
                val theta = Random.nextDouble(0.0, 2 * PI)
                val radius = r * sqrt(Random.nextDouble()) // √で面積均等
                val x = (radius * cos(theta)).toFloat()
                val y = (radius * sin(theta)).toFloat()

                // 速度は基準方向に少しブレ
                val angleOffset = (Random.nextFloat() - 0.5f) * Math.PI.toFloat() / 6f // ±30度
                val vx = cos(baseAngle + angleOffset) * INITIAL_SPEED
                val vy = sin(baseAngle + angleOffset) * INITIAL_SPEED

                val color = colorToFloatArray(colors[index])
                Boid(type = type, x = x, y = y, vx = vx, vy = vy, color = color)
            }
        }
    }
}
