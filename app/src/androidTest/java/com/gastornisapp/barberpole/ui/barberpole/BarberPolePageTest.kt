package com.gastornisapp.barberpole.ui.barberpole

import BarberPolePage
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.graphics.toArgb
import androidx.compose.ui.test.assertIsDisplayed
import androidx.compose.ui.test.assertIsNotDisplayed
import androidx.compose.ui.test.assertIsOff
import androidx.compose.ui.test.assertIsOn
import androidx.compose.ui.test.hasParent
import androidx.compose.ui.test.hasTestTag
import androidx.compose.ui.test.junit4.createComposeRule
import androidx.compose.ui.test.onNodeWithTag
import androidx.compose.ui.test.performClick
import androidx.test.ext.junit.runners.AndroidJUnit4
import com.gastornisapp.barberpole.ui.page.barberpole.BarberPoleViewModel
import com.gastornisapp.barberpole.ui.page.barberpole.Orientation
import org.junit.Assert
import org.junit.Before
import org.junit.Rule
import org.junit.Test
import org.junit.runner.RunWith

@RunWith(AndroidJUnit4::class)
class BarberPolePageTest {

    @get:Rule
    val composeTestRule = createComposeRule()

    private lateinit var viewModel: BarberPoleViewModel

    @Before
    fun setup() {
        viewModel = BarberPoleViewModel()
        composeTestRule.setContent { BarberPolePage(viewModel = viewModel) }
    }

    @Test
    fun initial_display() {
        composeTestRule.onNodeWithTag("BarberPoleViewComposable").assertIsDisplayed()
        composeTestRule.onNodeWithTag("BarberPoleBottomBar").assertIsDisplayed()
        composeTestRule.onNodeWithTag("SpeedBottomSheet").assertIsNotDisplayed()
        composeTestRule.onNodeWithTag("ColorBottomSheet").assertIsNotDisplayed()
    }

    // region view model to ui

    @Test
    fun togglePlay() {
    }

    @Test
    fun toggleOrientation() {
    }

    @Test
    fun updateSliderPosition() {

    }

    @Test
    fun updateColors() {
        viewModel.updateColorSheetVisible(true)
        val colors = listOf(Color.Red, Color.Blue, Color.Green, Color.Yellow, Color.Magenta, Color.Black)

        for (color in colors) {
            if (color == Color.Red)
                assertColorIsOn(color, "ColorBottomSheet_TopRow")
            else
                assertColorIsOff(color, "ColorBottomSheet_TopRow")

            if (color == Color.Blue)
                assertColorIsOn(color, "ColorBottomSheet_BottomRow")
            else
                assertColorIsOff(color, "ColorBottomSheet_BottomRow")
        }

        viewModel.updateColors(Color.Yellow, Color.Green)

        for (color in colors) {
            if (color == Color.Yellow)
                assertColorIsOn(color, "ColorBottomSheet_TopRow")
            else
                assertColorIsOff(color, "ColorBottomSheet_TopRow")

            if (color == Color.Green)
                assertColorIsOn(color, "ColorBottomSheet_BottomRow")
            else
                assertColorIsOff(color, "ColorBottomSheet_BottomRow")
        }
    }

    @Test
    fun updateSpeedSheetVisible() {
        viewModel.updateSpeedSheetVisible(true)
        composeTestRule.onNodeWithTag("SpeedBottomSheet").assertIsDisplayed()
        viewModel.updateSpeedSheetVisible(false)
        composeTestRule.onNodeWithTag("SpeedBottomSheet").assertIsNotDisplayed()
        viewModel.updateSpeedSheetVisible(true)
        composeTestRule.onNodeWithTag("SpeedBottomSheet").assertIsDisplayed()
    }

    @Test
    fun updateColorSheetVisible() {
        viewModel.updateColorSheetVisible(true)
        composeTestRule.onNodeWithTag("ColorBottomSheet").assertIsDisplayed()
        viewModel.updateColorSheetVisible(false)
        composeTestRule.onNodeWithTag("ColorBottomSheet").assertIsNotDisplayed()
        viewModel.updateColorSheetVisible(true)
        composeTestRule.onNodeWithTag("ColorBottomSheet").assertIsDisplayed()
    }

    // end region view model to ui

    // region ui to view model

    @Test
    fun clickPlay() {
        Assert.assertEquals(true, viewModel.isPlaying)
        composeTestRule.onNodeWithTag("BarberPoleBottomBar_PlayButton").performClick()
        Assert.assertEquals(false, viewModel.isPlaying)
        composeTestRule.onNodeWithTag("BarberPoleBottomBar_PlayButton").performClick()
        Assert.assertEquals(true, viewModel.isPlaying)
    }

    @Test
    fun clickOrientation() {
        Assert.assertEquals(Orientation.Right, viewModel.orientation)
        composeTestRule.onNodeWithTag("BarberPoleBottomBar_OrientationButton").performClick()
        Assert.assertEquals(Orientation.Left, viewModel.orientation)
        composeTestRule.onNodeWithTag("BarberPoleBottomBar_OrientationButton").performClick()
        Assert.assertEquals(Orientation.Right, viewModel.orientation)
    }

    @Test
    fun speedSlider() {
    }

    @Test
    fun clickSpeedButton() {
        Assert.assertEquals(false, viewModel.speedSheetVisible)
        composeTestRule.onNodeWithTag("BarberPoleBottomBar_SpeedButton").performClick()
        Assert.assertEquals(true, viewModel.speedSheetVisible)
        // TODO 閉じる場合のテストを追加
    }

    @Test
    fun clickColor() {
        Assert.assertEquals(false, viewModel.colorSheetVisible)
        composeTestRule.onNodeWithTag("BarberPoleBottomBar_ColorButton").performClick()
        Assert.assertEquals(true, viewModel.colorSheetVisible)
        // TODO 閉じる際のテストを追加
    }

    @Test
    fun selectFirstColor() {
        viewModel.updateColorSheetVisible(true)

        Assert.assertEquals(Color.Red, viewModel.colors.first)
        composeTestRule.onNode(
            hasTestTag("ColorBottomSheet_ColorItem_${Color.Black.toArgb()}") and hasParent(hasTestTag("ColorBottomSheet_TopRow"))
        ).performClick()

        Assert.assertEquals(Color.Black, viewModel.colors.first)
    }

    @Test
    fun selectSecondColor() {
        viewModel.updateColorSheetVisible(true)

        Assert.assertEquals(Color.Blue, viewModel.colors.second)
        composeTestRule.onNode(
            hasTestTag("ColorBottomSheet_ColorItem_${Color.Black.toArgb()}") and hasParent(hasTestTag("ColorBottomSheet_BottomRow"))
        ).performClick()

        Assert.assertEquals(Color.Black, viewModel.colors.second)
    }

    // end region ui to view model

    // region other

    private fun colorItemInRow(color: Color, rowTag: String) =
        hasTestTag("ColorBottomSheet_ColorItem_${color.toArgb()}") and
                hasParent(hasTestTag(rowTag))

    private fun assertColorIsOn(color: Color, rowTag: String) {
        composeTestRule.onNode(colorItemInRow(color, rowTag)).assertIsOn()
    }

    private fun assertColorIsOff(color: Color, rowTag: String) {
        composeTestRule.onNode(colorItemInRow(color, rowTag)).assertIsOff()
    }

    // end region other
}