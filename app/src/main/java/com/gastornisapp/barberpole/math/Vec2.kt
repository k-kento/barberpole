package com.gastornisapp.barberpole.math

import kotlin.math.pow
import kotlin.math.sqrt

data class Vec2(var x: Float = 0f, var y: Float = 0f) {

    operator fun plus(other: Vec2) = Vec2(x + other.x, y + other.y)

    operator fun minus(other: Vec2) = Vec2(x - other.x, y - other.y)

    operator fun times(s: Float) = Vec2(x * s, y * s)

    operator fun div(scalar: Float) = Vec2(x / scalar, y / scalar)

    fun mag(): Float = sqrt(x * x + y * y)

    fun normalize(): Vec2 {
        val m = mag()
        return if (m > 0) Vec2(x / m, y / m) else Vec2(0f, 0f)
    }

    fun limit(max: Float): Vec2 {
        return if (mag() > max) normalize() * max else this
    }

    fun distance(other: Vec2): Float = sqrt((x - other.x).pow(2) + (y - other.y).pow(2))

    fun dot(other: Vec2): Float {
        return x * other.x + y * other.y
    }
}
