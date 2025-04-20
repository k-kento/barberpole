package com.gastornisapp.barberpole.ui

import androidx.compose.foundation.clickable
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.ListItem
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
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
            ListItem({
                Text("利用規約")
            }, modifier = Modifier.clickable {

            })
            ListItem({
                Text("プライバシーポリシー")
            }, modifier = Modifier.clickable {

            })
            ListItem({
                Text("OSS ライセンス")
            }, modifier = Modifier.clickable {
                navHostController.navigate("license")
            })
            ListItem({
                Text("アプリバージョン")
            }, modifier = Modifier.clickable {

            })
        }
    }
}
