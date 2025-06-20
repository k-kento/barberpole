package com.gastornisapp.barberpole.ui

import androidx.annotation.DrawableRes
import androidx.compose.foundation.Image
import androidx.compose.foundation.clickable
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.PaddingValues
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.lazy.grid.GridCells
import androidx.compose.foundation.lazy.grid.LazyVerticalGrid
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.Info
import androidx.compose.material3.Card
import androidx.compose.material3.ExperimentalMaterial3Api
import androidx.compose.material3.Icon
import androidx.compose.material3.IconButton
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Text
import androidx.compose.material3.TopAppBar
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.layout.ContentScale
import androidx.compose.ui.res.painterResource
import androidx.compose.ui.unit.dp
import androidx.navigation.NavHostController
import com.gastornisapp.barberpole.R

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun HomePage(navHostController: NavHostController) {
    Scaffold(
        topBar = {
            TopAppBar(title = { Text("") }, actions = {
                IconButton(onClick = {
                    navHostController.navigate("info")
                }) {
                    Icon(
                        imageVector = Icons.Default.Info, // infoアイコン
                        contentDescription = "Info"
                    )
                }
            })
        },
    ) { paddingValues ->
        val itemList = listOf(
            ItemInfo(
                title = "さいんぽーる",
                drawableRes = R.drawable.ic_launcher_background,
                onClick = { navHostController.navigate("barber_pole") }
            ),
            ItemInfo(
                title = "くるま",
                drawableRes = R.drawable.ic_launcher_background,
                onClick = { navHostController.navigate("vehicle") }
            ),
            ItemInfo(
                title = "ハーモニー",
                drawableRes = R.drawable.ic_launcher_background,
                onClick = { navHostController.navigate("harmony") }
            )
        )

        LazyVerticalGrid(
            columns = GridCells.Fixed(2), // 列数
            verticalArrangement = Arrangement.spacedBy(8.dp),
            horizontalArrangement = Arrangement.spacedBy(8.dp),
            contentPadding = paddingValues,
            modifier = Modifier
                .fillMaxSize()
                .padding(PaddingValues(horizontal = 16.dp))
        ) {
            items(itemList.size) { index ->
                Item(itemList[index])
            }
        }
    }
}

@Composable
private fun Item(itemInfo: ItemInfo) {
    Card(
        modifier = Modifier
            .fillMaxWidth()
            .clickable {
                itemInfo.onClick()
            },
    ) {
        Column(
            modifier = Modifier.fillMaxWidth()
        ) {
            Image(
                painter = painterResource(id = itemInfo.drawableRes),
                contentDescription = "Sample Image",
                modifier = Modifier
                    .fillMaxWidth()
                    .height(200.dp),
                contentScale = ContentScale.Crop
            )
            Spacer(modifier = Modifier.height(8.dp))
            Text(
                text = itemInfo.title,
                modifier = Modifier.padding(16.dp),
            )
        }
    }
}

data class ItemInfo(
    val title: String,
    @DrawableRes val drawableRes: Int,
    val onClick: () -> Unit
)