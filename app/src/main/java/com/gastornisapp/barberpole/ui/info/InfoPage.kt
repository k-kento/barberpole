package com.gastornisapp.barberpole.ui.info

import android.net.Uri
import androidx.compose.foundation.clickable
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.ListItem
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.getValue
import androidx.compose.ui.Modifier
import androidx.compose.ui.unit.dp
import androidx.hilt.navigation.compose.hiltViewModel
import androidx.navigation.NavHostController

@Composable
fun InfoPage(navController: NavHostController, viewModel: InfoViewModel = hiltViewModel()) {
    val version by viewModel.versionName
    Scaffold { paddingValues ->
        Column(
            modifier = Modifier
                .fillMaxWidth()
                .padding(paddingValues)
                .padding(16.dp)
        ) {
            ListItem({
                Text("利用規約")
            }, modifier = Modifier.clickable {
                val url = Uri.encode("file:///android_asset/terms_of_service.html")
                navController.navigate("webpage/$url")
            })
            ListItem({
                Text("プライバシーポリシー")
            }, modifier = Modifier.clickable {
                val url = Uri.encode("file:///android_asset/privacy_policy.html")
                navController.navigate("webpage/$url")
            })
            ListItem({
                Text("OSS ライセンス")
            }, modifier = Modifier.clickable {
                navController.navigate("license")
            })
            ListItem(
                headlineContent = { Text("アプリバージョン") },
                supportingContent = { Text(version) },
            )
        }
    }
}
