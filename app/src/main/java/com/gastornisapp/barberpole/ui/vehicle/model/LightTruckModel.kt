package com.gastornisapp.barberpole.ui.vehicle.model

import com.gastornisapp.barberpole.ui.utils.ColorUtil

class LightTruckModel() : VehicleModel(scale = 0.35f) {

    override val colors: Array<FloatArray>
        get() = COLORS

    companion object {
        val COLORS = arrayOf(
            ColorUtil.colorToFloatArray(0xFEFFAF),
            ColorUtil.colorToFloatArray(0xFFFFFF),
        )
    }
}
