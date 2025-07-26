package com.gastornisapp.barberpole

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.core.splashscreen.SplashScreen.Companion.installSplashScreen
import androidx.hilt.navigation.compose.hiltViewModel
import com.gastornisapp.barberpole.ui.AppNavGraph
import com.gastornisapp.barberpole.ui.MainViewModel
import com.gastornisapp.barberpole.ui.theme.BarberPoleTheme
import dagger.hilt.android.AndroidEntryPoint

@AndroidEntryPoint
class MainActivity : ComponentActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        val splashScreen = installSplashScreen()

        setContent {
            val viewModel: MainViewModel = hiltViewModel()
            splashScreen.setKeepOnScreenCondition { viewModel.isSplashScreenShowing.value }

            BarberPoleTheme {
                AppNavGraph(viewModel)
            }
        }

        enableEdgeToEdge()
    }
}
