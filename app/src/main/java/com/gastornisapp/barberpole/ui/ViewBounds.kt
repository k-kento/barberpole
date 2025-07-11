package com.gastornisapp.barberpole.ui

data class ViewBounds(
    val left: Float,
    val right: Float,
    val bottom: Float,
    val top: Float
) {
    val width = right - left
    val height = top - bottom
}