package com.gastornisapp.barberpole.ui.barberpole

import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.size
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.ColorLens
import androidx.compose.material.icons.filled.Pause
import androidx.compose.material.icons.filled.PlayArrow
import androidx.compose.material.icons.filled.Speed
import androidx.compose.material.icons.filled.SwitchLeft
import androidx.compose.material.icons.filled.SwitchRight
import androidx.compose.material3.Icon
import androidx.compose.material3.IconButton
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.hapticfeedback.HapticFeedbackType
import androidx.compose.ui.platform.LocalHapticFeedback
import androidx.compose.ui.unit.dp
import com.gastornisapp.barberpole.ui.barberpole.Orientation.Left
import com.gastornisapp.barberpole.ui.barberpole.Orientation.Right

@Composable
fun BarberPoleBottomBar(
    modifier: Modifier = Modifier,
    isPlaying: Boolean,
    onPlayToggle: () -> Unit,
    onOrientationToggle: () -> Unit,
    onSpeedClick: () -> Unit,
    onColorClick: () -> Unit,
    orientation: Orientation
) {
    val haptic = LocalHapticFeedback.current

    Row(
        modifier = modifier,
        horizontalArrangement = Arrangement.SpaceAround
    ) {
        IconButton(modifier = Modifier.size(60.dp), onClick = {
            haptic.performHapticFeedback(HapticFeedbackType.Confirm)
            onPlayToggle()
        }) {
            Icon(
                modifier = Modifier.size(60.dp),
                imageVector = if (isPlaying) Icons.Default.Pause else Icons.Default.PlayArrow,
                contentDescription = if (isPlaying) "Pause" else "Play"
            )
        }
        IconButton(
            modifier = Modifier.size(60.dp),
            onClick = {
                onOrientationToggle()
                haptic.performHapticFeedback(HapticFeedbackType.Confirm)
            }) {
            Icon(
                modifier = Modifier.size(60.dp), imageVector = when (orientation) {
                    Left -> Icons.Default.SwitchLeft
                    Right -> Icons.Default.SwitchRight
                }, contentDescription = "Orientation"
            )
        }
        IconButton(modifier = Modifier.size(60.dp), onClick = {
            onSpeedClick()
        }) {
            Icon(
                modifier = Modifier.size(60.dp),
                imageVector = Icons.Default.Speed,
                contentDescription = ""
            )
        }
        IconButton(modifier = Modifier.size(60.dp), onClick = {
            onColorClick()
        }) {
            Icon(
                modifier = Modifier.size(60.dp),
                imageVector = Icons.Default.ColorLens,
                contentDescription = ""
            )
        }
    }
}
