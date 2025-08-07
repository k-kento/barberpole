package com.gastornisapp.barberpole.ui.home

import androidx.compose.material3.AlertDialog
import androidx.compose.material3.Text
import androidx.compose.material3.TextButton
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.platform.testTag

@Composable
fun ForceUpdateDialog(onUpdateClick: () -> Unit) {
    AlertDialog(
        modifier = Modifier.testTag("ForceUpdateDialog"),
        onDismissRequest = {}, // 外部タップでも閉じない
        title = { Text("アプリの更新が必要です") },
        text = {
            Text(
                """
                    ご利用中のアプリバージョンはサポートが終了しています。
                    引き続きサービスをご利用いただくには、最新バージョンへのアップデートが必要です。
                    """.trimIndent()
            )
        },
        confirmButton = {
            TextButton(onClick = onUpdateClick) {
                Text("アップデート")
            }
        },
        dismissButton = {} // キャンセルなし
    )
}