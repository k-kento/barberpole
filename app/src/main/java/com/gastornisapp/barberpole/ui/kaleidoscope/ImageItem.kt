package com.gastornisapp.barberpole.ui.kaleidoscope

import com.gastornisapp.myvulkan.kaleidoscope.KaleidoscopeImage

class ImageItem(
    val kaleidoscopeImage: KaleidoscopeImage
) {
    val assetPath: String
        get() = kaleidoscopeImage.getUrl()

}
