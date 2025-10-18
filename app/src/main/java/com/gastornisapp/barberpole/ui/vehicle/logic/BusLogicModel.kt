package com.gastornisapp.barberpole.ui.vehicle.logic

import com.gastornisapp.barberpole.ui.utils.ColorUtil

class BusLogicModel(id: Int) : VehicleLogicModel(
    id = id,
    scale = 0.4f
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