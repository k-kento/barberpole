package com.gastornisapp.barberpole.ui

import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.test.assertIsDisplayed
import androidx.compose.ui.test.junit4.createComposeRule
import androidx.compose.ui.test.onNodeWithContentDescription
import androidx.compose.ui.test.onNodeWithText
import androidx.compose.ui.test.performClick
import androidx.navigation.compose.ComposeNavigator
import androidx.navigation.testing.TestNavHostController
import androidx.test.ext.junit.runners.AndroidJUnit4
import org.junit.Before
import org.junit.Rule
import org.junit.runner.RunWith
import org.junit.Test

@RunWith(AndroidJUnit4::class)
class HomePageTest {

    @get:Rule
    val composeTestRule = createComposeRule()

    private lateinit var navController: TestNavHostController

    @Before
    fun setup() {
        composeTestRule.setContent {
            navController = TestNavHostController(LocalContext.current)
            navController.navigatorProvider.addNavigator(ComposeNavigator())
            AppNavGraph(startDestination = PageType.Home.route, navController = navController)
        }
    }

    @Test
    fun homePage_displaysAllItems() {
        composeTestRule.onNodeWithContentDescription("Info").assertIsDisplayed()

        // リスト
        composeTestRule.onNodeWithText("さいんぽーる").assertIsDisplayed()
        composeTestRule.onNodeWithText("くるま").assertIsDisplayed()
        composeTestRule.onNodeWithText("たいこ").assertIsDisplayed()
    }

    @Test
    fun clickingSignPole_navigatesToBarberPole() {
        composeTestRule.onNodeWithText("さいんぽーる").performClick()
        navController.assertCurrentRoute(PageType.BarberPole.route)
    }

    @Test
    fun clickingVehicle_navigatesToVehicle() {
        composeTestRule.onNodeWithText("くるま").performClick()
        navController.assertCurrentRoute(PageType.Vehicle.route)
    }

    @Test
    fun clickingPercussion_navigatesToPercussion() {
        composeTestRule.onNodeWithText("たいこ").performClick()
        navController.assertCurrentRoute(PageType.Percussion.route)
    }

    @Test
    fun clickingInfoIcon_navigatesToInfo() {
        composeTestRule.onNodeWithContentDescription("Info").performClick()
        navController.assertCurrentRoute(PageType.Info.route)
    }
}