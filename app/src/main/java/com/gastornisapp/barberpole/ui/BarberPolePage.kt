import android.opengl.GLSurfaceView.INVISIBLE
import android.opengl.GLSurfaceView.VISIBLE
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.size
import androidx.compose.foundation.layout.width
import androidx.compose.material3.ExperimentalMaterial3Api
import androidx.compose.material3.ModalBottomSheet
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Slider
import androidx.compose.runtime.Composable
import androidx.compose.runtime.DisposableEffect
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableFloatStateOf
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.hapticfeedback.HapticFeedbackType
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.platform.LocalHapticFeedback
import androidx.compose.ui.unit.dp
import androidx.compose.ui.viewinterop.AndroidView
import androidx.lifecycle.Lifecycle
import androidx.lifecycle.LifecycleEventObserver
import androidx.lifecycle.compose.LocalLifecycleOwner
import com.gastornisapp.barberpole.ui.barberpole.BarberPoleBottomBar
import com.gastornisapp.barberpole.ui.barberpole.BarberPoleView
import com.gastornisapp.barberpole.ui.barberpole.ColorPicker
import com.gastornisapp.barberpole.ui.barberpole.Orientation.Right


@Composable
fun BarberPolePage() {
    Scaffold { paddingValues ->
        Box(modifier = Modifier.padding(paddingValues)) {
            Main()
        }
    }
}

@OptIn(ExperimentalMaterial3Api::class)
@Composable
private fun Main() {
    var isPlaying by remember { mutableStateOf(true) }
    var orientation by remember { mutableStateOf(Right) }
    var sliderPosition by remember { mutableFloatStateOf(1.5f) }
    var showSpeedBottomSheet by remember { mutableStateOf(false) }
    var showColorBottomSheet by remember { mutableStateOf(false) }
    var colors by remember { mutableStateOf(Pair(Color.Red, Color.Blue)) }

    val context = LocalContext.current
    val barberPoleView = remember(context) { BarberPoleView(context) }

    val haptic = LocalHapticFeedback.current

    ObserveLifecycle(barberPoleView)

    LaunchedEffect(isPlaying) {
        if (isPlaying) {
            barberPoleView.play()
        } else {
            barberPoleView.pause()
        }
    }

    LaunchedEffect(orientation) {
        barberPoleView.setOrientation(orientation)
    }

    LaunchedEffect(sliderPosition) {
        barberPoleView.setSpeed(sliderPosition / 1000)
    }

    LaunchedEffect(colors) {
        barberPoleView.setColors(firstColor = colors.first, secondColor = colors.second)
    }

    Box(modifier = Modifier.fillMaxSize()) {
        AndroidView(
            modifier = Modifier
                .size(100.dp, 500.dp)
                .align(Alignment.Center),
            factory = { barberPoleView },
        )

        BarberPoleBottomBar(
            modifier = Modifier
                .fillMaxWidth()
                .align(Alignment.BottomCenter),
            isPlaying = isPlaying,
            onPlayToggle = {
                isPlaying = !isPlaying
            },
            onOrientationToggle = {
                orientation = orientation.toggle()
            },
            onSpeedClick = {
                showSpeedBottomSheet = true
            },
            onColorClick = {
                showColorBottomSheet = true
            },
            orientation = orientation
        )
    }

    if (showSpeedBottomSheet) {
        ModalBottomSheet(
            onDismissRequest = { showSpeedBottomSheet = false },
        ) {
            Box(
                modifier = Modifier
                    .fillMaxWidth()
                    .padding(vertical = 64.dp)
            ) {
                Slider(modifier = Modifier
                    .align(Alignment.Center)
                    .width(200.dp),
                    valueRange = 1f..2.0f,
                    value = sliderPosition,
                    steps = 6,
                    onValueChange = {
                        haptic.performHapticFeedback(HapticFeedbackType.SegmentFrequentTick)
                        sliderPosition = it
                    })
            }
        }
    }

    if (showColorBottomSheet) {
        ColorPicker(
            selectedFirstColor = colors.first,
            onFirstColorSelected = {
                haptic.performHapticFeedback(HapticFeedbackType.Confirm)
                colors = colors.copy(first = it)
            },
            selectedSecondColor = colors.second,
            onSecondColorSelected = {
                haptic.performHapticFeedback(HapticFeedbackType.Confirm)
                colors = colors.copy(second = it)
            },
            onDismissed = { showColorBottomSheet = false }
        )
    }
}

@Composable
private fun ObserveLifecycle(view: BarberPoleView) {
    val lifecycleOwner = LocalLifecycleOwner.current

    DisposableEffect(lifecycleOwner) {
        val observer = LifecycleEventObserver { _, event ->
            when (event) {
                Lifecycle.Event.ON_RESUME -> {
                    view.onResume()
                    view.visibility = VISIBLE
                }

                Lifecycle.Event.ON_PAUSE -> {
                    view.onPause()
                    view.visibility = INVISIBLE
                }

                else -> {}
            }
        }

        lifecycleOwner.lifecycle.addObserver(observer)

        onDispose {
            lifecycleOwner.lifecycle.removeObserver(observer)
        }
    }
}