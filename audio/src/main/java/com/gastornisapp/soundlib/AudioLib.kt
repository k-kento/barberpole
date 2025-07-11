package com.gastornisapp.soundlib

import android.content.res.AssetManager

class AudioLib {

    private var nativeHandle: Long = 0L

    fun create() {
        nativeHandle = createNativeEngine()
    }

    fun load(assetManager: AssetManager, resource: AudioResource) {
        load(nativeHandle, assetManager, resource.fileName)
    }

    fun start() {
        start(nativeHandle)
    }

    fun stop() {
        stop(nativeHandle)
    }

    fun release() {
        release(nativeHandle)
        nativeHandle = 0L
    }

    companion object {
        fun initialize() {
            System.loadLibrary("audio-lib")
        }
    }

    private external fun createNativeEngine(): Long
    private external fun load(nativeHandle: Long, assetsManager: AssetManager, resource: String)
    private external fun start(nativeHandle: Long)
    private external fun stop(nativeHandle: Long)
    private external fun release(nativeHandle: Long)
}