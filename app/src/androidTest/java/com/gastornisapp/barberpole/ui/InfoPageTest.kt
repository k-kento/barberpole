package com.gastornisapp.barberpole.ui

import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.test.assertIsDisplayed
import androidx.compose.ui.test.junit4.createAndroidComposeRule
import androidx.compose.ui.test.onNodeWithText
import androidx.compose.ui.test.performClick
import androidx.navigation.compose.ComposeNavigator
import androidx.navigation.testing.TestNavHostController
import com.gastornisapp.barberpole.HiltComponentActivity
import com.gastornisapp.barberpole.di.RepositoryModule
import com.gastornisapp.barberpole.domain.model.SemVer
import com.gastornisapp.barberpole.domain.repository.AppSettingsRepository
import dagger.Module
import dagger.Provides
import dagger.hilt.android.testing.HiltAndroidRule
import dagger.hilt.android.testing.HiltAndroidTest
import dagger.hilt.android.testing.UninstallModules
import dagger.hilt.components.SingletonComponent
import dagger.hilt.testing.TestInstallIn
import io.mockk.every
import io.mockk.mockk
import org.junit.Before
import org.junit.Rule
import org.junit.Test
import javax.inject.Singleton

@UninstallModules(RepositoryModule::class)
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

        composeTestRule.setContent {
            navController = TestNavHostController(LocalContext.current).apply {
                navigatorProvider.addNavigator(ComposeNavigator())
            }
            AppNavGraph(startDestination = PageType.Info.route, navController = navController)
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

    @Test
    fun clickingTerms_navigatesToTermsPage() {
        composeTestRule.onNodeWithText("利用規約").performClick()
        assert(navController.currentBackStackEntry?.destination?.route?.startsWith("webpage/") == true)
    }

    @Test
    fun clickingPrivacy_navigatesToPrivacyPage() {
        composeTestRule.onNodeWithText("プライバシーポリシー").performClick()
        assert(navController.currentBackStackEntry?.destination?.route?.startsWith("webpage/") == true)
    }

    @Test
    fun clickingOssLicense_navigatesToLicensePage() {
        composeTestRule.onNodeWithText("OSS ライセンス").performClick()
        navController.assertCurrentRoute(PageType.License.route)
    }
}

@Module
@TestInstallIn(
    components = [SingletonComponent::class],
    replaces = [RepositoryModule::class]
)
object FakeRepositoryModule {

    @Provides
    @Singleton
    fun provideAppSettingsRepository(): AppSettingsRepository {
        val mock = mockk<AppSettingsRepository>()
        every { mock.getCurrentAppVersion() } returns Result.success(SemVer.parse("1.2.3"))
        return mock
    }
}
