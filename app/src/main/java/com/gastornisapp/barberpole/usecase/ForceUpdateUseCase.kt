package com.gastornisapp.barberpole.usecase

import com.gastornisapp.barberpole.domain.repository.AppSettingsRepository
import javax.inject.Inject

class ForceUpdateUseCase @Inject constructor(
    private val appSettingsRepository: AppSettingsRepository
) {
    fun isForceUpdateRequired(): Boolean {
        val requiredVersion = appSettingsRepository.getRequiredAppVersion().getOrNull() ?: return false
        val currentVersion = appSettingsRepository.getCurrentAppVersion().getOrNull() ?: return false
        return currentVersion < requiredVersion
    }
}