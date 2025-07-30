package com.gastornisapp.barberpole.ui.barberpole

import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.Checkbox
import androidx.compose.material3.CheckboxDefaults
import androidx.compose.material3.ExperimentalMaterial3Api
import androidx.compose.material3.HorizontalDivider
import androidx.compose.material3.ModalBottomSheet
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.draw.scale
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.graphics.toArgb
import androidx.compose.ui.platform.testTag
import androidx.compose.ui.unit.dp

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun ColorBottomSheet(
    show: Boolean,
    selectedFirstColor: Color,
    selectedSecondColor: Color,
    onFirstColorSelected: (Color) -> Unit,
    onSecondColorSelected: (Color) -> Unit,
    onDismissed: () -> Unit
) {
    if (!show) return

    val colors = listOf(
        Color.Red, Color.Blue, Color.Green, Color.Yellow, Color.Magenta, Color.Black
    )

    ModalBottomSheet(
        onDismissRequest = { onDismissed() },
        modifier = Modifier.testTag("ColorBottomSheet")
    ) {
        Column(modifier = Modifier.padding(vertical = 16.dp)) {
            Row(
                modifier = Modifier
                    .fillMaxWidth()
                    .padding(vertical = 16.dp)
                    .testTag("ColorBottomSheet_TopRow"),
                horizontalArrangement = Arrangement.Center
            ) {
                colors.forEach { color ->
                    ColorItem(
                        color = color,
                        selectedColor = selectedFirstColor,
                        onColorSelected = onFirstColorSelected
                    )
                }
            }
            HorizontalDivider()
            Row(
                modifier = Modifier
                    .fillMaxWidth()
                    .padding(vertical = 16.dp)
                    .testTag("ColorBottomSheet_BottomRow"),
                horizontalArrangement = Arrangement.Center
            ) {
                colors.forEach { color ->
                    ColorItem(
                        color = color,
                        selectedColor = selectedSecondColor,
                        onColorSelected = onSecondColorSelected
                    )
                }
            }
        }
    }
}

@Composable
private fun ColorItem(
    color: Color,
    selectedColor: Color,
    onColorSelected: (Color) -> Unit,
) {
    Checkbox(
        modifier = Modifier
            .scale(1.8f)
            .padding(2.dp)
            .testTag("ColorBottomSheet_ColorItem_${color.toArgb()}"),
        checked = color == selectedColor,
        onCheckedChange = { onColorSelected(color) },
        colors = CheckboxDefaults.colors(
            checkedColor = color,
            uncheckedColor = color,
            checkmarkColor = Color.Transparent
        ),
    )
}