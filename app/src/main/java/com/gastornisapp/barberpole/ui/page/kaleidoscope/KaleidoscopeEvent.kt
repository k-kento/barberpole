package com.gastornisapp.barberpole.ui.page.kaleidoscope

sealed interface KaleidoscopeEvent {
    data object RotateCW : KaleidoscopeEvent
    data object RotateCCW : KaleidoscopeEvent
    data object StopRotation : KaleidoscopeEvent

    data object SelectButtonClicked : KaleidoscopeEvent
    data object ImagePickerDismissed : KaleidoscopeEvent
    data class ImageSelected(val imageItem: ImageItem) : KaleidoscopeEvent
}
