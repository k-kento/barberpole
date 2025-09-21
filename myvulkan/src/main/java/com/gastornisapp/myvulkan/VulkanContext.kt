package com.gastornisapp.myvulkan

class VulkanContext {

    internal var nativeHandle: Long = 0L
        private set

    fun init() {
        if (!isLibraryLoaded) {
            System.loadLibrary("myvulkan")
            isLibraryLoaded = true
        }
        if (nativeHandle == 0L) {
            nativeHandle = createNative()
        }
    }

    fun release() {
        if (nativeHandle != 0L) {
            release(nativeHandle)
        }
        nativeHandle = 0L
    }

    private external fun createNative(): Long
    private external fun release(nativeHandle: Long)

    companion object {
        private var isLibraryLoaded = false
    }
}
