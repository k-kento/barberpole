package com.gastornisapp.barberpole

import BarberPolePage
import android.os.Bundle
import android.util.Log
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.core.splashscreen.SplashScreen.Companion.installSplashScreen
import androidx.lifecycle.lifecycleScope
import androidx.navigation.compose.NavHost
import androidx.navigation.compose.composable
import androidx.navigation.compose.rememberNavController
import com.gastornisapp.barberpole.repositories.AppSettingsRepository
import com.gastornisapp.barberpole.repositories.dataStore
import com.gastornisapp.barberpole.ui.ConfirmationPage
import com.gastornisapp.barberpole.ui.WebPage
import com.gastornisapp.barberpole.ui.theme.BarberPoleTheme
import kotlinx.coroutines.flow.take
import kotlinx.coroutines.launch

class MainActivity : ComponentActivity() {

    private var isSplashShowing: Boolean = true
    private lateinit var appSettingsRepository: AppSettingsRepository

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        appSettingsRepository = AppSettingsRepository(applicationContext.dataStore)

        lifecycleScope.launch {
            appSettingsRepository.version
                .take(1)
                .collect { version ->
                    isSplashShowing = false
                    setContent {
                        val isTermsAgreed = 1 <= version
                        val navController = rememberNavController()

                        // ナビゲーションホストを設定
                        BarberPoleTheme {
                            NavHost(
                                navController = navController,
                                startDestination = if (isTermsAgreed) "home" else "confirmation"// 最初の画面を指定
                            ) {
                                composable("home") { BarberPolePage(navController) }
                                composable("confirmation") { ConfirmationPage(navController, appSettingsRepository = appSettingsRepository) }
                                composable("webpage") {
                                    val url = navController.previousBackStackEntry?.savedStateHandle?.get<String>("url")
                                    if (url != null) {
                                        WebPage(url = url)
                                    } else {
                                        Log.e("MainActivity", "url == null")
                                    }
                                }
                            }
                        }
                    }
                }
        }

        val splashScreen = installSplashScreen()

        // スプラッシュの表示を制御
        splashScreen.setKeepOnScreenCondition {
            isSplashShowing
        }

        enableEdgeToEdge()
    }
}
