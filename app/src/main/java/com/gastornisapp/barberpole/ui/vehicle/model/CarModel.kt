package com.gastornisapp.barberpole.ui.vehicle.model

import com.gastornisapp.barberpole.ui.utils.ColorUtil

class CarModel(distance: Float) : VehicleModel(
    scale = 0.27f,
    distance = distance
) {

    override val colors: Array<FloatArray>
        get() = COLORS

    companion object {
        private val COLORS = arrayOf(
            ColorUtil.colorToFloatArray(0xE66C5C),
            ColorUtil.colorToFloatArray(0x655CE6),
            ColorUtil.colorToFloatArray(0xE6A85C),
            ColorUtil.colorToFloatArray(0x75EB7F),
            ColorUtil.colorToFloatArray(0x61F2E6),
        )
    }

}
