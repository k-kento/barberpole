package com.gastornisapp.barberpole.ui.swarm

import android.annotation.SuppressLint
import android.util.Log
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.material3.Scaffold
import androidx.compose.runtime.Composable
import androidx.compose.runtime.DisposableEffect
import androidx.compose.runtime.remember
import androidx.compose.ui.Modifier
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.viewinterop.AndroidView
import androidx.lifecycle.Lifecycle
import androidx.lifecycle.LifecycleEventObserver
import androidx.lifecycle.compose.LocalLifecycleOwner

@SuppressLint("UnusedMaterial3ScaffoldPaddingParameter")
@Composable
fun SwarmPage() {
    Scaffold { paddingValues ->
        val lifecycleOwner = LocalLifecycleOwner.current
        val context = LocalContext.current
        val glView = remember { SwarmView(context.applicationContext) }

        DisposableEffect(lifecycleOwner) {
            val observer = LifecycleEventObserver { _, event ->
                when (event) {
                    Lifecycle.Event.ON_RESUME -> {
                        Log.d("SwarmPage", "onResume")
                        glView.onResume()
                    }

                    Lifecycle.Event.ON_PAUSE -> {
                        Log.d("SwarmPage", "onPause")
                        glView.onPause()
                    }

                    else -> {}
                }
            }
            lifecycleOwner.lifecycle.addObserver(observer)

            onDispose {
                lifecycleOwner.lifecycle.removeObserver(observer)
            }
        }

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
