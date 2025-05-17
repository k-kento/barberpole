package com.gastornisapp.barberpole

import BarberPolePage
import android.os.Bundle
import android.util.Log
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.core.splashscreen.SplashScreen.Companion.installSplashScreen
import androidx.lifecycle.lifecycleScope
import androidx.navigation.NavType
import androidx.navigation.compose.NavHost
import androidx.navigation.compose.composable
import androidx.navigation.compose.rememberNavController
import androidx.navigation.navArgument
import com.gastornisapp.barberpole.repositories.AppSettingsRepository
import com.gastornisapp.barberpole.repositories.dataStore
import com.gastornisapp.barberpole.ui.ConfirmationPage
import com.gastornisapp.barberpole.ui.HomePage
import com.gastornisapp.barberpole.ui.InfoPage
import com.gastornisapp.barberpole.ui.LicensePage
import com.gastornisapp.barberpole.ui.WebPage
import com.gastornisapp.barberpole.ui.theme.BarberPoleTheme
import com.gastornisapp.barberpole.ui.vehicle.VehiclePage
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
                                composable("home") { HomePage(navController) }
                                composable("barber_pole") { BarberPolePage() }
                                composable("confirmation") { ConfirmationPage(navController, appSettingsRepository = appSettingsRepository) }
                                composable("vehicle") { VehiclePage() }
                                composable(
                                    route = "webpage/{url}",
                                    arguments = listOf(navArgument("url") { type = NavType.StringType })
                                ) { backStackEntry ->
                                    val url = backStackEntry.arguments?.getString("url")
                                    if (url != null) {
                                        WebPage(url = url)
                                    } else {
                                        Log.e("MainActivity", "url == null")
                                    }
                                }
                                composable("info") { InfoPage(navController) }
                                composable("license") { LicensePage() }
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
