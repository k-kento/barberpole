package com.gastornisapp.barberpole.ui.page.barberpole

import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.width
import androidx.compose.material3.ExperimentalMaterial3Api
import androidx.compose.material3.ModalBottomSheet
import androidx.compose.material3.Slider
import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.platform.testTag
import androidx.compose.ui.unit.dp

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun SpeedBottomSheet(
    show: Boolean,
    sliderPosition: Float,
    onValueChange: (Float) -> Unit,
    onDismiss: () -> Unit
) {
    if (!show) return

    ModalBottomSheet(
        onDismissRequest = onDismiss,
        modifier = Modifier.testTag("SpeedBottomSheet")
    ) {
        Box(
            modifier = Modifier
                .fillMaxWidth()
                .padding(vertical = 64.dp)
        ) {
            Slider(
                modifier = Modifier
                    .align(Alignment.Center)
                    .width(200.dp)
                    .testTag("SpeedBottomSheet_SpeedSlider"),
                valueRange = 1f..2.0f,
                value = sliderPosition,
                steps = 6,
                onValueChange = onValueChange
            )
        }
    }
}