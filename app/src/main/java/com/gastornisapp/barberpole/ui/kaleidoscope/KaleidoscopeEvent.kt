package com.gastornisapp.barberpole.ui.kaleidoscope

sealed interface KaleidoscopeEvent {
    data object RotateCW : KaleidoscopeEvent
    data object RotateCCW : KaleidoscopeEvent
    data object StopRotation : KaleidoscopeEvent
}
