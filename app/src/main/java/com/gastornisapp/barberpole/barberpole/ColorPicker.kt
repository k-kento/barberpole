package com.gastornisapp.barberpole.barberpole

import androidx.compose.foundation.background
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.size
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
fun ColorPicker(selectedColor: Color, onSelected: (Color) -> Unit, onDismissed: () -> Unit) {
    ModalBottomSheet(
        onDismissRequest = { onDismissed() },
    ) {
        Column(
            modifier = Modifier
                .fillMaxWidth()
                .padding(16.dp),
            horizontalAlignment = Alignment.CenterHorizontally
        ) {
            ColorRow(color = Color.Red, selectedColor = selectedColor, onSelected = onSelected)
            ColorRow(color = Color.Green, selectedColor = selectedColor, onSelected = onSelected)
            ColorRow(color = Color.Blue, selectedColor = selectedColor, onSelected = onSelected)
        }
    }
}

@Composable
private fun ColorRow(color: Color, selectedColor: Color, onSelected: (Color) -> Unit) {
    Row(verticalAlignment = Alignment.CenterVertically) {
        Box(
            Modifier
                .size(24.dp, 24.dp)
                .background(color = color)
        )
        Spacer(modifier = Modifier.height(8.dp))
        RadioButton(
            selected = selectedColor == color,
            onClick = { onSelected(color) },
        )
    }
}