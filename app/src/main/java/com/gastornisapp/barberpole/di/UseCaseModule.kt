package com.gastornisapp.barberpole.di

import com.gastornisapp.barberpole.domain.port.SoundPlayer
import com.gastornisapp.barberpole.domain.repository.AppSettingsRepository
import com.gastornisapp.barberpole.domain.usecase.ForceUpdateUseCase
import com.gastornisapp.barberpole.domain.usecase.NoticeUseCase
import com.gastornisapp.barberpole.domain.usecase.PlaySoundUseCase
import dagger.Module
import dagger.Provides
import dagger.hilt.InstallIn
import dagger.hilt.android.components.ViewModelComponent
import dagger.hilt.android.scopes.ViewModelScoped

@Module
@InstallIn(ViewModelComponent::class)
object UseCaseModule {

    @Provides
    @ViewModelScoped
    fun provideForceUpdateUseCase(
        appSettingsRepository: AppSettingsRepository
    ): ForceUpdateUseCase {
        return ForceUpdateUseCase(appSettingsRepository = appSettingsRepository)
    }

    @Provides
    @ViewModelScoped
    fun provideNoticeUseCase(
        appSettingsRepository: AppSettingsRepository
    ): NoticeUseCase {
        return NoticeUseCase(appSettingsRepository = appSettingsRepository)
    }

    @Provides
    @ViewModelScoped
    fun providePlaySoundUseCase(
        soundPlayer: SoundPlayer
    ): PlaySoundUseCase {
        return PlaySoundUseCase(soundPlayer = soundPlayer)
    }
}
