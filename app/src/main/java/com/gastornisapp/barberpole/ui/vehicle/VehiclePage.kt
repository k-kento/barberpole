package com.gastornisapp.barberpole.ui.vehicle

import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.Scaffold
import androidx.compose.runtime.Composable
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.hapticfeedback.HapticFeedbackType
import androidx.compose.ui.platform.LocalHapticFeedback
import androidx.compose.ui.viewinterop.AndroidView
import androidx.lifecycle.compose.LocalLifecycleOwner
import com.gastornisapp.barberpole.ui.vehicle.model.VehicleType

@Composable
fun VehiclePage() {

    val addVehicleEvent = remember { mutableStateOf<VehicleType?>(null) }

    Scaffold(
        bottomBar = {
            BottomBar(onAddVehicle = {
                addVehicleEvent.value = it
            })
        }
    ) { paddingValues ->
        Box(
            modifier = Modifier
                .fillMaxSize()
                .padding(paddingValues)
        ) {
            val haptic = LocalHapticFeedback.current
            val lifecycle = LocalLifecycleOwner.current.lifecycle

            AndroidView(
                modifier = Modifier
                    .fillMaxSize()
                    .align(Alignment.Center),
                factory = {
                    VehicleView(it, lifecycle).apply {
                        this.onVehicleTouchStart = {
                            haptic.performHapticFeedback(HapticFeedbackType.LongPress)
                        }
                    }
                },
                update = { view ->
                    val vehicleType = addVehicleEvent.value
                    if (vehicleType != null) {
                        view.addVehicle(vehicleType)
                        addVehicleEvent.value = null
                    }
                },
                onRelease = { it.release() }
            )
        }
    }
}
