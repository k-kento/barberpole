package com.gastornisapp.barberpole.di

import com.gastornisapp.barberpole.domain.repository.AppSettingsRepository
import com.gastornisapp.barberpole.usecase.ForceUpdateUseCase
import com.gastornisapp.barberpole.usecase.NoticeUseCase
import dagger.Module
import dagger.Provides
import dagger.hilt.InstallIn
import dagger.hilt.android.components.ActivityRetainedComponent
import dagger.hilt.android.scopes.ActivityRetainedScoped

@Module
@InstallIn(ActivityRetainedComponent::class)
object UseCaseModule {

    @Provides
    @ActivityRetainedScoped
    fun provideForceUpdateUseCase(
        appSettingsRepository: AppSettingsRepository
    ): ForceUpdateUseCase {
        return ForceUpdateUseCase(appSettingsRepository = appSettingsRepository)
    }

    @Provides
    @ActivityRetainedScoped
    fun provideNoticeUseCase(
        appSettingsRepository: AppSettingsRepository
    ): NoticeUseCase {
        return NoticeUseCase(appSettingsRepository = appSettingsRepository)
    }
}
