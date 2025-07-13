package com.gastornisapp.barberpole.ui.percussion

import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.Scaffold
import androidx.compose.runtime.Composable
import androidx.compose.runtime.DisposableEffect
import androidx.compose.runtime.remember
import androidx.compose.ui.Modifier
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.viewinterop.AndroidView
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

        AndroidView(
            modifier = Modifier
                .fillMaxSize()
                .padding(paddingValues),
            factory = {
                PercussionView(context = it).apply {
                    onClicked = { type ->
                        audioLibs[type.toAudioResource()]?.start()
                    }
                }
            },
        )
    }
}
