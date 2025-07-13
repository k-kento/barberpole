package com.gastornisapp.barberpole.ui.percussion

import androidx.annotation.DrawableRes
import com.gastornisapp.barberpole.R
import com.gastornisapp.soundlib.AudioResource

enum class PercussionType(@DrawableRes val drawableRes: Int, val color: Int) {
    Bongo(drawableRes = R.drawable.conga, 0xF44336),
    Pan(drawableRes = R.drawable.pan, 0xFF9800),
    Shaker(drawableRes = R.drawable.conga, 0xFFEB3B),
    Taiko(drawableRes = R.drawable.conga, 0x4CAF50),
    Tambourines(drawableRes = R.drawable.conga, 0x2196F3),
    Stick(drawableRes = R.drawable.stick, 0x3F51B5),
    Bell(drawableRes = R.drawable.bell, 0x9C27B0),
    Conga(drawableRes = R.drawable.conga, 0x000000),
}

fun PercussionType.toAudioResource(): AudioResource = when (this) {
    PercussionType.Bongo -> AudioResource.Bongo
    PercussionType.Pan -> AudioResource.Pan
    PercussionType.Shaker -> AudioResource.Shaker
    PercussionType.Taiko -> AudioResource.Taiko
    PercussionType.Tambourines -> AudioResource.Tambourines
    PercussionType.Stick -> AudioResource.Stick
    PercussionType.Bell -> AudioResource.Bell
    PercussionType.Conga -> AudioResource.Conga
}