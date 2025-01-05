import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.size
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
import androidx.compose.material3.Slider
import androidx.compose.runtime.Composable
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableFloatStateOf
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.platform.LocalLifecycleOwner
import androidx.compose.ui.unit.dp
import androidx.compose.ui.viewinterop.AndroidView
import com.gastornisapp.barberpole.barberpole.BarberPoleView
import com.gastornisapp.barberpole.barberpole.ColorPicker
import com.gastornisapp.barberpole.barberpole.Orientation
import com.gastornisapp.barberpole.barberpole.Orientation.Left
import com.gastornisapp.barberpole.barberpole.Orientation.Right


@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun BarberPolePage(modifier: Modifier = Modifier) {
    val isPlaying = remember { mutableStateOf(true) }
    val orientation = remember { mutableStateOf(Orientation.Right) }
    val sliderPosition = remember { mutableFloatStateOf(0f) }
    val showSpeedBottomSheet = remember { mutableStateOf(false) }
    val showColorBottomSheet = remember { mutableStateOf(false) }
    var firstColor by remember { mutableStateOf(Color.Red) }
    val secondColor by remember { mutableStateOf(Color.Blue) }

    Box(modifier = modifier.fillMaxSize()) {
        val lifecycle = LocalLifecycleOwner.current.lifecycle
        AndroidView(
            modifier = Modifier
                .size(100.dp, 400.dp)
                .align(Alignment.Center),
            factory = { context -> BarberPoleView(context) },
            update = { view ->
                view.lifecycle = lifecycle
                if (isPlaying.value) {
                    view.play()
                } else {
                    view.pause()
                }
                view.setOrientation(orientation.value)
                view.setSpeed(sliderPosition.floatValue / 1000)
                view.setColors(firstColor = firstColor, secondColor = secondColor)
            },
            onRelease = { view ->
                // Need to release the lifecycle to prevent a memory leak
                view.lifecycle = null
            },
        )
        Row(
            modifier = Modifier
                .fillMaxWidth()
                .align(Alignment.BottomCenter),
            horizontalArrangement = Arrangement.SpaceAround
        ) {
            IconButton(modifier = Modifier.size(60.dp),
                onClick = { isPlaying.value = !isPlaying.value }) {
                Icon(
                    modifier = Modifier.size(60.dp),
                    imageVector = if (isPlaying.value) Icons.Default.Pause else Icons.Default.PlayArrow,
                    contentDescription = if (isPlaying.value) "Pause" else "Play"
                )
            }
            IconButton(modifier = Modifier.size(60.dp),
                onClick = { orientation.value = orientation.value.toggle() }) {
                Icon(
                    modifier = Modifier.size(60.dp),
                    imageVector = when (orientation.value) {
                        Left -> Icons.Default.SwitchLeft
                        Right -> Icons.Default.SwitchRight
                    },
                    contentDescription = "Orientation"
                )
            }
            IconButton(modifier = Modifier.size(60.dp),
                onClick = { showSpeedBottomSheet.value = true }) {
                Icon(
                    modifier = Modifier.size(60.dp),
                    imageVector = Icons.Default.Speed,
                    contentDescription = ""
                )
            }
            IconButton(modifier = Modifier.size(60.dp),
                onClick = {
                    showColorBottomSheet.value = true
                }) {
                Icon(
                    modifier = Modifier.size(60.dp),
                    imageVector = Icons.Default.ColorLens,
                    contentDescription = ""
                )
            }

            if (showSpeedBottomSheet.value) {
                ModalBottomSheet(
                    onDismissRequest = { showSpeedBottomSheet.value = false },
                ) {
                    Column(
                        modifier = Modifier
                            .fillMaxWidth()
                            .padding(16.dp),
                        horizontalAlignment = Alignment.CenterHorizontally
                    ) {
                        Slider(
                            valueRange = 1f..2f,
                            steps = 2,
                            value = sliderPosition.floatValue,
                            onValueChange = { sliderPosition.floatValue = it }
                        )
                    }
                }
            }

            if (showColorBottomSheet.value) {
                ColorPicker(
                    selectedColor = firstColor,
                    onSelected = { firstColor = it },
                    onDismissed = { showColorBottomSheet.value = false }
                )
            }
        }
    }
}
