package com.gastornisapp.barberpole.ui.vehicle

import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.Scaffold
import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.hapticfeedback.HapticFeedbackType
import androidx.compose.ui.platform.LocalHapticFeedback
import androidx.compose.ui.viewinterop.AndroidView

@Composable
fun VehiclePage() {

    Scaffold { paddingValues ->
        Box(
            modifier = Modifier
                .fillMaxSize()
                .padding(paddingValues)
        ) {
            val haptic = LocalHapticFeedback.current

            AndroidView(
                modifier = Modifier
                    .fillMaxSize()
                    .align(Alignment.Center),
                factory = {
                    VehicleView(it).apply {
                        this.onVehicleTouchStart = {
                            haptic.performHapticFeedback(HapticFeedbackType.LongPress)
                        }
                    }
                },
            )
        }
    }
}
