package com.gastornisapp.barberpole.ui.settings

sealed class SettingsEvent {

    object StartLock : SettingsEvent()

    object StopLock : SettingsEvent()

    object OpenSecuritySettings : SettingsEvent()

}
