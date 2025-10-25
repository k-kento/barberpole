package com.gastornisapp.barberpole.domain.usecase

import com.gastornisapp.barberpole.domain.repository.AppSettingsRepository
import kotlinx.coroutines.flow.Flow
import javax.inject.Inject

class AppLockUseCase @Inject constructor(
    private val appSettingsRepository: AppSettingsRepository
) {

    fun isLockEnabled(): Flow<Boolean> {
        return appSettingsRepository.isLockEnabled()
    }

    suspend fun setLockEnabled(enabled: Boolean) {
        return appSettingsRepository.setLockEnabled(enabled)
    }
}
