package com.gastornisapp.barberpole.ui.vehicle

import kotlin.math.floor
import kotlin.random.Random

// モデル空間の範囲
// X軸 -1.0f ~ 1.0f
// Y軸 -1.0f ~ 1.0f
class VehicleManager {

    private val activeVehicleIds = mutableListOf<Int>()
    private val inactiveVehicleIds = mutableListOf<Int>()
    private val pool: List<Vehicle>
    private var elapsedTime = 0L
    private var nextSpawnTime = getRandomSpawnTime()
    private var idCounter = 0
    private val toRemove = mutableListOf<Int>()

    init {
        pool = List(10) { Vehicle(id = idCounter++) }
        inactiveVehicleIds.addAll(pool.map(Vehicle::id))
    }

    companion object {
        // 最低車間距離
        private const val MIN_DISTANCE = Vehicle.VEHICLE_WIDTH + 0.1f
        private const val LANE_HEIGHT = 0.5f
        private const val LANE_NUM = 3
        // 幅。本来は、2.0であるが、画面外に少し余裕を持たせる
        private const val WIDTH = 2.5f
    }

    // 毎フレーム呼ばれる処理
    fun update(deltaTime: Long) {
        elapsedTime += deltaTime
        if (nextSpawnTime <= elapsedTime) {
            addVehicle()
            elapsedTime = 0L
            nextSpawnTime = getRandomSpawnTime()
        }

        updatePosition(deltaTime)
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
            val distanceGap = lastVehicle.distance - candidateVehicle.distance
            if (distanceGap < MIN_DISTANCE) {
                // 近すぎるので追加しない
                return
            }
        }

        // 車両初期化
        val rand = Random.nextFloat()

        candidateVehicle.apply {
            distance = -1f
            orientation = Vehicle.Orientation.Left
            vehicleType = if (rand < 0.8f) Vehicle.VehicleType.Car else Vehicle.VehicleType.Bus
        }

        inactiveVehicleIds.removeAt(randomIndex)
        activeVehicleIds.add(candidateVehicleId)
    }

    private fun updatePosition(deltaTime: Long) {
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
                    val maxDistance = frontVehicle.distance - MIN_DISTANCE
                    if (maxDistance <= newDistance) {
                        continue
                    }
                }
            }

            // distance を [-1, 1] のループに変換
            val loop = floor((newDistance + 1f) / WIDTH).toInt() // 何周目か
            if (LANE_NUM <= loop) {
                // 3回目は折り返さず、車両を削除
                toRemove.add(vehicle.id)
                inactiveVehicleIds.add(vehicle.id)
            } else {
                // 偶数ループなら左向き、奇数なら右向き
                val isLeft = (loop and 1) == 0
                val direction = if (isLeft) -1 else 1
                val posX = newDistance - loop * WIDTH

                vehicle.posX = direction * posX
                vehicle.posY = -0.5f + loop * LANE_HEIGHT
                vehicle.orientation = if (isLeft) Vehicle.Orientation.Left else Vehicle.Orientation.Right
                vehicle.distance = newDistance
            }
        }

        if (toRemove.isNotEmpty()) {
            activeVehicleIds.removeAll(toRemove)
            toRemove.clear()
        }
    }

    fun iterator(): Iterable<Vehicle> {
        return activeVehicleIds.map { pool[it] }
    }

    private fun getRandomSpawnTime(): Long {
        return Random.nextLong(2000L, 4000L) // 1〜3秒の間でランダム
    }
}
