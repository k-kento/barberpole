package com.gastornisapp.barberpole.ui.home

sealed class HomeEvent {
    object Info : HomeEvent()
    object Settings : HomeEvent()
}
