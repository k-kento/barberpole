package com.gastornisapp.barberpole.ui.page.home

sealed class HomeIntent {
    object TapMenu: HomeIntent()
    object CloseMenu: HomeIntent()
    object TapInfo: HomeIntent()
    object TapSettings: HomeIntent()
}
