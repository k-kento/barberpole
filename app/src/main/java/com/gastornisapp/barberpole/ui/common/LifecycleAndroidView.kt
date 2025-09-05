package com.gastornisapp.barberpole.ui.common

import android.content.Context
import android.view.View
import androidx.compose.runtime.Composable
import androidx.compose.runtime.DisposableEffect
import androidx.compose.runtime.remember
import androidx.compose.ui.Modifier
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.viewinterop.AndroidView
import androidx.lifecycle.Lifecycle
import androidx.lifecycle.LifecycleEventObserver
import androidx.lifecycle.compose.LocalLifecycleOwner

@Composable
fun <T : View> LifecycleAndroidView(
    modifier: Modifier = Modifier,
    factory: (Context) -> T,
    update: (T) -> Unit = {},
    onResume: (T) -> Unit = {},
    onPause: (T) -> Unit = {},
    onRelease: (T) -> Unit = {},
) {
    val lifecycleOwner = LocalLifecycleOwner.current
    val context = LocalContext.current

    // View を Compose 内で保持
    val view = remember { factory(context) }

    // ライフサイクル監視
    DisposableEffect(lifecycleOwner, view) {
        val observer = LifecycleEventObserver { _, event ->
            when (event) {
                Lifecycle.Event.ON_RESUME -> onResume(view)
                Lifecycle.Event.ON_PAUSE -> onPause(view)
                else -> Unit
            }
        }
        lifecycleOwner.lifecycle.addObserver(observer)

        onDispose {
            lifecycleOwner.lifecycle.removeObserver(observer)
            onRelease(view)
        }
    }

    AndroidView(
        modifier = modifier,
        factory = { view },
        update = { update(view) }
    )
}