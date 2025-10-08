package com.gastornisapp.barberpole.domain.usecase

import com.gastornisapp.barberpole.domain.repository.AppSettingsRepository
import javax.inject.Inject

class InitializeAppUseCase @Inject constructor(
    private val repository: AppSettingsRepository
) {
    suspend fun initializeApp() {
        repository.refreshConfig()
    }
}
