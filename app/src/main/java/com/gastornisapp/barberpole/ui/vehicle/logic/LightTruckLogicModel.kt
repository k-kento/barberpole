package com.gastornisapp.barberpole.ui.vehicle.logic

import com.gastornisapp.barberpole.ui.utils.ColorUtil

class LightTruckLogicModel(id: Int) : VehicleLogicModel(
    id = id,
    scale = 0.25f
) {

    override val colors: Array<FloatArray>
        get() = COLORS

    companion object {
        val COLORS = arrayOf(
            ColorUtil.colorToFloatArray(0xFEFFAF),
            ColorUtil.colorToFloatArray(0xFFFFFF),
        )
    }
}
