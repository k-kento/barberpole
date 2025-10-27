package com.gastornisapp.barberpole.ui.page.home

sealed class HomeEvent {
    object Info : HomeEvent()
    object Settings : HomeEvent()
}
