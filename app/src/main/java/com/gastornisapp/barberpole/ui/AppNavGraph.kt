package com.gastornisapp.barberpole.ui

import BarberPolePage
import androidx.compose.runtime.Composable
import androidx.navigation.NavHostController
import androidx.navigation.compose.NavHost
import androidx.navigation.compose.composable
import androidx.navigation.compose.rememberNavController
import com.gastornisapp.barberpole.ui.confiramtion.ConfirmationPage
import com.gastornisapp.barberpole.ui.harmony.HarmonyPage
import com.gastornisapp.barberpole.ui.info.InfoPage
import com.gastornisapp.barberpole.ui.percussion.PercussionPage
import com.gastornisapp.barberpole.ui.vehicle.VehiclePage

@Composable
fun AppNavGraph(
    startDestination: String,
    navController: NavHostController = rememberNavController()
) {
    NavHost(
        navController = navController,
        startDestination = startDestination
    ) {
        composable(PageType.Home.route) { HomePage(navController) }
        composable(PageType.BarberPole.route) { BarberPolePage() }
        composable(PageType.Confirmation.route) { ConfirmationPage(navController) }
        composable(PageType.Vehicle.route) { VehiclePage() }
        composable(PageType.Harmony.route) { HarmonyPage() }
        composable(PageType.Percussion.route) { PercussionPage() }
        composable(PageType.WebPage.route) {
            val url = navController.previousBackStackEntry
                ?.savedStateHandle
                ?.get<String>("url")
                ?: return@composable
            WebPage(url = url)
        }
        composable(PageType.Info.route) { InfoPage(navController) }
        composable(PageType.License.route) { LicensePage() }
    }
}