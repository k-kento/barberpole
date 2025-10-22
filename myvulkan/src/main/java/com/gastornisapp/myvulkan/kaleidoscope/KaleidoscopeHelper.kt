package com.gastornisapp.myvulkan.kaleidoscope

import android.net.Uri

enum class KaleidoscopeImage {
    A("bus_body.png"),
    B("car_body.png"),
    ;

    private val fileName: String;

    constructor(path: String) {
        this.fileName = path;
    }

    fun getUrl(): String {
        return "file:///android_asset/${getPath()}"
    }

    fun getPath(): String {
        return "images/kaleidoscope/$fileName";
    }

    companion object {
        fun getImages(): List<KaleidoscopeImage> {
            return KaleidoscopeImage.entries
        }
    }
}