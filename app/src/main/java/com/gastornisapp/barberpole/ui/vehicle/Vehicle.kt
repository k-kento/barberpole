package com.gastornisapp.barberpole.ui.vehicle

data class Vehicle(
    val model: VehicleModel,
    var pressed: Boolean = false,
    val velocity: Float,
    var posX: Float
)
