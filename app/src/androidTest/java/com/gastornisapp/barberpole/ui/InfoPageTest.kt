package com.gastornisapp.barberpole.ui

import androidx.compose.runtime.Composable
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.test.assertIsDisplayed
import androidx.compose.ui.test.junit4.createAndroidComposeRule
import androidx.compose.ui.test.onNodeWithText
import androidx.compose.ui.test.performClick
import androidx.navigation.compose.ComposeNavigator
import androidx.navigation.testing.TestNavHostController
import com.gastornisapp.barberpole.HiltComponentActivity
import com.gastornisapp.barberpole.ui.info.InfoPage
import com.gastornisapp.barberpole.ui.info.InfoViewModel
import dagger.hilt.android.testing.HiltAndroidRule
import dagger.hilt.android.testing.HiltAndroidTest
import org.junit.Assert.assertEquals
import org.junit.Before
import org.junit.Rule
import org.junit.Test

@HiltAndroidTest
class InfoPageTest {
    @get:Rule(order = 0)
    var hiltRule = HiltAndroidRule(this)

    @get:Rule(order = 1)
    val composeTestRule = createAndroidComposeRule<HiltComponentActivity>()

    @Before
    fun setup() {
        hiltRule.inject()
        val viewModel = InfoViewModel(FakeAppSettingsRepository())
        composeTestRule.setContent {
            val nav = initNavController()
            InfoPage(nav, viewModel)
        }
    }

    @Test
    fun infoPage_displaysAllListItems() {
        composeTestRule.onNodeWithText("利用規約").assertIsDisplayed()
        composeTestRule.onNodeWithText("プライバシーポリシー").assertIsDisplayed()
        composeTestRule.onNodeWithText("OSS ライセンス").assertIsDisplayed()
        composeTestRule.onNodeWithText("アプリバージョン").assertIsDisplayed()
        composeTestRule.onNodeWithText("アプリバージョン").assertExists("1.2.3")
    }
}

@HiltAndroidTest
class InfoPageNavigationTest {

    @get:Rule(order = 0)
    var hiltRule = HiltAndroidRule(this)

    @get:Rule(order = 1)
    val composeTestRule = createAndroidComposeRule<HiltComponentActivity>()

    private lateinit var navController: TestNavHostController

    @Before
    fun setup() {
        hiltRule.inject()
        composeTestRule.setContent {
            navController = initNavController()
            AppNavGraph(
                startDestination = PageType.Info.route,
                navController = navController
            )
        }
    }

    @Test
    fun clickingTerms_navigatesToTermsPage() {
        composeTestRule.onNodeWithText("利用規約").performClick()
        navController.assertCurrentRoute(PageType.WebPage.route)
        val url = navController.previousBackStackEntry
            ?.savedStateHandle
            ?.get<String>("url")
        assertEquals("file:///android_asset/terms_of_service.html", url)
    }

    @Test
    fun clickingPrivacy_navigatesToPrivacyPage() {
        composeTestRule.onNodeWithText("プライバシーポリシー").performClick()
        navController.assertCurrentRoute(PageType.WebPage.route)
        val url = navController.previousBackStackEntry
            ?.savedStateHandle
            ?.get<String>("url")
        assertEquals("file:///android_asset/privacy_policy.html", url)
    }

    @Test
    fun clickingOssLicense_navigatesToLicensePage() {
        composeTestRule.onNodeWithText("OSS ライセンス").performClick()
        navController.assertCurrentRoute(PageType.License.route)
    }
}

@Composable
private fun initNavController(): TestNavHostController {
    val controller = TestNavHostController(LocalContext.current).apply {
        navigatorProvider.addNavigator(ComposeNavigator())
    }
    return controller
}

private class FakeAppSettingsRepository : BaseFakeAppSettingsRepository() {
    override fun getVersionName(): String {
        return "1.2.3"
    }
}

