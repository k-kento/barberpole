package com.gastornisapp.barberpole.ui.home

import android.content.Intent
import androidx.annotation.DrawableRes
import androidx.compose.foundation.Image
import androidx.compose.foundation.clickable
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.aspectRatio
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.lazy.grid.GridCells
import androidx.compose.foundation.lazy.grid.LazyVerticalGrid
import androidx.compose.foundation.lazy.grid.items
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
import androidx.compose.runtime.collectAsState
import androidx.compose.runtime.getValue
import androidx.compose.ui.Modifier
import androidx.compose.ui.layout.ContentScale
import androidx.compose.ui.platform.LocalConfiguration
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.res.painterResource
import androidx.compose.ui.unit.dp
import androidx.hilt.navigation.compose.hiltViewModel
import androidx.navigation.NavHostController
import com.gastornisapp.barberpole.R
import com.gastornisapp.barberpole.ui.common.navigateToWebPage
import com.gastornisapp.barberpole.ui.PageType
import androidx.core.net.toUri

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun HomePage(
    navController: NavHostController,
    viewModel: HomeViewModel = hiltViewModel()
) {
    val context = LocalContext.current

    val tabletMinWidth = LocalContext.current.resources.getInteger(R.integer.tablet_width_dp)
    val configuration = LocalConfiguration.current
    val columns = if (configuration.screenWidthDp >= tabletMinWidth) 2 else 1

    val dialogStatus by viewModel.dialogStatus.collectAsState()
    val items = createItems()

    Scaffold(
        topBar = {
            TopAppBar(title = { Text("") }, actions = {
                IconButton(onClick = {
                    navController.navigate("info")
                }) {
                    Icon(
                        imageVector = Icons.Default.Info,
                        contentDescription = "Info"
                    )
                }
            })
        },
    ) { paddingValues ->
        LazyVerticalGrid(
            columns = GridCells.Fixed(columns),
            verticalArrangement = Arrangement.spacedBy(16.dp),
            horizontalArrangement = Arrangement.spacedBy(8.dp),
            contentPadding = paddingValues,
            modifier = Modifier
                .fillMaxSize()
                .padding(horizontal = 16.dp)
        ) {
            items(items) { item ->
                Item(itemInfo = item) {
                    navController.navigate(it.route)
                }
            }
        }

        when (val status = dialogStatus) {
            DialogStatus.ForceUpdateRequired -> {
                ForceUpdateDialog(onUpdateClick = {
                    val webIntent = Intent(
                        Intent.ACTION_VIEW,
                        "https://play.google.com/store/apps/details?id=${context.packageName}".toUri()
                    )
                    context.startActivity(webIntent)
                })
            }

            is DialogStatus.ShowNotice -> {
                NoticeDialog(
                    notice = status.notice,
                    onDetailsClick = navController::navigateToWebPage,
                    onDismissRequest = viewModel::onNoticeDismissed // 複数回呼ばれることがあるため注意する
                )
            }

            DialogStatus.None -> Unit
        }
    }
}

@Composable
private fun Item(
    itemInfo: ItemInfo,
    onClicked: (PageType) -> Unit = {},
) {
    Card(
        modifier = Modifier
            .fillMaxWidth()
            .clickable {
                onClicked(itemInfo.route)
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
                    .aspectRatio(1f),
                contentScale = itemInfo.contentScale
            )
            Spacer(modifier = Modifier.height(8.dp))
        }
    }
}

private fun createItems(): List<ItemInfo> {
    return listOf(
        ItemInfo(
            drawableRes = R.drawable.vehicles,
            PageType.Vehicle
        ),
        ItemInfo(
            drawableRes = R.drawable.percussion,
            PageType.Percussion
        ),
        ItemInfo(
            drawableRes = R.drawable.fishes,
            PageType.Swarm
        ),
        ItemInfo(
            drawableRes = R.drawable.kaleidoscope,
            PageType.Kaleidoscope,
            contentScale = ContentScale.Fit
        ),
        ItemInfo(
            drawableRes = R.drawable.barberpole,
            PageType.BarberPole,
            contentScale = ContentScale.Fit
        ),
    )
}

private data class ItemInfo(
    @param:DrawableRes val drawableRes: Int,
    val route: PageType,
    val contentScale: ContentScale = ContentScale.Crop
)