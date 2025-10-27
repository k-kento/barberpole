package com.gastornisapp.barberpole.ui.page.kaleidoscope

import com.gastornisapp.myvulkan.kaleidoscope.KaleidoscopeImage

class ImageItem(
    val kaleidoscopeImage: KaleidoscopeImage
) {
    val assetPath: String
        get() = kaleidoscopeImage.getUrl()

}
