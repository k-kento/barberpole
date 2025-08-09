package com.gastornisapp.barberpole.ui.percussion

import androidx.compose.foundation.background
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.aspectRatio
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.shape.CircleShape
import androidx.compose.material3.Icon
import androidx.compose.material3.IconButton
import androidx.compose.material3.Scaffold
import androidx.compose.runtime.Composable
import androidx.compose.runtime.DisposableEffect
import androidx.compose.runtime.remember
import androidx.compose.ui.Modifier
import androidx.compose.ui.draw.clip
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.graphics.vector.ImageVector
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.res.vectorResource
import androidx.compose.ui.unit.dp
import com.gastornisapp.soundlib.AudioLib
import com.gastornisapp.soundlib.AudioResource

@Composable
fun PercussionPage() {
    Scaffold { paddingValues ->
        val context = LocalContext.current

        fun createAudioLib(resource: AudioResource): AudioLib {
            return AudioLib().apply {
                create()
                load(context.assets, resource)
            }
        }

        val audioLibs = remember(context.assets) {
            AudioResource.entries.associateWith { type ->
                createAudioLib(type)
            }
        }

        DisposableEffect(Unit) {
            onDispose {
                audioLibs.values.forEach { it.release() }
            }
        }

        Box(
            modifier = Modifier
                .padding(paddingValues)
                .padding(horizontal = 16.dp)
        ) {
            GridButtons(onClick = { type ->
                audioLibs[type.toAudioResource()]?.start()
            })
        }
    }
}

@Composable
fun GridButtons(onClick: (PercussionType) -> Unit) {
    val types = PercussionType.entries
    val half = (types.size + 1) / 2

    Column(verticalArrangement = Arrangement.spacedBy(4.dp)) {
        ButtonsRow(
            types = types.take(half),
            onClick = {
                onClick(it)
            })
        ButtonsRow(
            types = types.drop(half),
            onClick = {
                onClick(it)
            })
    }
}

@Composable
fun ButtonsRow(
    types: List<PercussionType>,
    onClick: (PercussionType) -> Unit
) {
    Row(
        modifier = Modifier.fillMaxWidth(),
        horizontalArrangement = Arrangement.spacedBy(4.dp)
    ) {
        for (type in types) {
            IconButton(
                onClick = {
                    onClick(type)
                },
                modifier = Modifier
                    .weight(1f)
                    .aspectRatio(1f)
                    .clip(CircleShape)
                    .background(Color(type.color or 0xFF000000.toInt()))
            ) {
                Icon(
                    modifier = Modifier.padding(12.dp),
                    imageVector = ImageVector.vectorResource(id = type.drawableRes),
                    tint = Color.White,
                    contentDescription = "",
                )
            }
        }
    }
}
