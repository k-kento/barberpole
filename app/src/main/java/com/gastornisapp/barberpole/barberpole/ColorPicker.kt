package com.gastornisapp.barberpole.barberpole

import androidx.compose.foundation.background
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.size
import androidx.compose.foundation.layout.width
import androidx.compose.material3.ExperimentalMaterial3Api
import androidx.compose.material3.ModalBottomSheet
import androidx.compose.material3.RadioButton
import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.unit.dp

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun ColorPicker(
    selectedFirstColor: Color,
    selectedSecondColor: Color,
    onFirstColorSelected: (Color) -> Unit,
    onSecondColorSelected: (Color) -> Unit,
    onDismissed: () -> Unit
) {
    ModalBottomSheet(
        onDismissRequest = { onDismissed() },
    ) {
        Column(
            modifier = Modifier
                .fillMaxWidth()
                .padding(16.dp),
            horizontalAlignment = Alignment.CenterHorizontally
        ) {
            ColorRow(
                color = Color.Red,
                selectedFirstColor = selectedFirstColor,
                onFirstColorSelected = onFirstColorSelected,
                selectedSecondColor = selectedSecondColor,
                onSecondColorSelected = onSecondColorSelected
            )
            ColorRow(
                color = Color.Blue,
                selectedFirstColor = selectedFirstColor,
                onFirstColorSelected = onFirstColorSelected,
                selectedSecondColor = selectedSecondColor,
                onSecondColorSelected = onSecondColorSelected
            )
            ColorRow(
                color = Color.Green,
                selectedFirstColor = selectedFirstColor,
                onFirstColorSelected = onFirstColorSelected,
                selectedSecondColor = selectedSecondColor,
                onSecondColorSelected = onSecondColorSelected
            )
            ColorRow(
                color = Color.Yellow,
                selectedFirstColor = selectedFirstColor,
                onFirstColorSelected = onFirstColorSelected,
                selectedSecondColor = selectedSecondColor,
                onSecondColorSelected = onSecondColorSelected
            )
            ColorRow(
                color = Color.Magenta,
                selectedFirstColor = selectedFirstColor,
                onFirstColorSelected = onFirstColorSelected,
                selectedSecondColor = selectedSecondColor,
                onSecondColorSelected = onSecondColorSelected
            )
        }
    }
}

@Composable
private fun ColorRow(
    color: Color,
    selectedFirstColor: Color,
    selectedSecondColor: Color,
    onFirstColorSelected: (Color) -> Unit,
    onSecondColorSelected: (Color) -> Unit,
) {
    Row(verticalAlignment = Alignment.CenterVertically) {
        Box(
            Modifier
                .size(32.dp, 32.dp)
                .background(color = color)
        )
        Spacer(modifier = Modifier.width(8.dp))
        RadioButton(
            selected = selectedFirstColor == color,
            onClick = { onFirstColorSelected(color) },
        )
        RadioButton(
            selected = selectedSecondColor == color,
            onClick = { onSecondColorSelected(color) },
        )
    }
}