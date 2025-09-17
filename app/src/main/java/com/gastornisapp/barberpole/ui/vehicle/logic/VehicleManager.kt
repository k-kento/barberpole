package com.gastornisapp.barberpole.ui.vehicle.logic

import com.gastornisapp.barberpole.ui.ViewBounds
import kotlin.math.floor
import kotlin.random.Random

class VehicleManager {

    /**
     * 走行中の車
     */
    private val _activeVehicles = mutableListOf<VehicleLogicModel>()
    val activeVehicles: List<VehicleLogicModel>
        get() = _activeVehicles
    private val inactiveVehicles = mutableListOf<VehicleLogicModel>()

    private var elapsedTime = 0L
    private var nextSpawnTime = getRandomSpawnTime()

    var viewBounds = ViewBounds(0f, 0f, 0f, 0f)

    init {
        val vehicles = VehicleFactory.create()
        inactiveVehicles.addAll(vehicles)
    }

    // 毎フレーム呼ばれる処理
    fun update(deltaTime: Long) {
        elapsedTime += deltaTime
        if (nextSpawnTime <= elapsedTime) {
            addVehicle()
            elapsedTime = 0L
            nextSpawnTime = getRandomSpawnTime()
        }
        updatePosition(deltaTime = deltaTime)
    }

    private fun addVehicle() {
        if (inactiveVehicles.isEmpty()) return

        // 未使用車両からランダムに1台選ぶ
        val randomIndex = (inactiveVehicles.indices).random()
        val candidateVehicle = inactiveVehicles[randomIndex]
        val lastVehicle = _activeVehicles.lastOrNull()

        // 車両追加時に位置をチェックするため、削除時に位置を初期化する
        candidateVehicle.reset(viewBounds)

        if (lastVehicle != null) {
            if (!candidateVehicle.isFollowingDistanceSafe(lastVehicle, viewBounds)) {
                // 近すぎるので追加しない
                return
            }
        }
        inactiveVehicles.removeAt(randomIndex)
        _activeVehicles.add(candidateVehicle)
    }

    private fun updatePosition(deltaTime: Long) {
        val laneHeight = viewBounds.height / LANE_NUM
        for (index in _activeVehicles.lastIndex downTo 0) {
            // ループないで active な車両を削除するため、downTo を使用
            val vehicle = _activeVehicles[index]
            // タップされている時、その車両を移動させない。
            if (vehicle.pressed) continue

            val deltaDistance = vehicle.velocity * deltaTime
            val newDistance = vehicle.distance + deltaDistance

            // 車両間距離チェック
            if (0 < index) {
                val frontVehicle = _activeVehicles.getOrNull(index - 1)
                if (frontVehicle != null) {
                    if (!vehicle.isFollowingDistanceSafe(frontVehicle, viewBounds)) {
                        continue
                    }
                }
            }

            // distance を [screenInfo.left, screenInfo.right] のループに変換
            val loop = floor((newDistance + viewBounds.right) / viewBounds.width).toInt() // 何周目か
            if (LANE_NUM <= loop) {
                //折り返さず、車両を削除
                _activeVehicles.removeAt(index)
                inactiveVehicles.add(vehicle)
            } else {
                vehicle.updatePosition(
                    loop = loop,
                    newDistance = newDistance,
                    laneHeight = laneHeight,
                    viewBounds = viewBounds
                )
            }
        }
    }

    fun handleTouchDown(touchX: Float, touchY: Float): Boolean {
        var touch = false
        for (vehicle in _activeVehicles) {
            if (vehicle.handleTouchDown(touchX = touchX, touchY = touchY)) {
                touch = true
                break
            }
        }
        return touch
    }

    fun handleTouchUp() {
        for (vehicle in _activeVehicles) {
            vehicle.pressed = false
        }
    }

    private fun getRandomSpawnTime(): Long {
        return Random.nextLong(1000L, 3000L) // 1〜3秒の間でランダム
    }

    companion object {
        private const val LANE_NUM = 2
    }
}
