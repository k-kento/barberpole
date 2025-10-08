package com.gastornisapp.barberpole.common

import kotlin.math.sqrt

fun Float.toDegrees(): Float = (this * 180f / Math.PI.toFloat())
fun Float.toRadian(): Float = (this * Math.PI.toFloat() / 180f)

object MathUtil {

    // 2点間の距離の二乗を計算する
    fun distance(x1: Float, y1: Float, x2: Float, y2: Float): Float {
        return sqrt(distanceSquared(x1, y1, x2, y2))
    }

    // 2点間の距離の二乗を計算する
    fun distanceSquared(x1: Float, y1: Float, x2: Float, y2: Float): Float {
        val dx = x2 - x1
        val dy = y2 - y1
        return dx * dx + dy * dy
    }

    // ベクトルの正規化
    fun normalize(x: Float, y: Float): Pair<Float, Float> {
        val mag = sqrt(x * x + y * y)
        return if (mag > 0) {
            Pair(x / mag, y / mag)
        } else {
            Pair(0f, 0f)
        }
    }

    /**
     * ベクトル out を正規化する
     * 長さ0の場合は (0,0) のまま
     */
    fun normalize(out: FloatArray) {
        val x = out[0]
        val y = out[1]
        val mag = sqrt(x * x + y * y)
        if (mag > 0f) {
            out[0] = x / mag
            out[1] = y / mag
        } else {
            out[0] = 0f
            out[1] = 0f
        }
    }

    /**
     * ベクトル out に係数を掛ける
     */
    fun scale(out: FloatArray, factor: Float) {
        out[0] *= factor
        out[1] *= factor
    }

    /**
     * ベクトル out の長さを limit で制限
     */
    fun limit(out: FloatArray, limit: Float) {
        val x = out[0]
        val y = out[1]
        val mag = sqrt(x * x + y * y)
        if (mag > limit && mag > 0f) {
            val factor = limit / mag
            out[0] = x * factor
            out[1] = y * factor
        }
    }

    fun norm(x: Float, y: Float): Float {
        return sqrt(x * x + y * y)
    }

    // 内積を計算する
    fun dot(x1: Float, y1: Float, x2: Float, y2: Float): Float {
        return x1 * x2 + y1 * y2
    }
}