import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.size
import androidx.compose.material3.Scaffold
import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.hapticfeedback.HapticFeedbackType
import androidx.compose.ui.platform.LocalHapticFeedback
import androidx.compose.ui.unit.dp
import androidx.hilt.navigation.compose.hiltViewModel
import com.gastornisapp.barberpole.ui.barberpole.BarberPoleBottomBar
import com.gastornisapp.barberpole.ui.barberpole.BarberPoleViewComposable
import com.gastornisapp.barberpole.ui.barberpole.BarberPoleViewModel
import com.gastornisapp.barberpole.ui.barberpole.ColorPicker
import com.gastornisapp.barberpole.ui.barberpole.SpeedBottomSheet

@Composable
fun BarberPolePage(viewModel: BarberPoleViewModel = hiltViewModel()) {
    Scaffold { paddingValues ->
        Box(modifier = Modifier.padding(paddingValues)) {
            val haptic = LocalHapticFeedback.current

            Box(modifier = Modifier.fillMaxSize()) {
                BarberPoleViewComposable(
                    modifier = Modifier
                        .size(100.dp, 500.dp)
                        .align(Alignment.Center),
                    isPlaying = viewModel.isPlaying,
                    orientation = viewModel.orientation,
                    sliderPosition = viewModel.sliderPosition,
                    firstColor = viewModel.colors.first,
                    secondColor = viewModel.colors.second
                )

                BarberPoleBottomBar(
                    modifier = Modifier
                        .fillMaxWidth()
                        .align(Alignment.BottomCenter),
                    isPlaying = viewModel.isPlaying,
                    onPlayToggle = viewModel::togglePlay,
                    onOrientationToggle = viewModel::toggleOrientation,
                    onSpeedClick = { viewModel.updateShowSpeedBottomSheet(true) },
                    onColorClick = { viewModel.updateShowColorBottomSheet(true) },
                    orientation = viewModel.orientation
                )

                SpeedBottomSheet(
                    show = viewModel.showSpeedBottomSheet,
                    sliderPosition = viewModel.sliderPosition,
                    onValueChange = {
                        haptic.performHapticFeedback(HapticFeedbackType.SegmentFrequentTick)
                        viewModel.updateSliderPosition(it)
                    },
                    onDismiss = { viewModel.updateShowSpeedBottomSheet(false) }
                )

                ColorPicker(
                    show = viewModel.showColorBottomSheet,
                    selectedFirstColor = viewModel.colors.first,
                    onFirstColorSelected = {
                        haptic.performHapticFeedback(HapticFeedbackType.Confirm)
                        viewModel.setColors(it, viewModel.colors.second)
                    },
                    selectedSecondColor = viewModel.colors.second,
                    onSecondColorSelected = {
                        haptic.performHapticFeedback(HapticFeedbackType.Confirm)
                        viewModel.setColors(viewModel.colors.first, it)
                    },
                    onDismissed = { viewModel.updateShowColorBottomSheet(false) }
                )
            }
        }
    }
}
