package com.gastornisapp.barberpole.ui.barberpole

import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.setValue
import androidx.compose.ui.graphics.Color
import androidx.lifecycle.ViewModel

class BarberPoleViewModel() : ViewModel() {

    var isPlaying by mutableStateOf(true)
        private set

    var orientation by mutableStateOf(Orientation.Right)
        private set

    var sliderPosition by mutableStateOf(1.5f)
        private set

    var speedSheetVisible by mutableStateOf(false)
        private set

    var colorSheetVisible by mutableStateOf(false)
        private set

    var colors by mutableStateOf(Pair(Color.Red, Color.Blue))
        private set

    fun togglePlay() {
        isPlaying = !isPlaying
    }

    fun toggleOrientation() {
        orientation = orientation.toggle()
    }

    fun updateSliderPosition(value: Float) {
        sliderPosition = value
    }

    fun updateColors(first: Color, second: Color) {
        colors = first to second
    }

    fun updateSpeedSheetVisible(visible: Boolean) {
        speedSheetVisible = visible
    }

    fun updateColorSheetVisible(visible: Boolean) {
        colorSheetVisible = visible
    }
}