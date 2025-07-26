package com.gastornisapp.barberpole.di

import android.content.Context
import androidx.datastore.core.DataStore
import androidx.datastore.preferences.core.Preferences
import androidx.datastore.preferences.preferencesDataStore
import com.gastornisapp.barberpole.data.AppPreferencesDataSource
import com.gastornisapp.barberpole.data.DefaultAppPreferencesDataSource
import com.gastornisapp.barberpole.data.DefaultRemoteConfigDataSource
import com.gastornisapp.barberpole.data.RemoteConfigDataSource
import dagger.Binds
import dagger.Module
import dagger.Provides
import dagger.hilt.InstallIn
import dagger.hilt.android.qualifiers.ApplicationContext
import dagger.hilt.components.SingletonComponent
import javax.inject.Singleton

@Module
@InstallIn(SingletonComponent::class)
object DataSourceModule {

    @Provides
    @Singleton
    fun provideRemoteConfigDataSource(): RemoteConfigDataSource {
        return DefaultRemoteConfigDataSource()
    }


    private val Context.dataStore: DataStore<Preferences> by preferencesDataStore(name = "settings")

    @Provides
    @Singleton
    fun provideAAppPreferencesDataSource(@ApplicationContext context: Context): AppPreferencesDataSource {
        val dataStore = context.dataStore
        return DefaultAppPreferencesDataSource(dataStore)
    }
}

