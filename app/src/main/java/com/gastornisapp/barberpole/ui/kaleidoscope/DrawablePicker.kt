package com.gastornisapp.barberpole.ui.kaleidoscope

import android.graphics.BitmapFactory
import android.util.Log
import androidx.compose.foundation.BorderStroke
import androidx.compose.foundation.Image
import androidx.compose.foundation.clickable
import androidx.compose.foundation.layout.aspectRatio
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.lazy.grid.GridCells
import androidx.compose.foundation.lazy.grid.LazyVerticalGrid
import androidx.compose.foundation.lazy.grid.items
import androidx.compose.material3.Card
import androidx.compose.material3.MaterialTheme
import androidx.compose.runtime.Composable
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.ImageBitmap
import androidx.compose.ui.graphics.asImageBitmap
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.unit.dp
import java.io.IOException

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
                AssetImage(imageItem.assetPath)
            }
        }
    }
}

@Composable
fun AssetImage(assetName: String) {
    val context = LocalContext.current
    var imageBitmap by remember { mutableStateOf<ImageBitmap?>(null) }

    LaunchedEffect(assetName) {
        try {
            context.assets.open("images/$assetName").use { inputStream ->
                val bitmap = BitmapFactory.decodeStream(inputStream)
                imageBitmap = bitmap.asImageBitmap()
            }
        } catch (e: IOException) {
            Log.e("DrawablePicker", "image error", e)
        }
    }

    imageBitmap?.let {
        Image(bitmap = it, contentDescription = null)
    }
}
