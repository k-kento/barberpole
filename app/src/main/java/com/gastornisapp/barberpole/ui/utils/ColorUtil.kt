package com.gastornisapp.barberpole.ui.utils

import android.graphics.Color
import kotlin.math.max
import kotlin.math.min
import kotlin.random.Random

fun adjustPressedColor(color: Int): Int {
    val r = (color shr 16) and 0xFF
    val g = (color shr 8) and 0xFF
    val b = color and 0xFF

    // 明るさ判定（0～255）
    val luminance = 0.299 * r + 0.587 * g + 0.114 * b

    return if (luminance < 128) {
        // 暗い色 → 明るく（白寄りに）
        Color.rgb(
            min(r + 60, 255),
            min(g + 60, 255),
            min(b + 60, 255)
        )
    } else {
        // 明るい色 → 暗く（黒寄りに）
        Color.rgb(
            max(r - 60, 0),
            max(g - 60, 0),
            max(b - 60, 0)
        )
    }
}

@Deprecated("colorToFloatArrayを使用してください")

fun colorCodeToFloatArray(colorInt: Int): FloatArray {
    val r = Color.red(colorInt) / 255f
    val g = Color.green(colorInt) / 255f
    val b = Color.blue(colorInt) / 255f
    val a = Color.alpha(colorInt) / 255f
    return floatArrayOf(r, g, b, a)
}

fun colorToFloatArray(colorInt: Int): FloatArray {
    val r = Color.red(colorInt) / 255f
    val g = Color.green(colorInt) / 255f
    val b = Color.blue(colorInt) / 255f
    val a = Color.alpha(colorInt) / 255f
    return floatArrayOf(r, g, b, a)
}

object ColorUtil {

    /**
     * 指定したベース色に近い色を生成
     *
     * @param baseHue 基準となる色相 (0..360)
     * @param count 生成する色の数
     */
    fun generateSimilarColors(baseHue: Float, count: Int): List<Int> {
        val colors = mutableListOf<Int>()
        for (i in 0 until count) {
            val hue = (baseHue + Random.nextFloat() * 20f - 10f) % 360f // ±10°以内
            val saturation = 0.7f + Random.nextFloat() * 0.3f // 0.7〜1.0
            val value = 0.7f + Random.nextFloat() * 0.3f // 0.7〜1.0
            colors.add(Color.HSVToColor(floatArrayOf(hue, saturation, value)))
        }
        return colors
    }
}
