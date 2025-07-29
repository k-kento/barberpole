package com.gastornisapp.barberpole.ui.barberpole

import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.setValue
import androidx.compose.ui.graphics.Color
import androidx.lifecycle.ViewModel
import dagger.hilt.android.lifecycle.HiltViewModel
import javax.inject.Inject

@HiltViewModel
open class BarberPoleViewModel @Inject constructor(
) : ViewModel() {

    var isPlaying by mutableStateOf(true)
        private set

    var orientation by mutableStateOf(Orientation.Right)
        private set

    var sliderPosition by mutableStateOf(1.5f)
        private set

    var showSpeedBottomSheet by mutableStateOf(false)
        private set

    var showColorBottomSheet by mutableStateOf(false)
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

    fun setColors(first: Color, second: Color) {
        colors = first to second
    }

    fun updateShowSpeedBottomSheet(show: Boolean) {
        showSpeedBottomSheet = show
    }

    fun updateShowColorBottomSheet(show: Boolean) {
        showColorBottomSheet = show
    }
}