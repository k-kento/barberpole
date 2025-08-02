package com.gastornisapp.barberpole.ui.home

import android.util.Patterns
import androidx.compose.material3.*
import androidx.compose.runtime.*
import com.gastornisapp.barberpole.domain.model.Notice

@Composable
fun NoticeDialog(
    notice: Notice,
    onDismissRequest: () -> Unit,
    onDetailsClick: ((String) -> Unit)? = null
) {
    AlertDialog(
        onDismissRequest = onDismissRequest,
        title = {
            Text(text = notice.title)
        },
        text = {
            Text(text = notice.message)
        },
        confirmButton = {
            val url = notice.url
            if (url != null && Patterns.WEB_URL.matcher(url).matches()) {
                TextButton(
                    onClick = {
                        onDetailsClick?.invoke(notice.url)
                    }
                ) {
                    Text("詳細を見る")
                }
            }
        },
        dismissButton = {
            TextButton(
                onClick = {
                    onDismissRequest()
                }
            ) {
                Text("閉じる")
            }
        }
    )
}