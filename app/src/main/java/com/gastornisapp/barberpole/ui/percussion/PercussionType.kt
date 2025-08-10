package com.gastornisapp.barberpole.ui.percussion

import androidx.annotation.DrawableRes
import com.gastornisapp.barberpole.R

enum class PercussionType(@param:DrawableRes val drawableRes: Int, val color: Int, private val fileName: String) {
    Bongo(drawableRes = R.drawable.bongo, 0xF44336, fileName = "bongo.wav"),
    Pan(drawableRes = R.drawable.pan, 0xFF9800, fileName = "pan.wav"),
    Shaker(drawableRes = R.drawable.shaker, 0xFFEB3B, fileName = "shaker.wav"),
    Taiko(drawableRes = R.drawable.taiko, 0x4CAF50, fileName = "taiko.wav"),
    Tambourines(drawableRes = R.drawable.tamburins, 0x2196F3, fileName = "tambourines.wav"),
    Stick(drawableRes = R.drawable.stick, 0x3F51B5, fileName = "stick.wav"),
    Bell(drawableRes = R.drawable.bell, 0x9C27B0, fileName = "bell.wav"),
    Conga(drawableRes = R.drawable.conga, 0x000000, fileName = "conga.wav"),
    ;

    fun filePath(): String {
        return "audio/${fileName}"
    }
}
