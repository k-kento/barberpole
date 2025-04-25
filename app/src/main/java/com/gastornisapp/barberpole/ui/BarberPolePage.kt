import android.opengl.GLSurfaceView.INVISIBLE
import android.opengl.GLSurfaceView.VISIBLE
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.size
import androidx.compose.foundation.layout.width
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.ColorLens
import androidx.compose.material.icons.filled.Pause
import androidx.compose.material.icons.filled.PlayArrow
import androidx.compose.material.icons.filled.Speed
import androidx.compose.material.icons.filled.SwitchLeft
import androidx.compose.material.icons.filled.SwitchRight
import androidx.compose.material3.ExperimentalMaterial3Api
import androidx.compose.material3.Icon
import androidx.compose.material3.IconButton
import androidx.compose.material3.ModalBottomSheet
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Slider
import androidx.compose.runtime.Composable
import androidx.compose.runtime.DisposableEffect
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableFloatStateOf
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.unit.dp
import androidx.compose.ui.viewinterop.AndroidView
import androidx.lifecycle.Lifecycle
import androidx.lifecycle.LifecycleEventObserver
import androidx.lifecycle.compose.LocalLifecycleOwner
import com.gastornisapp.barberpole.ui.barberpole.BarberPoleView
import com.gastornisapp.barberpole.ui.barberpole.ColorPicker
import com.gastornisapp.barberpole.ui.barberpole.Orientation.Left
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
    var firstColor by remember { mutableStateOf(Color.Red) }
    var secondColor by remember { mutableStateOf(Color.Blue) }

    val context = LocalContext.current
    val lifecycleOwner = LocalLifecycleOwner.current
    val barberPoleView = remember { BarberPoleView(context) }

    Box(modifier = Modifier.fillMaxSize()) {
        DisposableEffect(lifecycleOwner) {
            val observer = LifecycleEventObserver { _, event ->
                when (event) {
                    Lifecycle.Event.ON_RESUME -> {
                        barberPoleView.onResume()
                        barberPoleView.visibility = VISIBLE
                    }

                    Lifecycle.Event.ON_PAUSE -> {
                        barberPoleView.onPause()
                        // 前の画面に戻った時に、表示が一時的に残ってしまう。
                        // この問題を緩和するため、非表示にする。
                        barberPoleView.visibility = INVISIBLE
                    }

                    else -> {}
                }
            }

            val lifecycle = lifecycleOwner.lifecycle
            lifecycle.addObserver(observer)

            onDispose {
                lifecycle.removeObserver(observer)
            }
        }

        AndroidView(
            modifier = Modifier
                .size(100.dp, 500.dp)
                .align(Alignment.Center),
            factory = { barberPoleView },
            update = { view ->
                if (isPlaying) {
                    view.play()
                } else {
                    view.pause()
                }
                view.setOrientation(orientation)
                view.setSpeed(sliderPosition / 1000)
                view.setColors(firstColor = firstColor, secondColor = secondColor)
            },
        )
        Row(
            modifier = Modifier
                .fillMaxWidth()
                .align(Alignment.BottomCenter),
            horizontalArrangement = Arrangement.SpaceAround
        ) {
            IconButton(modifier = Modifier.size(60.dp), onClick = { isPlaying = !isPlaying }) {
                Icon(
                    modifier = Modifier.size(60.dp),
                    imageVector = if (isPlaying) Icons.Default.Pause else Icons.Default.PlayArrow,
                    contentDescription = if (isPlaying) "Pause" else "Play"
                )
            }
            IconButton(
                modifier = Modifier.size(60.dp),
                onClick = { orientation = orientation.toggle() }) {
                Icon(
                    modifier = Modifier.size(60.dp), imageVector = when (orientation) {
                        Left -> Icons.Default.SwitchLeft
                        Right -> Icons.Default.SwitchRight
                    }, contentDescription = "Orientation"
                )
            }
            IconButton(modifier = Modifier.size(60.dp), onClick = { showSpeedBottomSheet = true }) {
                Icon(
                    modifier = Modifier.size(60.dp),
                    imageVector = Icons.Default.Speed,
                    contentDescription = ""
                )
            }
            IconButton(modifier = Modifier.size(60.dp), onClick = {
                showColorBottomSheet = true
            }) {
                Icon(
                    modifier = Modifier.size(60.dp),
                    imageVector = Icons.Default.ColorLens,
                    contentDescription = ""
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
                            onValueChange = { sliderPosition = it })
                    }
                }
            }

            if (showColorBottomSheet) {
                ColorPicker(selectedFirstColor = firstColor,
                    onFirstColorSelected = { firstColor = it },
                    selectedSecondColor = secondColor,
                    onSecondColorSelected = { secondColor = it },
                    onDismissed = { showColorBottomSheet = false })
            }
        }
    }
}
