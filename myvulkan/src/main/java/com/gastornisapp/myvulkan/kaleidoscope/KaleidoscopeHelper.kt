package com.gastornisapp.myvulkan.kaleidoscope

import android.content.Context

data class KaleidoscopeImage(
    private val fileName: String
) {

    fun getUrl(): String {
        return "file:///android_asset/${getPath()}"
    }

    fun getPath(): String {
        return "images/kaleidoscope/$fileName"
    }

    companion object {

        fun getImages(context: Context): List<KaleidoscopeImage> {
            return context.assets.list("images/kaleidoscope")
                ?.sorted()
                ?.map { KaleidoscopeImage(it) }
                ?: emptyList()
        }
    }
}
