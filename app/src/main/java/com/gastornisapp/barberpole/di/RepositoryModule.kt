package com.gastornisapp.barberpole.di

import com.gastornisapp.barberpole.data.AppPreferencesDataSource
import com.gastornisapp.barberpole.data.DefaultAppSettingsRepository
import com.gastornisapp.barberpole.data.RemoteConfigDataSource
import com.gastornisapp.barberpole.domain.repository.AppSettingsRepository
import dagger.Module
import dagger.Provides
import dagger.hilt.InstallIn
import dagger.hilt.components.SingletonComponent
import javax.inject.Singleton

@Module
@InstallIn(SingletonComponent::class)
object RepositoryModule {

    @Provides
    @Singleton
    fun provideAppSettingsRepository(
        remoteConfigDataSource: RemoteConfigDataSource,
        appPreferencesDataSource: AppPreferencesDataSource
    ): AppSettingsRepository {
        return DefaultAppSettingsRepository(
            remoteConfigDataSource = remoteConfigDataSource,
            appPreferencesDataSource = appPreferencesDataSource
        )
    }
}
