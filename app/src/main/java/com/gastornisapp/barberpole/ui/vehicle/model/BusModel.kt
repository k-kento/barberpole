package com.gastornisapp.barberpole.ui.vehicle.model

import com.gastornisapp.barberpole.ui.utils.ColorUtil

class BusModel(distance: Float) : VehicleModel(
    scale = 0.4f,
    distance = distance
) {

    override val colors: Array<FloatArray>
        get() = COLORS

    companion object {
        private val COLORS = arrayOf(
            ColorUtil.colorToFloatArray(0xE66C5C),
            ColorUtil.colorToFloatArray(0x655CE6),
        )
    }

}