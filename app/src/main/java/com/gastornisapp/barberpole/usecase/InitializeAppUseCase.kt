package com.gastornisapp.barberpole.usecase

import com.gastornisapp.barberpole.domain.repository.AppSettingsRepository
import com.gastornisapp.soundlib.AudioLib
import javax.inject.Inject

class InitializeAppUseCase @Inject constructor(
    private val repository: AppSettingsRepository
) {
    suspend fun initializeApp() {
        repository.refreshConfig()
        AudioLib.initialize() // TODO
    }
}
