package com.gastornisapp.barberpole.ui.percussion

import com.gastornisapp.barberpole.ui.ViewBounds

class PercussionLogicModelFactory {
    fun create(bounds: ViewBounds): Set<PercussionLogicModel> {
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

        return PercussionType.entries.mapIndexed { index, type ->
            val row = index / cols
            val col = index % cols

            val x = startX + col * cellWidth
            val y = startY - row * cellHeight

            PercussionLogicModel(
                x = x,
                y = y,
                scale = scale,
                type = type,
            )
        }.toSet()
    }
}
