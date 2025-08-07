package com.gastornisapp.barberpole.ui.harmony

import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.Scaffold
import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.viewinterop.AndroidView

@Composable
fun HarmonyPage() {
    Scaffold { paddingValues ->
        Box(modifier = Modifier.padding(paddingValues)) {
            AndroidView(
                modifier = Modifier
                    .fillMaxSize()
                    .align(Alignment.Center),
                factory = { HarmonyView(context = it) },
            )
        }
    }
}
