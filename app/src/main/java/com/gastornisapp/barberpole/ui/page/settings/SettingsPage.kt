package com.gastornisapp.barberpole.ui.page.settings

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
import androidx.compose.ui.res.stringResource
import androidx.compose.ui.unit.dp
import androidx.hilt.navigation.compose.hiltViewModel
import com.gastornisapp.barberpole.R

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun SettingsPage(
    viewModel: SettingsViewModel = hiltViewModel<SettingsViewModel>(),
) {
    val context = LocalContext.current
    val isLockEnabled by viewModel.isLockEnabled.collectAsState()
    val screenPinningDisabledText = stringResource(R.string.screen_pinning_disabled_toast)

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
                    Toast.makeText(context, screenPinningDisabledText, Toast.LENGTH_LONG).show()
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
            TopAppBar(title = { Text(stringResource(R.string.settings)) })
        }
    ) { paddingValues ->
        Column(modifier = Modifier.padding(paddingValues = paddingValues)) {
            ListItem(
                headlineContent = {
                    Text(stringResource(R.string.screen_pinning_title))
                },
                supportingContent = {
                    Column {
                        Text(stringResource(R.string.screen_pinning_description))
                        Spacer(Modifier.height(8.dp))
                        TextButton(
                            onClick = { viewModel.onEvent(SettingsIntent.TapSecurityScreenButton) }) {
                            Text(stringResource(R.string.open_security_settings))
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
