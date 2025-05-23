package com.gastornisapp.barberpole.ui.vehicle

class Vehicle(
    val id: Int,
    val model: VehicleModel,
    var velocity: Float = 0.0005f,
    var pressed: Boolean = false,
    var distance: Float = 0f,
    var posX: Float = 0f,
    var posY: Float = 0f,
)
