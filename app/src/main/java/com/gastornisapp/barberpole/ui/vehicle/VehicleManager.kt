package com.gastornisapp.barberpole.ui.vehicle

import kotlin.math.floor

class VehicleManager {

    private val vehicles: MutableList<Vehicle> = mutableListOf()

    fun initialize(program: VehicleShaderProgram) {
        val vehicles = List(5) { index ->
            val distance = -2f * index * 0.3f
            Vehicle(
                model = VehicleModel(program),
                distance = distance,
                posX = distance
            )
        }
        this.vehicles.addAll(vehicles)
    }

    fun updatePositions(deltaFrameTime: Long) {
        for ((index, vehicle) in vehicles.withIndex()) {
            if (vehicle.pressed) continue

            val deltaDistance = vehicle.velocity * deltaFrameTime
            val distance = vehicle.distance + deltaDistance

            if (-1f < vehicle.distance) {

                if (index != 0) {
                    val frontVehicle = vehicles[index - 1] // 前の車
                    val distanceGap = frontVehicle.distance - distance
                    val minDistance = 0.3f  // 最低車間距離

                    // 近づきすぎたら減速
                    if (distanceGap <= minDistance) {
                        continue
                    }
                }

                // distance を0~1の間に変換
                val loop = floor((distance + 1f) / 2f).toInt()

                val orientation = if (loop and 1 == 0) -1 else 1

                // x 座標が-1~1の間に収まるよう変換
                val posX = distance - loop * 2f

                vehicle.posX = orientation * posX
                vehicle.posY = loop * 0.5f
            }
            vehicle.distance = distance
        }
    }

    fun iterator(): Iterable<Vehicle> {
        return vehicles
    }
}
