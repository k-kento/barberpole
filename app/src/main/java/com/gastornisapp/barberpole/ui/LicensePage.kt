package com.gastornisapp.barberpole.ui

import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.material3.ExperimentalMaterial3Api
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Text
import androidx.compose.material3.TopAppBar
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import com.mikepenz.aboutlibraries.ui.compose.LibrariesContainer

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun LicensePage() {
    Scaffold(
        topBar = {
            TopAppBar(title = { Text("ライセンス") })
        },
    ) { paddingValues ->
        LibrariesContainer(
            modifier = Modifier.fillMaxSize(),
            contentPadding = paddingValues
        )
    }
}