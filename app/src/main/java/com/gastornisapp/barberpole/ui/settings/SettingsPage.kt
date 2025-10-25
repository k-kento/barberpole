package com.gastornisapp.barberpole.ui.settings

import android.app.Activity
import android.content.Intent
import android.provider.Settings
import android.util.Log
import android.widget.Toast
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.ExperimentalMaterial3Api
import androidx.compose.material3.ListItem
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Switch
import androidx.compose.material3.Text
import androidx.compose.material3.TextButton
import androidx.compose.material3.TopAppBar
import androidx.compose.runtime.Composable
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.runtime.collectAsState
import androidx.compose.runtime.getValue
import androidx.compose.ui.Modifier
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.unit.dp
import androidx.hilt.navigation.compose.hiltViewModel

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun SettingsPage(
    viewModel: SettingsViewModel = hiltViewModel<SettingsViewModel>(),
) {
    val context = LocalContext.current
    val isLockEnabled by viewModel.isLockEnabled.collectAsState()

    LaunchedEffect(Unit) {
        viewModel.events.collect { event ->
            when (event) {
                SettingsEvent.StartLock -> {
                    try {
                        (context as? Activity)?.startLockTask()
                    } catch (e: Exception) {
                        Log.e("SettingsPage", "Unexpected error on startLockTask", e)
                    }
                }

                SettingsEvent.StopLock -> {
                    Toast.makeText(context, "画面固定は次回のアプリ起動時から無効になります。", Toast.LENGTH_LONG).show()
                }

                SettingsEvent.OpenSecuritySettings -> {
                    val intent = Intent(Settings.ACTION_SECURITY_SETTINGS)
                    context.startActivity(intent)
                }
            }
        }
    }

    Scaffold(
        topBar = {
            TopAppBar(title = { Text("設定") })
        }
    ) { paddingValues ->
        Column(modifier = Modifier.padding(paddingValues = paddingValues)) {
            ListItem(
                headlineContent = {
                    Text("アプリの画面固定")
                },
                supportingContent = {
                    Column {
                        Text(
                            "このアプリの画面を固定し、お子さまが他のアプリを操作できないようにします。\n" +
                                    "この機能を利用するには、端末のセキュリティ設定で「アプリ固定」を有効にしておく必要があります。"
                        )
                        Spacer(Modifier.height(8.dp))
                        TextButton(
                            onClick = { viewModel.onEvent(SettingsIntent.TapSecurityScreenButton) }) {
                            Text("端末のセキュリティ設定を開く")
                        }
                    }
                },
                trailingContent = {
                    Switch(
                        checked = isLockEnabled,
                        onCheckedChange = { viewModel.onEvent(SettingsIntent.ToggleLockMode) },
                    )
                })
        }
    }
}
