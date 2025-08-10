package com.gastornisapp.barberpole.ui.percussion

import androidx.compose.foundation.BorderStroke
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.aspectRatio
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
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
                GridButtons(onClick = { type ->
                    viewModel.play(type)
                })
            }
        }
    }
}

@Composable
fun GridButtons(onClick: (PercussionType) -> Unit) {
    val types = PercussionType.entries
    val half = (types.size + 1) / 2

    Column(verticalArrangement = Arrangement.spacedBy(4.dp)) {
        ButtonsRow(
            types = types.take(half),
            onClick = onClick
        )
        ButtonsRow(
            types = types.drop(half),
            onClick = onClick
        )
    }
}

@Composable
fun ButtonsRow(
    types: List<PercussionType>,
    onClick: (PercussionType) -> Unit
) {
    Row(
        modifier = Modifier.fillMaxWidth(),
        horizontalArrangement = Arrangement.spacedBy(4.dp)
    ) {
        for (type in types) {
            // 円形 かつ RippleEffect を適用するため OutlinedIconButton を使用する
            OutlinedIconButton(
                onClick = {
                    onClick(type)
                },
                border = BorderStroke(0.dp, Color(0X00000000)),
                shape = CircleShape,
                colors = IconButtonDefaults.outlinedIconButtonColors(Color(type.color or 0xFF000000.toInt())),
                modifier = Modifier
                    .weight(1f)
                    .aspectRatio(1f)
            ) {
                Icon(
                    modifier = Modifier.padding(12.dp),
                    imageVector = ImageVector.vectorResource(id = type.drawableRes),
                    tint = Color.White,
                    contentDescription = "",
                )
            }
        }
    }
}
