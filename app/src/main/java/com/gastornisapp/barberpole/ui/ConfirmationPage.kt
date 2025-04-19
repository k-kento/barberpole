package com.gastornisapp.barberpole.ui

import android.app.Activity
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.Button
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.rememberCoroutineScope
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.unit.dp
import androidx.navigation.NavController
import com.gastornisapp.barberpole.repositories.AppSettingsRepository
import kotlinx.coroutines.launch

@Composable
fun ConfirmationPage(
    navController: NavController,
    appSettingsRepository: AppSettingsRepository,
) {
    Scaffold { paddingValues ->
        val context = LocalContext.current
        val coroutineScope = rememberCoroutineScope()

        Box(
            Modifier
                .fillMaxSize()
                .padding(paddingValues)
                .padding(16.dp)
        ) {
            Column(
                modifier = Modifier
                    .fillMaxSize()
            ) {
                Text(
                    text = "文章がここに入ります。",
                    style = MaterialTheme.typography.bodyMedium
                )
            }
            Row(
                modifier = Modifier
                    .align(Alignment.BottomCenter)
                    .fillMaxWidth(),
                horizontalArrangement = Arrangement.SpaceBetween
            ) {
                Button(
                    onClick = {
                        val activity = context as? Activity
                        activity?.finish()
                    },
                ) {
                    Text("アプリを終了")
                }

                Button(
                    onClick = {
                        coroutineScope.launch {
                            appSettingsRepository.saveVersion()
                            navController.navigate("home")
                        }
                    },
                ) {
                    Text("同意")
                }
            }
        }
    }
}
