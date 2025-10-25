package com.gastornisapp.barberpole.ui.settings

sealed class SettingsIntent {

    object ToggleLockMode : SettingsIntent()

    object TapSecurityScreenButton : SettingsIntent()

}
