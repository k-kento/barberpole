package com.gastornisapp.barberpole.ui

import androidx.navigation.testing.TestNavHostController
import org.junit.Assert.assertEquals

fun TestNavHostController.assertCurrentRoute(expectedRoute: String) {
    assertEquals(expectedRoute, currentBackStackEntry?.destination?.route)
}