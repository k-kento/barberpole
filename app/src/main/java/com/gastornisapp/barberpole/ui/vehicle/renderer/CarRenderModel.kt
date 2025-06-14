package com.gastornisapp.barberpole.ui.vehicle.renderer

import com.gastornisapp.barberpole.ui.vehicle.VehicleShaderProgram

class CarRenderModel(program: VehicleShaderProgram, textureId: Int) : VehicleRenderModel(
    vertices = vertices,
    indices = indices,
    program = program,
    textureId = textureId
) {

    override val width: Float = 2f
    override val height: Float = 2f

    companion object {
        // 座標: x, y, u, v
        private val vertices = floatArrayOf(
            // 左下
            -1f, -1f, 0f, 1f,
            // 右下
            1f, -1f, 1f, 1f,
            // 左上
            -1f, 1f, 0f, 0f,
            // 右上
            1f, 1f, 1f, 0f
        )

        private val indices = shortArrayOf(
            0, 1, 2,
            2, 1, 3
        )
    }
}
