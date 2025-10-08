package com.gastornisapp.barberpole.di

import android.content.Context
import com.gastornisapp.barberpole.data.datastore.AppPreferencesDataSource
import com.gastornisapp.barberpole.data.DefaultAppSettingsRepository
import com.gastornisapp.barberpole.data.remoteconfig.RemoteConfigDataSource
import com.gastornisapp.barberpole.data.db.ReadNoticeDao
import com.gastornisapp.barberpole.domain.repository.AppSettingsRepository
import dagger.Module
import dagger.Provides
import dagger.hilt.InstallIn
import dagger.hilt.android.qualifiers.ApplicationContext
import dagger.hilt.components.SingletonComponent
import javax.inject.Singleton

@Module
@InstallIn(SingletonComponent::class)
object RepositoryModule {

    @Provides
    @Singleton
    fun provideAppSettingsRepository(
        @ApplicationContext context: Context,
        remoteConfigDataSource: RemoteConfigDataSource,
        appPreferencesDataSource: AppPreferencesDataSource,
        readNoticeDao: ReadNoticeDao
    ): AppSettingsRepository {
        return DefaultAppSettingsRepository(
            context = context,
            remoteConfigDataSource = remoteConfigDataSource,
            appPreferencesDataSource = appPreferencesDataSource,
            readNoticeDao = readNoticeDao
        )
    }
}
