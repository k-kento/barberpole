package com.gastornisapp.barberpole.ui.page.settings

sealed class SettingsIntent {

    object ToggleLockMode : SettingsIntent()

    object TapSecurityScreenButton : SettingsIntent()

}
