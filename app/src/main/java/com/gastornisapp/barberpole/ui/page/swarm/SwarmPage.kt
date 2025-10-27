package com.gastornisapp.barberpole.ui.page.swarm

import android.annotation.SuppressLint
import android.util.Log
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.material3.Scaffold
import androidx.compose.runtime.Composable
import androidx.compose.runtime.remember
import androidx.compose.ui.Modifier
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.viewinterop.AndroidView
import androidx.lifecycle.compose.LocalLifecycleOwner

@SuppressLint("UnusedMaterial3ScaffoldPaddingParameter")
@Composable
fun SwarmPage() {
    Scaffold { paddingValues ->
        val lifecycleOwner = LocalLifecycleOwner.current
        val context = LocalContext.current
        val glView = remember { SwarmView(context.applicationContext, lifecycleOwner.lifecycle) }

        AndroidView(
            factory = { glView },
            modifier = Modifier.fillMaxSize(),
            onRelease = { view ->
                Log.d("SwarmPage", "onRelease")
                view.release()
            }
        )
    }
}
