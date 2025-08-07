package com.gastornisapp.barberpole.ui.vehicle.logic

import com.gastornisapp.barberpole.ui.ViewBounds
import kotlin.math.floor
import kotlin.random.Random

// モデル空間の範囲
// X軸 -1.0f ~ 1.0f
// Y軸 -1.0f ~ 1.0f
class VehicleManager {

    private val activeVehicleIds = mutableListOf<Int>()
    private val inactiveVehicleIds = mutableListOf<Int>()
    private val pool: List<VehicleLogicModel>
    private var elapsedTime = 0L
    private var nextSpawnTime = getRandomSpawnTime()
    private var idCounter = 0
    private val toRemove = mutableListOf<Int>()
    var viewBounds = ViewBounds(0f, 0f, 0f, 0f)

    init {
        val pool = mutableListOf<VehicleLogicModel>()

        repeat(30) {
            // 0〜9 の乱数 → 0〜6 なら車、7〜9 ならバス → 7:3 の確率
            if ((0..9).random() < 7) {
                pool.add(CarLogicModel(idCounter++))
            } else {
                pool.add(BusLogicModel(idCounter++))
            }
        }

        this.pool = pool

        inactiveVehicleIds.addAll(pool.map(VehicleLogicModel::id))
    }

    fun activeVehicles(): List<VehicleLogicModel> {
        return activeVehicleIds.map { pool[it] }
    }

    // 毎フレーム呼ばれる処理
    fun update(deltaTime: Long, viewBounds: ViewBounds) {
        elapsedTime += deltaTime
        if (nextSpawnTime <= elapsedTime) {
            addVehicle()
            elapsedTime = 0L
            nextSpawnTime = getRandomSpawnTime()
        }
        updatePosition(deltaTime = deltaTime, viewBounds = viewBounds)
    }

    private fun addVehicle() {
        if (inactiveVehicleIds.isEmpty()) return

        // 未使用車両からランダムに1台選ぶ
        val randomIndex = (inactiveVehicleIds.indices).random()
        val candidateVehicleId = inactiveVehicleIds[randomIndex]
        val candidateVehicle = pool[candidateVehicleId]

        val lastVehicleId = activeVehicleIds.lastOrNull()
        val lastVehicle = lastVehicleId?.let { pool[it] }

        if (lastVehicle != null) {
            if (!candidateVehicle.isFollowingDistanceSafe(lastVehicle, viewBounds)) {
                // 近すぎるので追加しない
                return
            }
        }

        candidateVehicle.apply {
            distance = -1 * viewBounds.right
            orientation = VehicleLogicModel.Orientation.Left
            color = VehicleLogicModel.colors.random()
        }

        inactiveVehicleIds.removeAt(randomIndex)
        activeVehicleIds.add(candidateVehicleId)
    }

    private fun updatePosition(deltaTime: Long, viewBounds: ViewBounds) {
        for (index in 0 until activeVehicleIds.size) {
            val vehicle = pool.getOrNull(activeVehicleIds[index]) ?: continue
            // タップされている時、その車両を移動させない。
            if (vehicle.pressed) continue

            val deltaDistance = vehicle.velocity * deltaTime
            val newDistance = vehicle.distance + deltaDistance

            // 車両間距離チェック
            if (0 < index) {
                val frontVehicleId = activeVehicleIds[index - 1]
                val frontVehicle = pool.getOrNull(frontVehicleId)
                if (frontVehicle != null) {
                    if (!vehicle.isFollowingDistanceSafe(frontVehicle, viewBounds)) {
                        continue
                    }
                }
            }

            // distance を [screenInfo.left, screenInfo.right] のループに変換
            val loop = floor((newDistance + viewBounds.right) / viewBounds.width).toInt() // 何周目か
            if (LANE_NUM <= loop) {
                // 3回目は折り返さず、車両を削除
                toRemove.add(vehicle.id)
                inactiveVehicleIds.add(vehicle.id)

                // 車両追加時に位置をチェックするため、削除時に位置を初期化する
                vehicle.distance = -1 * viewBounds.right
            } else {
                // 偶数ループなら左向き、奇数なら右向き
                val isLeft = (loop and 1) == 0
                val direction = if (isLeft) -1 else 1
                val posX = newDistance - loop * viewBounds.width

                vehicle.posX = direction * posX
                vehicle.posY = -0.5f + loop * LANE_HEIGHT
                vehicle.orientation = if (isLeft) VehicleLogicModel.Orientation.Left else VehicleLogicModel.Orientation.Right
                vehicle.distance = newDistance
            }
        }

        if (toRemove.isNotEmpty()) {
            activeVehicleIds.removeAll(toRemove)
            toRemove.clear()
        }
    }

    fun handleTouchDown(touchX: Float, touchY: Float): Boolean {
        var touch = false
        for (vehicle in activeVehicleIds.map { pool[it] }) {
            // 車の位置とサイズ（中心座標と半サイズ）で矩形当たり判定
            val halfWidth = vehicle.scaledHeight / 2f
            val halfHeight = vehicle.scaledWidth / 2f

            val left = vehicle.posX - halfWidth
            val right = vehicle.posX + halfWidth
            val top = vehicle.posY + halfHeight
            val bottom = vehicle.posY - halfHeight

            if (touchX in left..right && touchY in bottom..top) {
                vehicle.pressed = true
                touch = true
                break
            }
        }
        return touch
    }

    fun handleTouchUp() {
        for (vehicle in activeVehicleIds.map { pool[it] }) {
            vehicle.pressed = false
        }
    }

    private fun getRandomSpawnTime(): Long {
        return Random.nextLong(2000L, 4000L) // 1〜3秒の間でランダム
    }

    companion object {
        private const val LANE_HEIGHT = 0.5f
        private const val LANE_NUM = 3
    }
}
