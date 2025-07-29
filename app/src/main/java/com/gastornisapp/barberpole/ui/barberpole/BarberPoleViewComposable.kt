package com.gastornisapp.barberpole.ui.barberpole

import androidx.compose.runtime.Composable
import androidx.compose.runtime.DisposableEffect
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.setValue
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.viewinterop.AndroidView
import androidx.lifecycle.Lifecycle
import androidx.lifecycle.LifecycleEventObserver
import androidx.lifecycle.compose.LocalLifecycleOwner

@Composable
fun BarberPoleViewComposable(
    modifier: Modifier,
    isPlaying: Boolean,
    orientation: Orientation,
    sliderPosition: Float,
    firstColor: Color,
    secondColor: Color,
) {
    val lifecycleOwner = LocalLifecycleOwner.current

    var isResumed: Boolean? by mutableStateOf(null)

    DisposableEffect(lifecycleOwner) {
        val observer = LifecycleEventObserver { _, event ->
            isResumed = when (event) {
                Lifecycle.Event.ON_RESUME -> true
                Lifecycle.Event.ON_PAUSE -> false
                else -> isResumed
            }
        }
        lifecycleOwner.lifecycle.addObserver(observer)
        onDispose {
            lifecycleOwner.lifecycle.removeObserver(observer)
        }
    }

    AndroidView(
        modifier = modifier,
        factory = { context ->
            BarberPoleView(context)
        },
        update = { view ->
            view.apply {
                isResumed?.let {
                    setResumed(it)
                }
                setPlaying(isPlaying)
                setSpeed(sliderPosition / 1000)
                setOrientation(orientation)
                setColors(firstColor = firstColor, secondColor = secondColor)
            }
        }
    )
}