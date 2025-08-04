package com.gastornisapp.barberpole.ui.home

import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.test.assertIsDisplayed
import androidx.compose.ui.test.assertIsNotDisplayed
import androidx.compose.ui.test.junit4.createAndroidComposeRule
import androidx.compose.ui.test.onNodeWithContentDescription
import androidx.compose.ui.test.onNodeWithTag
import androidx.compose.ui.test.onNodeWithText
import androidx.compose.ui.test.performClick
import androidx.navigation.compose.ComposeNavigator
import androidx.navigation.testing.TestNavHostController
import com.gastornisapp.barberpole.HiltComponentActivity
import com.gastornisapp.barberpole.domain.model.Notice
import com.gastornisapp.barberpole.ui.AppNavGraph
import com.gastornisapp.barberpole.ui.PageType
import com.gastornisapp.barberpole.ui.assertCurrentRoute
import dagger.hilt.android.testing.BindValue
import dagger.hilt.android.testing.HiltAndroidRule
import dagger.hilt.android.testing.HiltAndroidTest
import io.mockk.every
import io.mockk.mockk
import kotlinx.coroutines.flow.MutableStateFlow
import org.junit.Before
import org.junit.Rule
import org.junit.Test

@HiltAndroidTest
class HomePageTest {

    @get:Rule(order = 0)
    var hiltRule = HiltAndroidRule(this)

    @get:Rule(order = 1)
    val composeTestRule = createAndroidComposeRule<HiltComponentActivity>()

    @BindValue
    @JvmField
    val mockViewModel: HomeViewModel = mockk(relaxed = true)

    private val dialogStatus = MutableStateFlow<DialogStatus>(DialogStatus.None)

    private lateinit var navController: TestNavHostController

    @Before
    fun setup() {
        hiltRule.inject()

        every { mockViewModel.dialogStatus } returns dialogStatus

        composeTestRule.setContent {
            navController = TestNavHostController(LocalContext.current)
            navController.navigatorProvider.addNavigator(ComposeNavigator())
            AppNavGraph(startDestination = PageType.Home.route, navController = navController)
        }
    }

    // region user interaction

    @Test
    fun homePage_displaysAllItems() {
        composeTestRule.onNodeWithContentDescription("Info").assertIsDisplayed()

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

    // end region user interaction

    // region view model to ui

    @Test
    fun dialog_none_displays_no_dialogs() {
        dialogStatus.value = DialogStatus.None
        composeTestRule.onNodeWithTag("ForceUpdateDialog").assertIsNotDisplayed()
        composeTestRule.onNodeWithTag("NoticeDialog").assertIsNotDisplayed()
    }

    @Test
    fun dialog_forceUpdateRequired_displays_forceUpdateDialog_only() {
        dialogStatus.value = DialogStatus.ForceUpdateRequired
        composeTestRule.onNodeWithTag("ForceUpdateDialog").assertIsDisplayed()
        composeTestRule.onNodeWithTag("NoticeDialog").assertIsNotDisplayed()
    }

    @Test
    fun dialog_showNotice_displays_noticeDialog_only() {
        dialogStatus.value = DialogStatus.ShowNotice(Notice("id", "title", "message", startAt = 0, endAt = 0))
        composeTestRule.onNodeWithTag("ForceUpdateDialog").assertIsNotDisplayed()
        composeTestRule.onNodeWithTag("NoticeDialog").assertIsDisplayed()
    }

    // end region view model to ui
}

