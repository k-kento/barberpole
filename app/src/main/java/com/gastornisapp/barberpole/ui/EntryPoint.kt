package com.gastornisapp.barberpole.ui

import androidx.compose.runtime.Composable
import androidx.compose.runtime.collectAsState
import androidx.hilt.navigation.compose.hiltViewModel

@Composable
fun AppNavGraph(
    viewModel: MainViewModel = hiltViewModel()
) {
    val startDestination = viewModel.startDestination.collectAsState().value
    if (startDestination == null) {
        LoadingPage()
    } else {
        AppNavGraph(startDestination = startDestination.route)
    }
}