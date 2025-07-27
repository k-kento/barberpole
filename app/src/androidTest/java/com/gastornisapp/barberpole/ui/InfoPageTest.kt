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
import org.junit.Before
import org.junit.Rule
import org.junit.Test

@HiltAndroidTest
class InfoPageTest {

    @get:Rule(order = 0)
    var hiltRule = HiltAndroidRule(this)

    @get:Rule(order = 1)
    val composeTestRule = createAndroidComposeRule<HiltComponentActivity>()

    private lateinit var navController: TestNavHostController

    @Before
    fun setup() {
        hiltRule.inject()
    }

    @Composable
    private fun initNavController(): TestNavHostController {
        val controller = TestNavHostController(LocalContext.current).apply {
            navigatorProvider.addNavigator(ComposeNavigator())
        }
        navController = controller
        return controller
    }

    private fun launchWithInfoPage(viewModel: InfoViewModel? = null) {
        composeTestRule.setContent {
            val nav = initNavController()
            InfoPage(nav, viewModel ?: defaultViewModel())
        }
    }

    private fun launchWithAppNavGraph() {
        composeTestRule.setContent {
            val nav = initNavController()
            AppNavGraph(
                startDestination = PageType.Info.route,
                navController = nav
            )
        }
    }

    private fun defaultViewModel() = InfoViewModel(FakeAppSettingsRepository())

    @Test
    fun infoPage_displaysAllListItems() {
        launchWithInfoPage()
        composeTestRule.onNodeWithText("利用規約").assertIsDisplayed()
        composeTestRule.onNodeWithText("プライバシーポリシー").assertIsDisplayed()
        composeTestRule.onNodeWithText("OSS ライセンス").assertIsDisplayed()
        composeTestRule.onNodeWithText("アプリバージョン").assertIsDisplayed()
        composeTestRule.onNodeWithText("アプリバージョン").assertExists("1.2.3")
    }

    @Test
    fun clickingTerms_navigatesToTermsPage() {
        launchWithAppNavGraph()
        composeTestRule.onNodeWithText("利用規約").performClick()
        assert(navController.currentBackStackEntry?.destination?.route?.startsWith("webpage/") == true)
    }

    @Test
    fun clickingPrivacy_navigatesToPrivacyPage() {
        launchWithAppNavGraph()
        composeTestRule.onNodeWithText("プライバシーポリシー").performClick()
        assert(navController.currentBackStackEntry?.destination?.route?.startsWith("webpage/") == true)
    }

    @Test
    fun clickingOssLicense_navigatesToLicensePage() {
        launchWithAppNavGraph()
        composeTestRule.onNodeWithText("OSS ライセンス").performClick()
        navController.assertCurrentRoute(PageType.License.route)
    }
}

private class FakeAppSettingsRepository : BaseFakeAppSettingsRepository() {
    override fun getVersionName(): String {
        return "1.2.3"
    }
}

