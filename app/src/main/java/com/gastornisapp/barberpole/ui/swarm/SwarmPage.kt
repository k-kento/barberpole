package com.gastornisapp.barberpole.ui.swarm

import android.annotation.SuppressLint
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.material3.Scaffold
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.viewinterop.AndroidView

@SuppressLint("UnusedMaterial3ScaffoldPaddingParameter")
@Composable
fun SwarmPage() {
    Scaffold { paddingValues ->
        AndroidView(
            modifier = Modifier.fillMaxSize(),
            factory = {
                SwarmView(context = it.applicationContext)
            },
        )
    }
}
