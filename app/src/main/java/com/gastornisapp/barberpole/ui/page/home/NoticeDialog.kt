package com.gastornisapp.barberpole.ui.page.home

import android.util.Patterns
import androidx.compose.material3.*
import androidx.compose.runtime.*
import androidx.compose.ui.Modifier
import androidx.compose.ui.platform.testTag
import androidx.compose.ui.res.stringResource
import com.gastornisapp.barberpole.R
import com.gastornisapp.barberpole.domain.entity.Notice

@Composable
fun NoticeDialog(
    notice: Notice,
    onDismissRequest: (String) -> Unit,
    onDetailsClick: ((String) -> Unit)
) {
    AlertDialog(
        modifier = Modifier.testTag("NoticeDialog"),
        onDismissRequest = {
            // ダイアログ外部タップ時に onDismissRequest が連続で呼び出されるため使用しない
        },
        title = { Text(text = notice.title) },
        text = { Text(text = notice.message) },
        confirmButton = {
            val url = notice.url
            if (url != null && Patterns.WEB_URL.matcher(url).matches()) {
                TextButton(
                    onClick = { onDetailsClick(notice.url) }
                ) {
                    Text(stringResource(R.string.see_details))
                }
            }
        },
        dismissButton = {
            TextButton(
                onClick = { onDismissRequest(notice.id) }
            ) {
                Text(stringResource(R.string.close))
            }
        }
    )
}
