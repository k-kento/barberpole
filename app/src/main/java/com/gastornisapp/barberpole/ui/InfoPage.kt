package com.gastornisapp.barberpole.ui

import android.content.Context
import android.content.pm.PackageManager
import android.net.Uri
import androidx.compose.foundation.clickable
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.ListItem
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.unit.dp
import androidx.navigation.NavHostController

@Composable
fun InfoPage(navHostController: NavHostController) {
    Scaffold { paddingValues ->
        Column(
            modifier = Modifier
                .fillMaxWidth()
                .padding(paddingValues)
                .padding(16.dp)
        ) {
            val context = LocalContext.current

            ListItem({
                Text("利用規約")
            }, modifier = Modifier.clickable {
                val url = Uri.encode("file:///android_asset/terms_of_service.html")
                navHostController.navigate("webpage/$url")
            })
            ListItem({
                Text("プライバシーポリシー")
            }, modifier = Modifier.clickable {
                val url = Uri.encode("file:///android_asset/privacy_policy.html")
                navHostController.navigate("webpage/$url")
            })
            ListItem({
                Text("OSS ライセンス")
            }, modifier = Modifier.clickable {
                navHostController.navigate("license")
            })
            ListItem(
                headlineContent = { Text("アプリバージョン") },
                supportingContent = { Text(getAppVersionName(context) ?: "") },
            )
        }
    }
}

/**
 * アプリバージョン名を取得する
 */
private fun getAppVersionName(context: Context): String? {
    return try {
        val packageInfo = context.packageManager.getPackageInfo(context.packageName, 0)
        packageInfo.versionName
    } catch (e: PackageManager.NameNotFoundException) {
        e.printStackTrace()
        null
    }
}