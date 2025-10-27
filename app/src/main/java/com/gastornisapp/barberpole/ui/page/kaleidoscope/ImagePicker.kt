package com.gastornisapp.barberpole.ui.page.kaleidoscope

import androidx.compose.foundation.BorderStroke
import androidx.compose.foundation.Image
import androidx.compose.foundation.clickable
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.aspectRatio
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.lazy.grid.GridCells
import androidx.compose.foundation.lazy.grid.LazyVerticalGrid
import androidx.compose.foundation.lazy.grid.items
import androidx.compose.material3.Card
import androidx.compose.material3.MaterialTheme
import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.layout.ContentScale
import androidx.compose.ui.unit.dp
import coil3.compose.rememberAsyncImagePainter

@Composable
fun DrawablePicker(
    imageItems: List<ImageItem>,
    selected: ImageItem?,
    onSelect: (ImageItem) -> Unit,
) {
    LazyVerticalGrid(
        columns = GridCells.Fixed(3),
        modifier = Modifier.fillMaxWidth(),
    ) {
        items(imageItems) { imageItem ->
            val isSelected = imageItem == selected

            Card(
                modifier = Modifier
                    .padding(8.dp)
                    .aspectRatio(1f)
                    .clickable { onSelect(imageItem) },
                border = if (isSelected) BorderStroke(4.dp, MaterialTheme.colorScheme.primary) else null,
                shape = MaterialTheme.shapes.medium,
            ) {
                Box(
                    modifier = Modifier.fillMaxSize(),
                    contentAlignment = Alignment.Center
                ) {
                    Image(
                        painter = rememberAsyncImagePainter(imageItem.assetPath),
                        contentDescription = null,
                        contentScale = ContentScale.Fit
                    )
                }
            }
        }
    }
}
