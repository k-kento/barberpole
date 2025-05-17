package com.gastornisapp.barberpole.ui.vehicle

import kotlin.random.Random

class VehicleManager {

    private val vehicles: MutableList<Vehicle> = mutableListOf()

    fun initialize(program: VehicleShaderProgram) {
        val vehicles = List(5) {
            val posX = Random.nextFloat() * 2f - 1f
            Vehicle(
                model = VehicleModel(program),
                velocity = 0.0005f,
                posX = posX
            )
        }
        this.vehicles.addAll(vehicles)
    }

    fun iterator(): Iterable<Vehicle> {
        return vehicles
    }

}
