package com.gastornisapp.barberpole.ui.swarm

import com.gastornisapp.barberpole.math.MathUtil.distance
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
     * 1フレーム分の更新
     */
    fun updateBoid() {
        boids.forEach { boid ->
            updateBoid(self = boid)
        }
    }

    /**
     * 個別の Boid を更新
     */
    private fun updateBoid(self: Boid) {
        var dvSeparationX = 0f
        var dvSeparationY = 0f

        var dvAlignmentX = 0f
        var dvAlignmentY = 0f

        var dvCohesionX = 0f
        var dvCohesionY = 0f

        // 近隣の Boid の数
        var neighborCount = 0

        for (other in boids) {
            if (other == self) continue
            val distance = distance(x1 = self.x, y1 = self.y, x2 = other.x, y2 = other.y)
            if (NEIGHBOR_RADIUS < distance) continue
            if (!self.isInFieldOfView(other)) continue

            // 分離
            if (distance < DESIRED_SEPARATION) {
                dvSeparationX += self.x - other.x
                dvSeparationY += self.y - other.y
            }

            // 整列
            dvAlignmentX += other.vx
            dvAlignmentY += other.vy

            // 凝集
            dvCohesionX += other.x
            dvCohesionY += other.y

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

        // 画面内に留める力
        val (dvBoundaryX, dvBoundaryY) = boundaryForce(
            x = self.x,
            y = self.y,
            radius = max(viewBounds.width, viewBounds.height) / 4
        )

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

        tempFloatArray[0] = dvBoundaryX
        tempFloatArray[1] = dvBoundaryY
        limit(tempFloatArray, MAX_FORCE)
        val boundingForceX = tempFloatArray[0]
        val boundingForceY = tempFloatArray[1]

        // 重み付き合成
        val dvTotalX = dvSeparationX * SEPARATION_WEIGHT +
                dvAlignmentX * ALIGNMENT_WEIGHT +
                dvCohesionX * COHESION_WEIGHT +
                boundingForceX * BOUNDARY_WEIGHT

        val dvTotalY = dvSeparationY * SEPARATION_WEIGHT +
                dvAlignmentY * ALIGNMENT_WEIGHT +
                dvCohesionY * COHESION_WEIGHT +
                boundingForceY * BOUNDARY_WEIGHT

        var vx = self.vx + dvTotalX
        var vy = self.vy + dvTotalY

        tempFloatArray[0] = vx
        tempFloatArray[1] = vy
        // 速度を [MIN, MAX] に制限
        clampVelocity(tempFloatArray, min = MIN_VELOCITY, max = MAX_VELOCITY)
        vx = tempFloatArray[0]
        vy = tempFloatArray[1]

        // Boid に適用
        self.update(vx = vx, vy = vy)
    }

    /**
     * 円形領域（半径 radius）内にBoidを収める力
     * - 内側：力ゼロ
     * - 外側：中心方向に押し戻す
     */
    private fun boundaryForce(
        x: Float,
        y: Float,
        radius: Float,
    ): Pair<Float, Float> {
        val distSq = x * x + y * y
        val radiusSq = radius * radius

        if (distSq <= radiusSq) {
            return 0f to 0f // 円内 → 力ゼロ
        }

        val dist = sqrt(distSq)
        val nx = x / dist
        val ny = y / dist
        val excess = dist - radius

        // 距離に応じた反発力
        val strength = MAX_FORCE * (excess / radius).coerceAtMost(1f)
        return -nx * strength to -ny * strength
    }

    private fun clampVelocity(v: FloatArray, min: Float, max: Float) {
        val speed = sqrt(v[0] * v[0] + v[1] * v[1])
        if (speed > max) {
            // 上限を超えたら制限
            v[0] = v[0] / speed * max
            v[1] = v[1] / speed * max
        } else if (speed < min && speed > 0f) {
            // 下限を下回ったら引き上げ
            v[0] = v[0] / speed * min
            v[1] = v[1] / speed * min
        }
    }

    companion object {

        // ==========================
        // 運動特性
        // ==========================
        /**
         * 最大速度(距離/フレーム)
         */
        private const val MAX_VELOCITY = 0.06f

        private const val MIN_VELOCITY = 0.01f

        /**
         * 最大加速度(一フレームあたりどれほど速度を変化させるか)
         */
        private const val MAX_FORCE = 0.0003f

        private const val INITIAL_SPEED = 0.02f


        // ==========================
        // 群れ行動パラメータ
        // ==========================
        /** 近隣の Boid を判定する半径 */
        private const val NEIGHBOR_RADIUS = 1.5f

        /** Boid 同士の望ましい最小距離 */
        private const val DESIRED_SEPARATION = BoidTransform.SCALE

        /** 分離の力の重み */
        private const val SEPARATION_WEIGHT = 0.01f

        /** 整列の力の重み*/
        private const val ALIGNMENT_WEIGHT = 0.55f

        /** 凝集の力の重み */
        private const val COHESION_WEIGHT = 0.25f

        /** 中央に集まる力の重み */
        private const val BOUNDARY_WEIGHT = 0.5f

        // ==========================
        // シミュレーション設定
        // ==========================
        private const val BOIDS_COUNT = 20

        private fun createBoids(): List<Boid> {
            val colors = ColorUtil.generateSimilarColors(baseHue = 200f, BOIDS_COUNT)

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
                Boid(x = x, y = y, vx = vx, vy = vy, color = color)
            }
        }
    }
}
