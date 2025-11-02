package com.gastornisapp.myvulkan

import android.view.Surface

object Utility {

    fun rotationToDegrees(rotation: Int): Int {
        return when (rotation) {
            Surface.ROTATION_0 -> 0
            Surface.ROTATION_90 -> 90
            Surface.ROTATION_180 -> 180
            Surface.ROTATION_270 -> 270
            else -> 0 // 万一のため
        }
    }

}
