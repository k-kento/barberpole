package com.gastornisapp.barberpole.ui.vehicle

import kotlin.math.floor
import kotlin.random.Random

class VehicleManager(program: VehicleShaderProgram) {

    private val activeVehicleIds = mutableListOf<Int>()
    private val inactiveVehicleIds = mutableListOf<Int>()
    private val pool: List<Vehicle>
    private var elapsedTime = 0L
    private var nextSpawnTime = getRandomSpawnTime()
    private var idCounter = 0
    private val toRemove = mutableListOf<Int>()

    init {
        val vehicles = List(10) {
            Vehicle(
                id = idCounter++,
                model = VehicleModel(program),
                distance = -1f,
            )
        }

        pool = vehicles

        // 初期はすべて未使用リストに追加
        inactiveVehicleIds.addAll(pool.map { it.id })
    }

    companion object {
        // 最低車間距離
        private const val MIN_DISTANCE = VehicleModel.VEHICLE_WIDTH + 0.1f
        private const val LANE_HEIGHT = 0.5f
        private const val SCREEN_WIDTH = 2f
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
        val vehicleId = inactiveVehicleIds.removeAt(randomIndex)
        val vehicle = pool[vehicleId]

        vehicle.distance = -1f // 初期位置リセット
        activeVehicleIds.add(vehicleId)
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
            val loop = floor((newDistance + 1f) / SCREEN_WIDTH).toInt() // 何周目か
            if (2 < loop) {
                // 3回目は折り返さず、車両を削除
                vehicle.distance = -1f
                toRemove.add(vehicle.id)
                inactiveVehicleIds.add(vehicle.id)
            } else {
                val orientation = if (loop and 1 == 0) -1 else 1
                val posX = newDistance - loop * SCREEN_WIDTH

                vehicle.posX = orientation * posX
                vehicle.posY = -0.5f + loop * LANE_HEIGHT
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
        return Random.nextLong(1000L, 3000L) // 1〜3秒の間でランダム
    }
}
