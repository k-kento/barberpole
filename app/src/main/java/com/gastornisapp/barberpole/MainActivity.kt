package com.gastornisapp.barberpole

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.activity.viewModels
import androidx.compose.runtime.collectAsState
import androidx.compose.runtime.getValue
import androidx.core.splashscreen.SplashScreen.Companion.installSplashScreen
import com.gastornisapp.barberpole.ui.AppNavGraph
import com.gastornisapp.barberpole.ui.LoadingPage
import com.gastornisapp.barberpole.ui.AppStartupViewModel
import com.gastornisapp.barberpole.ui.AppStartupUiStatus
import com.gastornisapp.barberpole.ui.theme.BarberPoleTheme
import dagger.hilt.android.AndroidEntryPoint

@AndroidEntryPoint
class MainActivity : ComponentActivity() {

    private val viewModel: AppStartupViewModel by viewModels()

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        enableEdgeToEdge()

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
