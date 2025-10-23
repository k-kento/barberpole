package com.gastornisapp.barberpole.ui.kaleidoscope

import androidx.compose.foundation.interaction.MutableInteractionSource
import androidx.compose.foundation.interaction.collectIsPressedAsState
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.size
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.Image
import androidx.compose.material.icons.filled.PhotoSizeSelectLarge
import androidx.compose.material.icons.filled.Rotate90DegreesCcw
import androidx.compose.material.icons.filled.Rotate90DegreesCw
import androidx.compose.material3.BottomAppBar
import androidx.compose.material3.ExperimentalMaterial3Api
import androidx.compose.material3.Icon
import androidx.compose.material3.IconButton
import androidx.compose.material3.ModalBottomSheet
import androidx.compose.material3.Scaffold
import androidx.compose.runtime.Composable
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.runtime.collectAsState
import androidx.compose.runtime.getValue
import androidx.compose.runtime.remember
import androidx.compose.ui.Modifier
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.platform.testTag
import androidx.compose.ui.unit.dp
import androidx.compose.ui.viewinterop.AndroidView
import androidx.hilt.navigation.compose.hiltViewModel
import com.gastornisapp.myvulkan.kaleidoscope.KaleidoscopeView

@OptIn(ExperimentalMaterial3Api::class)
@Composable
// TODO onResume onPause
fun KaleidoscopePage(viewModel: KaleidoscopeViewModel = hiltViewModel()) {
    val context = LocalContext.current

    val rotationState = viewModel.rotationState.collectAsState()
    val selectedImage = viewModel.selectedImage.collectAsState()
    val isImagePickerShowing = viewModel.isImagePickerShowing.collectAsState()
    val imageItems = viewModel.images.collectAsState()

    Scaffold(
        bottomBar = {
            BottomBar(viewModel)
        }) { paddingValues ->

        AndroidView(
            factory = {
                KaleidoscopeView(
                    context = context,
                    vulkanContext = viewModel.vulkanContext,
                )
            },
            modifier = Modifier
                .padding(paddingValues = paddingValues)
                .fillMaxSize(),
            update = { view ->
                view.rotationState = rotationState.value
                view.imagePath = selectedImage.value.kaleidoscopeImage
            },
        )

        if (isImagePickerShowing.value) {
            ModalBottomSheet(
                onDismissRequest = { viewModel.onEvent(KaleidoscopeEvent.ImagePickerDismissed) },
                modifier = Modifier.testTag("ImageBottomSheet")
            ) {
                DrawablePicker(
                    imageItems = imageItems.value,
                    selected = selectedImage.value,
                    onSelect = { imageItem -> viewModel.onEvent(KaleidoscopeEvent.ImageSelected(imageItem)) }
                )
            }
        }
    }
}

@Composable
fun BottomBar(viewModel: KaleidoscopeViewModel) {
    val cwInteraction = remember { MutableInteractionSource() }
    val ccwInteraction = remember { MutableInteractionSource() }

    val isCwPressed by cwInteraction.collectIsPressedAsState()
    val isCcwPressed by ccwInteraction.collectIsPressedAsState()

    LaunchedEffect(isCwPressed) {
        if (isCwPressed) {
            viewModel.onEvent(KaleidoscopeEvent.RotateCW)
        } else {
            viewModel.onEvent(KaleidoscopeEvent.StopRotation)
        }
    }
    LaunchedEffect(isCcwPressed) {
        if (isCcwPressed) {
            viewModel.onEvent(KaleidoscopeEvent.RotateCCW)
        } else {
            viewModel.onEvent(KaleidoscopeEvent.StopRotation)
        }
    }

    BottomAppBar {
        Row(
            modifier = Modifier.fillMaxWidth(), horizontalArrangement = Arrangement.SpaceEvenly
        ) {
            IconButton(
                modifier = Modifier.testTag("ImageButton"),
                onClick = { viewModel.onEvent(KaleidoscopeEvent.SelectButtonClicked) },
            ) {
                Icon(
                    modifier = Modifier.size(60.dp),
                    imageVector = Icons.Default.Image,
                    contentDescription = null,
                )
            }
            IconButton(
                modifier = Modifier.testTag("SizeButton"),
                onClick = {},
            ) {
                Icon(
                    modifier = Modifier.size(60.dp),
                    imageVector = Icons.Default.PhotoSizeSelectLarge,
                    contentDescription = null,
                )
            }
            IconButton(
                modifier = Modifier.testTag("CwButton"),
                onClick = {},
                interactionSource = cwInteraction,
            ) {
                Icon(
                    modifier = Modifier.size(60.dp),
                    imageVector = Icons.Default.Rotate90DegreesCw,
                    contentDescription = null,
                )
            }
            IconButton(
                modifier = Modifier.testTag("CcwButton"),
                onClick = {},
                interactionSource = ccwInteraction,
            ) {
                Icon(
                    modifier = Modifier.size(60.dp),
                    imageVector = Icons.Default.Rotate90DegreesCcw,
                    contentDescription = null,
                )
            }
        }
    }
}
