package com.gastornisapp.barberpole.math

fun Float.toDegrees(): Float = (this * 180f / Math.PI.toFloat())
fun Float.toRadian(): Float = (this * Math.PI.toFloat() / 180f)