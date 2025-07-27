package com.gastornisapp.barberpole.ui

import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.test.assertIsDisplayed
import androidx.compose.ui.test.junit4.createComposeRule
import androidx.compose.ui.test.onNodeWithText
import androidx.compose.ui.test.performClick
import androidx.navigation.compose.ComposeNavigator
import androidx.navigation.testing.TestNavHostController
import androidx.test.ext.junit.runners.AndroidJUnit4
import org.junit.Assert.assertTrue
import org.junit.Before
import org.junit.Rule
import org.junit.Test
import org.junit.runner.RunWith

@RunWith(AndroidJUnit4::class)
class InfoPageTest {

    @get:Rule
    val composeTestRule = createComposeRule()

    private lateinit var navController: TestNavHostController

    @Before
    fun setup() {
        composeTestRule.setContent {
            navController = TestNavHostController(LocalContext.current)
            navController.navigatorProvider.addNavigator(ComposeNavigator())
            AppNavGraph(startDestination = PageType.Info.route, navController = navController)
        }
    }

    @Test
    fun infoPage_displaysAllListItems() {
        composeTestRule.onNodeWithText("利用規約").assertIsDisplayed()
        composeTestRule.onNodeWithText("プライバシーポリシー").assertIsDisplayed()
        composeTestRule.onNodeWithText("OSS ライセンス").assertIsDisplayed()
        composeTestRule.onNodeWithText("アプリバージョン").assertIsDisplayed()
    }

    @Test
    fun clickingTerms_navigatesToTermsPage() {
        composeTestRule.onNodeWithText("利用規約").performClick()
        assertTrue(navController.currentBackStackEntry?.destination?.route?.startsWith("webpage/") == true)
    }

    @Test
    fun clickingPrivacy_navigatesToPrivacyPage() {
        composeTestRule.onNodeWithText("プライバシーポリシー").performClick()
        assertTrue(navController.currentBackStackEntry?.destination?.route?.startsWith("webpage/") == true)
    }

    @Test
    fun appVersion_isDisplayedCorrectly() {
        composeTestRule.onNodeWithText("アプリバージョン")
            .assertExists("1.0.0")
    }

    @Test
    fun clickingOssLicense_navigatesToLicensePage() {
        composeTestRule.onNodeWithText("OSS ライセンス").performClick()
        navController.assertCurrentRoute(PageType.License.route)
    }
}
