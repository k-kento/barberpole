package com.gastornisapp.barberpole.di

import com.gastornisapp.barberpole.domain.repository.AppSettingsRepository
import com.gastornisapp.barberpole.usecase.ForceUpdateUseCase
import com.gastornisapp.barberpole.usecase.NoticeUseCase
import dagger.Module
import dagger.Provides
import dagger.hilt.InstallIn
import dagger.hilt.components.SingletonComponent
import javax.inject.Singleton

@Module
@InstallIn(SingletonComponent::class)
object UseCaseModule {

    @Provides
    @Singleton
    fun provideForceUpdateUseCase(
        appSettingsRepository: AppSettingsRepository
    ): ForceUpdateUseCase {
        return ForceUpdateUseCase(appSettingsRepository = appSettingsRepository)
    }

    @Provides
    @Singleton
    fun provideNoticeUseCase(
        appSettingsRepository: AppSettingsRepository
    ): NoticeUseCase {
        return NoticeUseCase(appSettingsRepository = appSettingsRepository)
    }
}
