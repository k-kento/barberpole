package com.gastornisapp.barberpole.ui.page.barberpole

enum class Orientation {
    Left,
    Right;

    fun toggle(): Orientation {
        return when (this) {
            Left -> Right
            Right -> Left
        }
    }
}
