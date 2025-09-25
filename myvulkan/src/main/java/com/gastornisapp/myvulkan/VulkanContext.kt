package com.gastornisapp.myvulkan

import android.content.Context
import android.content.res.AssetManager

class VulkanContext {

    internal var nativeHandle: Long = 0L
        private set

    fun init(context: Context) {
        if (!isLibraryLoaded) {
            System.loadLibrary("myvulkan")
            isLibraryLoaded = true
        }
        if (nativeHandle == 0L) {
            nativeHandle = nativeCreate(context.assets)
        }
    }

    fun release() {
        if (nativeHandle != 0L) {
            nativeDestroy(nativeHandle)
        }
        nativeHandle = 0L
    }

    private external fun nativeCreate(assetManager: AssetManager): Long
    private external fun nativeDestroy(nativeHandle: Long)

    companion object {
        private var isLibraryLoaded = false
    }
}
