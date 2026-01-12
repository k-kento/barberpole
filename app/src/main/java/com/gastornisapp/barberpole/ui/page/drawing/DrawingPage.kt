package com.gastornisapp.barberpole.ui.page.drawing

import androidx.compose.foundation.clickable
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.size
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.Brush
import androidx.compose.material3.AlertDialog
import androidx.compose.material3.BottomAppBar
import androidx.compose.material3.ExperimentalMaterial3Api
import androidx.compose.material3.Icon
import androidx.compose.material3.IconButton
import androidx.compose.material3.RadioButton
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Text
import androidx.compose.material3.TextButton
import androidx.compose.runtime.Composable
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.platform.testTag
import androidx.compose.ui.unit.dp
import androidx.compose.ui.viewinterop.AndroidView
import androidx.hilt.navigation.compose.hiltViewModel
import com.gastornisapp.myvulkan.drawing.BrushType
import com.gastornisapp.myvulkan.drawing.DrawingView

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun DrawingPage(viewModel: DrawingViewModel = hiltViewModel()) {
    val context = LocalContext.current
    var showBrushDialog by remember { mutableStateOf(false) }
    var currentBrushType by remember { mutableStateOf(BrushType.Rainbow) }
    var drawingView by remember { mutableStateOf<DrawingView?>(null) }

    if (showBrushDialog) {
        BrushSelectionDialog(
            currentBrushType = currentBrushType,
            onBrushSelected = { brushType ->
                currentBrushType = brushType
                drawingView?.setBrushType(brushType)
                showBrushDialog = false
            },
            onDismiss = { showBrushDialog = false }
        )
    }

    Scaffold(
        bottomBar = {
            BottomBar(onBrushClick = { showBrushDialog = true })
        }) { paddingValues ->

        AndroidView(
            factory = {
                DrawingView(
                    context = context,
                    vulkanContext = viewModel.vulkanContext,
                ).also { drawingView = it }
            },
            modifier = Modifier
                .padding(paddingValues = paddingValues)
                .fillMaxSize(),
            update = { view ->
            },
        )
    }
}

@Composable
fun BottomBar(onBrushClick: () -> Unit) {
    BottomAppBar {
        Row(
            modifier = Modifier.fillMaxWidth(), horizontalArrangement = Arrangement.SpaceEvenly
        ) {
            IconButton(
                modifier = Modifier.testTag("BrushButton"),
                onClick = onBrushClick,
            ) {
                Icon(
                    modifier = Modifier.size(60.dp),
                    imageVector = Icons.Default.Brush,
                    contentDescription = null,
                )
            }
        }
    }
}

@Composable
fun BrushSelectionDialog(
    currentBrushType: BrushType,
    onBrushSelected: (BrushType) -> Unit,
    onDismiss: () -> Unit
) {
    AlertDialog(
        onDismissRequest = onDismiss,
        title = { Text("Select Brush") },
        text = {
            Column {
                BrushType.entries.forEach { brushType ->
                    Row(
                        modifier = Modifier
                            .fillMaxWidth()
                            .clickable { onBrushSelected(brushType) }
                            .padding(vertical = 8.dp),
                        verticalAlignment = Alignment.CenterVertically
                    ) {
                        RadioButton(
                            selected = brushType == currentBrushType,
                            onClick = { onBrushSelected(brushType) }
                        )
                        Text(
                            text = brushType.name,
                            modifier = Modifier.padding(start = 8.dp)
                        )
                    }
                }
            }
        },
        confirmButton = {
            TextButton(onClick = onDismiss) {
                Text("Cancel")
            }
        }
    )
}
