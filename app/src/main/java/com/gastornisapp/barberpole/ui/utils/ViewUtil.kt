package com.gastornisapp.barberpole.ui.utils

import com.gastornisapp.barberpole.ui.ViewBounds

object ViewUtil {

    /**
     * 画面座標（ピクセル単位）をワールド座標へ直接変換する。
     *
     * スクリーン座標系は左上が (0,0)、右下が (screenWidth, screenHeight)。
     * OpenGL の座標系は原点が中央で、X軸は右向き、Y軸は上向き。
     * そのため Y 座標を反転してから ViewBounds にマッピングする。
     *
     */
    fun toWorldCoordDirect(
        x: Float,
        y: Float,
        screenWidth: Int,
        screenHeight: Int,
        viewBounds: ViewBounds
    ): Pair<Float, Float> {
        val worldX = viewBounds.left + (x / screenWidth) * (viewBounds.right - viewBounds.left)
        // OpenGL は Y 軸が上向き、画面は下向きなので反転
        val worldY = viewBounds.top - (y / screenHeight) * (viewBounds.top - viewBounds.bottom)
        return worldX to worldY
    }

}