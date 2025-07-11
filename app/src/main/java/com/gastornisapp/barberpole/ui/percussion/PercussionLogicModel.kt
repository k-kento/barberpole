package com.gastornisapp.barberpole.ui.percussion

import android.opengl.Matrix
import com.gastornisapp.barberpole.ui.ViewBounds
import com.gastornisapp.barberpole.ui.adjustPressedColor
import com.gastornisapp.barberpole.ui.colorCodeToFloatArray
import com.gastornisapp.soundlib.AudioResource

class PercussionLogicModel(
    val x: Float,
    val y: Float,
    val scale: Float = 0.25f,
    val type: AudioResource,
    var isPressed: Boolean = false,
    color: Int
) {
    val modelMatrix = FloatArray(16)

    /**
     * エフェクト用
     */
    val effectModelMatrix = FloatArray(16)

    val color = colorCodeToFloatArray(color)

    val pressedColor = colorCodeToFloatArray(adjustPressedColor(color))

    fun updateModelMatrix() {
        Matrix.setIdentityM(modelMatrix, 0)
        Matrix.translateM(modelMatrix, 0, x, y, 0f)

        System.arraycopy(modelMatrix, 0, effectModelMatrix, 0, 16)

        Matrix.scaleM(modelMatrix, 0, scale, scale, 1f)
        Matrix.scaleM(effectModelMatrix, 0, scale * 1.3f, scale * 1.3f, 1f)
    }

    companion object {
        fun buildButtonLogicModels(bounds: ViewBounds): Set<PercussionLogicModel> {
            val buttons = listOf(
                ButtonInfo(AudioResource.Bongo, 0xF44336),
                ButtonInfo(AudioResource.Pan, 0xFF9800),
                ButtonInfo(AudioResource.Shaker, 0xFFEB3B),
                ButtonInfo(AudioResource.Taiko, 0x4CAF50),
                ButtonInfo(AudioResource.Tambourines, 0x2196F3),
                ButtonInfo(AudioResource.Stick, 0x3F51B5),
                ButtonInfo(AudioResource.Bell, 0x9C27B0),
                ButtonInfo(AudioResource.Conga, 0x000000)
            )

            val width = bounds.right - bounds.left
            val height = bounds.top - bounds.bottom

            // 画面の向きに応じて行・列数を決定（長い方に4を入れる）
            val (rows, cols) = if (width >= height) {
                2 to 4  // 横長なら2行4列
            } else {
                4 to 2  // 縦長なら4行2列
            }

            val cellWidth = width / cols
            val cellHeight = height / rows

            val scale = 0.45f * minOf(cellWidth, cellHeight)

            val startX = bounds.left + cellWidth / 2f
            val startY = bounds.top - cellHeight / 2f

            return buttons.mapIndexed { index, info ->
                val row = index / cols
                val col = index % cols

                val x = startX + col * cellWidth
                val y = startY - row * cellHeight

                PercussionLogicModel(
                    x = x,
                    y = y,
                    scale = scale,
                    type = info.type,
                    color = info.color
                )
            }.toSet()
        }

        private data class ButtonInfo(
            val type: AudioResource,
            val color: Int
        )
    }
}