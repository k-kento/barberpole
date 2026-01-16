package com.gastornisapp.barberpole.ui.page.drawing

import androidx.compose.foundation.BorderStroke
import androidx.compose.foundation.clickable
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.aspectRatio
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.size
import androidx.compose.foundation.lazy.grid.GridCells
import androidx.compose.foundation.lazy.grid.LazyVerticalGrid
import androidx.compose.foundation.lazy.grid.items
import androidx.compose.foundation.shape.RoundedCornerShape
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.Brush
import androidx.compose.material.icons.filled.Circle
import androidx.compose.material.icons.filled.Flare
import androidx.compose.material.icons.filled.Palette
import androidx.compose.material3.AlertDialog
import androidx.compose.material3.BottomAppBar
import androidx.compose.material3.Card
import androidx.compose.material3.CardDefaults
import androidx.compose.material3.ExperimentalMaterial3Api
import androidx.compose.material3.Icon
import androidx.compose.material3.IconButton
import androidx.compose.material3.MaterialTheme
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
    var currentBrushType by remember { mutableStateOf(BrushType.Circle) }
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
            LazyVerticalGrid(
                columns = GridCells.Fixed(2),
                horizontalArrangement = Arrangement.spacedBy(12.dp),
                verticalArrangement = Arrangement.spacedBy(12.dp)
            ) {
                items(BrushType.entries) { brushType ->
                    BrushGridItem(
                        brushType = brushType,
                        isSelected = brushType == currentBrushType,
                        onClick = { onBrushSelected(brushType) }
                    )
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

@Composable
fun BrushGridItem(
    brushType: BrushType,
    isSelected: Boolean,
    onClick: () -> Unit
) {
    val icon = when (brushType) {
        BrushType.Normal -> Icons.Default.Brush
        BrushType.Rainbow -> Icons.Default.Palette
        BrushType.Glow -> Icons.Default.Flare
        BrushType.Circle -> Icons.Default.Circle
    }

    Card(
        modifier = Modifier
            .aspectRatio(1f)
            .clickable(onClick = onClick),
        shape = RoundedCornerShape(12.dp),
        colors = CardDefaults.cardColors(
            containerColor = if (isSelected)
                MaterialTheme.colorScheme.primaryContainer
            else
                MaterialTheme.colorScheme.surface
        ),
        border = if (isSelected)
            BorderStroke(2.dp, MaterialTheme.colorScheme.primary)
        else
            BorderStroke(1.dp, MaterialTheme.colorScheme.outline)
    ) {
        Box(
            modifier = Modifier.fillMaxSize(),
            contentAlignment = Alignment.Center
        ) {
            Column(
                horizontalAlignment = Alignment.CenterHorizontally,
                verticalArrangement = Arrangement.Center
            ) {
                Icon(
                    imageVector = icon,
                    contentDescription = brushType.name,
                    modifier = Modifier.size(32.dp),
                    tint = if (isSelected)
                        MaterialTheme.colorScheme.primary
                    else
                        MaterialTheme.colorScheme.onSurface
                )
                Text(
                    text = brushType.name,
                    style = MaterialTheme.typography.labelMedium,
                    modifier = Modifier.padding(top = 4.dp),
                    color = if (isSelected)
                        MaterialTheme.colorScheme.primary
                    else
                        MaterialTheme.colorScheme.onSurface
                )
            }
        }
    }
}
