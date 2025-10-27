package com.gastornisapp.barberpole.ui

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.activity.viewModels
import androidx.compose.runtime.collectAsState
import androidx.compose.runtime.getValue
import androidx.core.splashscreen.SplashScreen.Companion.installSplashScreen
import androidx.core.view.WindowInsetsCompat
import androidx.core.view.WindowInsetsControllerCompat
import com.gastornisapp.barberpole.ui.page.LoadingPage
import com.gastornisapp.barberpole.ui.theme.BarberPoleTheme
import dagger.hilt.android.AndroidEntryPoint

@AndroidEntryPoint
class MainActivity : ComponentActivity() {

    private val viewModel: AppStartupViewModel by viewModels()

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        enableEdgeToEdge()

        // Immersive Sticky
        WindowInsetsControllerCompat(window, window.decorView).let { controller ->
            controller.hide(WindowInsetsCompat.Type.systemBars())
            controller.systemBarsBehavior = WindowInsetsControllerCompat.BEHAVIOR_SHOW_TRANSIENT_BARS_BY_SWIPE
        }

        val splashScreen = installSplashScreen()
        splashScreen.setKeepOnScreenCondition { viewModel.uiStatus.value == AppStartupUiStatus.ShowingSplash }

        viewModel.initialize()

        setContent {
            BarberPoleTheme {
                val status by viewModel.uiStatus.collectAsState()
                when (val status = status) {
                    AppStartupUiStatus.ShowingSplash -> Unit
                    AppStartupUiStatus.ShowingProgress -> LoadingPage()
                    is AppStartupUiStatus.Completed -> AppNavGraph(startDestination = status.startDestination.route)
                }
            }
        }
    }
}
