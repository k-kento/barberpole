package com.gastornisapp.barberpole.ui.page.drawing

import androidx.compose.foundation.interaction.MutableInteractionSource
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.size
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.Image
import androidx.compose.material.icons.filled.Rotate90DegreesCcw
import androidx.compose.material.icons.filled.Rotate90DegreesCw
import androidx.compose.material3.BottomAppBar
import androidx.compose.material3.ExperimentalMaterial3Api
import androidx.compose.material3.Icon
import androidx.compose.material3.IconButton
import androidx.compose.material3.Scaffold
import androidx.compose.runtime.Composable
import androidx.compose.runtime.remember
import androidx.compose.ui.Modifier
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.platform.testTag
import androidx.compose.ui.unit.dp
import androidx.compose.ui.viewinterop.AndroidView
import androidx.hilt.navigation.compose.hiltViewModel
import com.gastornisapp.myvulkan.drawing.DrawingView

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun DrawingPage(viewModel: DrawingViewModel = hiltViewModel()) {
    val context = LocalContext.current

    Scaffold(
        bottomBar = {
            BottomBar(viewModel)
        }) { paddingValues ->

        AndroidView(
            factory = {
                DrawingView(
                    context = context,
                    vulkanContext = viewModel.vulkanContext,
                )
            },
            modifier = Modifier
                .padding(paddingValues = paddingValues)
                .fillMaxSize(),
            update = { view ->
            },
        )

//        if (isImagePickerShowing.value) {
//            ModalBottomSheet(
//                onDismissRequest = { viewModel.onEvent(DrawingEvent.ImagePickerDismissed) },
//                modifier = Modifier.testTag("ImageBottomSheet")
//            ) {
//                DrawablePicker(
//                    imageItems = imageItems.value,
//                    selected = selectedImage.value,
//                    onSelect = { imageItem -> viewModel.onEvent(DrawingEvent.ImageSelected(imageItem)) }
//                )
//            }
//        }
    }
}

@Composable
fun BottomBar(viewModel: DrawingViewModel) {
    val cwInteraction = remember { MutableInteractionSource() }
    val ccwInteraction = remember { MutableInteractionSource() }

    BottomAppBar {
        Row(
            modifier = Modifier.fillMaxWidth(), horizontalArrangement = Arrangement.SpaceEvenly
        ) {
            IconButton(
                modifier = Modifier.testTag("ImageButton"),
                onClick = { },
            ) {
                Icon(
                    modifier = Modifier.size(60.dp),
                    imageVector = Icons.Default.Image,
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
