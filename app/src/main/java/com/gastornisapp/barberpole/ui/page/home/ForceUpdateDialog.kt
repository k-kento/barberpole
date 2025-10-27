package com.gastornisapp.barberpole.ui.page.home

import androidx.compose.material3.AlertDialog
import androidx.compose.material3.Text
import androidx.compose.material3.TextButton
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.platform.testTag
import androidx.compose.ui.res.stringResource
import com.gastornisapp.barberpole.R

@Composable
fun ForceUpdateDialog(onUpdateClick: () -> Unit) {
    AlertDialog(
        modifier = Modifier.testTag("ForceUpdateDialog"),
        onDismissRequest = {}, // 外部タップでも閉じない
        title = { Text(stringResource(R.string.force_update_title)) },
        text = {
            Text(stringResource(R.string.force_update_description))
        },
        confirmButton = {
            TextButton(onClick = onUpdateClick) {
                Text(stringResource(R.string.update))
            }
        },
        dismissButton = {} // キャンセルなし
    )
}