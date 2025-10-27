import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.aspectRatio
import androidx.compose.foundation.layout.fillMaxHeight
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.Scaffold
import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.hapticfeedback.HapticFeedbackType
import androidx.compose.ui.platform.LocalHapticFeedback
import androidx.compose.ui.platform.testTag
import androidx.compose.ui.viewinterop.AndroidView
import androidx.hilt.navigation.compose.hiltViewModel
import androidx.lifecycle.compose.LocalLifecycleOwner
import com.gastornisapp.barberpole.ui.page.barberpole.BarberPoleBottomBar
import com.gastornisapp.barberpole.ui.page.barberpole.BarberPoleView
import com.gastornisapp.barberpole.ui.page.barberpole.BarberPoleViewModel
import com.gastornisapp.barberpole.ui.page.barberpole.ColorBottomSheet
import com.gastornisapp.barberpole.ui.page.barberpole.SpeedBottomSheet

@Composable
fun BarberPolePage(viewModel: BarberPoleViewModel = hiltViewModel()) {
    val haptic = LocalHapticFeedback.current
    val lifecycle = LocalLifecycleOwner.current.lifecycle

    Scaffold { paddingValues ->
        Column(modifier = Modifier.padding(paddingValues)) {

            Box(
                modifier = Modifier
                    .weight(1f)
                    .fillMaxWidth(),
                contentAlignment = Alignment.Center
            ) {
                AndroidView(
                    modifier = Modifier
                        .fillMaxHeight(0.7f)
                        .aspectRatio(0.2f)
                        .testTag("BarberPoleViewComposable"),
                    factory = { context -> BarberPoleView(context, lifecycle = lifecycle) },
                    update = {
                        it.apply {
                            setPlaying(viewModel.isPlaying)
                            setSpeed(viewModel.sliderPosition / 1000f)
                            setOrientation(viewModel.orientation)
                            setColors(viewModel.colors.first, viewModel.colors.second)
                        }
                    },
                    onRelease = { it.release() }
                )
            }

            BarberPoleBottomBar(
                modifier = Modifier
                    .fillMaxWidth()
                    .testTag("BarberPoleBottomBar"),
                isPlaying = viewModel.isPlaying,
                onPlayToggle = viewModel::togglePlay,
                onOrientationToggle = viewModel::toggleOrientation,
                onSpeedClick = { viewModel.updateSpeedSheetVisible(true) },
                onColorClick = { viewModel.updateColorSheetVisible(true) },
                orientation = viewModel.orientation
            )

            SpeedBottomSheet(
                show = viewModel.speedSheetVisible,
                sliderPosition = viewModel.sliderPosition,
                onValueChange = {
                    haptic.performHapticFeedback(HapticFeedbackType.SegmentFrequentTick)
                    viewModel.updateSliderPosition(it)
                },
                onDismiss = { viewModel.updateSpeedSheetVisible(false) }
            )

            ColorBottomSheet(
                show = viewModel.colorSheetVisible,
                selectedFirstColor = viewModel.colors.first,
                onFirstColorSelected = {
                    haptic.performHapticFeedback(HapticFeedbackType.Confirm)
                    viewModel.updateColors(it, viewModel.colors.second)
                },
                selectedSecondColor = viewModel.colors.second,
                onSecondColorSelected = {
                    haptic.performHapticFeedback(HapticFeedbackType.Confirm)
                    viewModel.updateColors(viewModel.colors.first, it)
                },
                onDismissed = { viewModel.updateColorSheetVisible(false) }
            )
        }
    }
}
