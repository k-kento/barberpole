package com.gastornisapp.barberpole.ui.vehicle.logic

import android.opengl.GLES30
import android.opengl.Matrix
import com.gastornisapp.barberpole.ui.colorCodeToFloatArray
import com.gastornisapp.barberpole.ui.vehicle.VehicleShaderProgram
import com.gastornisapp.barberpole.ui.vehicle.renderer.VehicleRenderModel

abstract class VehicleLogicModel(
    val id: Int,
    private val renderModel: VehicleRenderModel,
) {

    private val modelMatrix = FloatArray(16)
    var velocity: Float = 0.0004f
    var pressed: Boolean = false
    var distance: Float = 0f
    var posX: Float = 0f
    var posY: Float = 0f
    var orientation: Orientation = Orientation.Left
    var color: FloatArray = colors.random()

    val scale: Float = 0.25f

    /**
     * 幅
     */
    val scaledWidth: Float = renderModel.width * scale

    /**
     * 高さ
     */
    val scaledHeight: Float = renderModel.height * scale

    fun render(program: VehicleShaderProgram) {
        updateModelMatrix(program)
        draw()
    }

    private fun draw() {
        renderModel.draw(color = color)
    }

    private fun updateModelMatrix(program: VehicleShaderProgram) {
        Matrix.setIdentityM(modelMatrix, 0)
        // 車の向き
        val flipX = if (orientation == Orientation.Left) -1 else 1
        Matrix.translateM(modelMatrix, 0, modelMatrix, 0, posX, posY, 0f)
        Matrix.scaleM(modelMatrix, 0, flipX * scale, scale, 1f)
        GLES30.glUniformMatrix4fv(program.uModelMatrixLocation, 1, false, modelMatrix, 0)
    }

    fun checkFollowingDistance(frontVehicleLogicModel: VehicleLogicModel): Boolean {
        // 中心間距離
        val distanceBetweenCenters = frontVehicleLogicModel.distance - distance
        // 衝突しないための最低距離
        val minDistance = scaledWidth / 2f + frontVehicleLogicModel.scaledWidth / 2f

        return distanceBetweenCenters < minDistance
    }

    companion object {
        // HSB S=65 B=100
        val colors = arrayOf(
            colorCodeToFloatArray(0xFF1500), // 赤
            colorCodeToFloatArray(0x1100FF), // 青
            colorCodeToFloatArray(0x08FF00), // 緑
            colorCodeToFloatArray(0xED00FF), // 紫
            colorCodeToFloatArray(0x00D7FF), // 水色
            colorCodeToFloatArray(0xFFF301), // 黄
            colorCodeToFloatArray(0xFFA400), // オレンジ
        )
    }

    enum class Orientation {
        Left,
        Right
    }
}