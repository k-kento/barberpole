package com.gastornisapp.barberpole.ui

import BarberPolePage
import androidx.compose.runtime.Composable
import androidx.navigation.NavHostController
import androidx.navigation.NavType
import androidx.navigation.compose.NavHost
import androidx.navigation.compose.composable
import androidx.navigation.compose.rememberNavController
import androidx.navigation.navArgument
import com.gastornisapp.barberpole.ui.page.LicensePage
import com.gastornisapp.barberpole.ui.page.WebPage
import com.gastornisapp.barberpole.ui.page.confiramtion.ConfirmationPage
import com.gastornisapp.barberpole.ui.page.drawing.DrawingPage
import com.gastornisapp.barberpole.ui.page.home.HomePage
import com.gastornisapp.barberpole.ui.page.info.InfoPage
import com.gastornisapp.barberpole.ui.page.kaleidoscope.KaleidoscopePage
import com.gastornisapp.barberpole.ui.page.percussion.PercussionPage
import com.gastornisapp.barberpole.ui.page.settings.SettingsPage
import com.gastornisapp.barberpole.ui.page.swarm.SwarmPage
import com.gastornisapp.barberpole.ui.page.vehicle.VehiclePage

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
        composable(PageType.Percussion.route) { PercussionPage() }
        composable(
            route = PageType.WebPage.route,
            arguments = listOf(
                navArgument("url") {
                    type = NavType.StringType
                    nullable = false
                }
            )
        ) { backStackEntry ->
            val url = backStackEntry.arguments?.getString("url") ?: error("url is null")
            WebPage(url = url)
        }
        composable(PageType.Swarm.route ) { SwarmPage() }
        composable(PageType.Kaleidoscope.route ) { KaleidoscopePage() }
        composable(PageType.Drawing.route ) { DrawingPage() }
        composable(PageType.Info.route) { InfoPage(navController) }
        composable(PageType.Settings.route) { SettingsPage() }
        composable(PageType.License.route) { LicensePage() }
    }
}
