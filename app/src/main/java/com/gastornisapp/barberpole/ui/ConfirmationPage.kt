package com.gastornisapp.barberpole.ui

import android.app.Activity
import android.net.Uri
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.Button
import androidx.compose.material3.ExperimentalMaterial3Api
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Text
import androidx.compose.material3.TextButton
import androidx.compose.material3.TopAppBar
import androidx.compose.runtime.Composable
import androidx.compose.runtime.rememberCoroutineScope
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.unit.dp
import androidx.navigation.NavController
import com.gastornisapp.barberpole.repositories.AppSettingsRepository
import kotlinx.coroutines.launch

/**
 * 利用規約確認画面
 */
@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun ConfirmationPage(
    navController: NavController,
    appSettingsRepository: AppSettingsRepository,
) {
    Scaffold(
        topBar = { TopAppBar(title = { Text("利用規約") }) },
    ) { paddingValues ->
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
                    text = "本アプリをご利用いただくには、利用規約への同意が必要です。\n" +
                            "データの取り扱いやお客様のプライバシーについては、プライバシーポリシーをご確認ください。\n" +
                            "内容をご確認のうえ、「同意する」ボタンをタップしてください。",
                )
                Spacer(modifier = Modifier.height(16.dp))
                TextButton(onClick = {
                    val url = Uri.encode("file:///android_asset/terms_of_service.html")
                    navController.navigate("webpage/$url")
                }) {
                    Text("利用規約")
                }
                TextButton(onClick = {
                    val url = Uri.encode("file:///android_asset/privacy_policy.html")
                    navController.navigate("webpage/$url")
                }) {
                    Text("プライバシーポリシー")
                }
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
                            // 利用規約画面をポップし、ホーム画面へ遷移する。
                            navController.navigate("home") {
                                popUpTo("confirmation") { inclusive = true }
                            }
                        }
                    },
                ) {
                    Text("同意する")
                }
            }
        }
    }
}
