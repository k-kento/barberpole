package com.gastornisapp.barberpole.ui.kaleidoscope

import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.material3.Scaffold
import androidx.compose.runtime.Composable
import androidx.compose.runtime.remember
import androidx.compose.ui.Modifier
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.viewinterop.AndroidView
import androidx.hilt.navigation.compose.hiltViewModel
import com.gastornisapp.myvulkan.kaleidoscope.KaleidoscopeView

@Composable
// TODO onResume onPause
fun KaleidoscopePage(viewModel: KaleidoscopeViewModel = hiltViewModel()) {

    Scaffold { paddingValues ->
        val context = LocalContext.current
        val view = remember {
            KaleidoscopeView(
                context = context.applicationContext,
                vulkanContext = viewModel.vulkanContext
            )
        }

        AndroidView(
            factory = { view },
            modifier = Modifier.fillMaxSize(),
            onRelease = { view ->
//                view.release()
            }
        )
    }
}

