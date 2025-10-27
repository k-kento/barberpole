package com.gastornisapp.barberpole.ui.page.settings

sealed class SettingsEvent {

    object StartLock : SettingsEvent()

    object StopLock : SettingsEvent()

    object OpenSecuritySettings : SettingsEvent()

}
