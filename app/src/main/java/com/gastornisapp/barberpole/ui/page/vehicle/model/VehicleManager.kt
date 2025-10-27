package com.gastornisapp.barberpole.ui.page.vehicle.model

import android.util.Log
import com.gastornisapp.barberpole.ui.ViewBounds
import kotlin.math.floor

class VehicleManager {

    private val _activeVehicles = mutableListOf<VehicleModel>()
    val activeVehicles: List<VehicleModel>
        get() = _activeVehicles

    private val pendingAddQueue = ArrayDeque<VehicleModel>()

    var viewBounds = ViewBounds(0f, 0f, 0f, 0f)
        set(value) {
            field = ViewBounds(
                left = value.left - SCREEN_WIDTH_OFFSET,
                top = value.top,
                right = value.right + SCREEN_WIDTH_OFFSET,
                bottom = value.bottom
            )
        }

    // 毎フレーム呼ばれる処理
    fun update(deltaTime: Long) {
        processPendingVehicles()
        updatePosition(deltaTime = deltaTime)
    }

    fun addVehicle(vehicleType: VehicleType) {
        if (pendingAddQueue.size <= MAX_PENDING_VEHICLES) {
            val vehicle = VehicleModel.create(vehicleType)
            pendingAddQueue.addLast(vehicle)
        }
    }

    /**
     * 保留中の車両追加リクエストを処理する
     * - キューの先頭車両を確認して追加可能なら activeVehicles に登録
     * - 安全距離を確保できない場合は次フレームに持ち越す
     */
    private fun processPendingVehicles() {
        val tailVehicle = activeVehicles.lastOrNull()
        val newVehicle = pendingAddQueue.firstOrNull()
        if (newVehicle != null) {
            if (tailVehicle == null || newVehicle.isFollowingDistanceSafe(tailVehicle, viewBounds)) {
                _activeVehicles.add(newVehicle)
                Log.d("VehicleManager", "vehicle added.size = ${_activeVehicles.size}")
                pendingAddQueue.removeFirst()
            }
        }
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
            val loop = floor(newDistance / viewBounds.width).toInt() // 何周目か
            if (LANE_NUM <= loop) {
                //折り返さず、車両を削除
                _activeVehicles.removeAt(index)
                Log.d("VehicleManager", "Vehicle removed. size=${_activeVehicles.size}")
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

    companion object {
        private const val LANE_NUM = 2
        // 最大キューサイズ
        private const val MAX_PENDING_VEHICLES = 3
        // 画面幅補正
        private const val SCREEN_WIDTH_OFFSET = 0.3f
    }
}
