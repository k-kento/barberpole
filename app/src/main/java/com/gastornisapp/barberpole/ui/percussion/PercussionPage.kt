package com.gastornisapp.barberpole.ui.percussion

import androidx.compose.foundation.BorderStroke
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.aspectRatio
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.lazy.grid.GridCells
import androidx.compose.foundation.lazy.grid.LazyVerticalGrid
import androidx.compose.foundation.lazy.grid.items
import androidx.compose.foundation.shape.CircleShape
import androidx.compose.material3.CircularProgressIndicator
import androidx.compose.material3.Icon
import androidx.compose.material3.IconButtonDefaults
import androidx.compose.material3.OutlinedIconButton
import androidx.compose.material3.Scaffold
import androidx.compose.runtime.Composable
import androidx.compose.runtime.collectAsState
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.graphics.vector.ImageVector
import androidx.compose.ui.res.vectorResource
import androidx.compose.ui.unit.dp
import androidx.hilt.navigation.compose.hiltViewModel

@Composable
fun PercussionPage(viewModel: PercussionViewModel = hiltViewModel()) {
    val inProgress = viewModel.inProgress.collectAsState()

    Scaffold { paddingValues ->
        Box(
            modifier = Modifier
                .fillMaxSize()
                .padding(paddingValues)
                .padding(horizontal = 16.dp)
        ) {
            if (inProgress.value) {
                CircularProgressIndicator(modifier = Modifier.align(Alignment.Center))
            } else {
                val types = PercussionType.entries
                LazyVerticalGrid(
                    columns = GridCells.Fixed(2),
                    modifier = Modifier
                        .align(Alignment.Center)
                        .padding(8.dp),
                    content = {
                        items(types) { type ->
                            Box(modifier = Modifier.padding(4.dp)) {
                                PercussionButton(type) {
                                    viewModel.play(it)
                                }
                            }
                        }
                    }
                )
            }
        }
    }
}

@Composable
fun PercussionButton(
    percussionType: PercussionType,
    onClick: (PercussionType) -> Unit
) {
    Row(
        modifier = Modifier.fillMaxWidth(),
        horizontalArrangement = Arrangement.spacedBy(4.dp)
    ) {
        OutlinedIconButton(
            onClick = {
                onClick(percussionType)
            },
            border = BorderStroke(0.dp, Color(0X00000000)),
            shape = CircleShape,
            colors = IconButtonDefaults.outlinedIconButtonColors(Color(percussionType.color or 0xFF000000.toInt())),
            modifier = Modifier
                .weight(1f)
                .aspectRatio(1f)
        ) {
            Icon(
                modifier = Modifier.padding(12.dp),
                imageVector = ImageVector.vectorResource(id = percussionType.drawableRes),
                tint = Color.White,
                contentDescription = "", // TODO
            )
        }
    }
}
