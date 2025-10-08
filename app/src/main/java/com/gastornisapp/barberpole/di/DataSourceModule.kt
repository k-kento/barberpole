package com.gastornisapp.barberpole.di

import android.content.Context
import androidx.datastore.core.DataStore
import androidx.datastore.preferences.core.Preferences
import androidx.datastore.preferences.preferencesDataStore
import androidx.room.Room
import com.gastornisapp.barberpole.data.datastore.AppPreferencesDataSource
import com.gastornisapp.barberpole.data.datastore.DefaultAppPreferencesDataSource
import com.gastornisapp.barberpole.data.remoteconfig.DefaultRemoteConfigDataSource
import com.gastornisapp.barberpole.data.remoteconfig.RemoteConfigDataSource
import com.gastornisapp.barberpole.data.db.AppDatabase
import com.gastornisapp.barberpole.data.db.ReadNoticeDao
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
    fun provideAppPreferencesDataSource(@ApplicationContext context: Context): AppPreferencesDataSource {
        val dataStore = context.dataStore
        return DefaultAppPreferencesDataSource(dataStore)
    }

    @Provides
    @Singleton
    fun provideAppDatabase(@ApplicationContext context: Context): AppDatabase {
        return Room.databaseBuilder(
            context,
            AppDatabase::class.java,
            "app.db"
        ).build()
    }

    @Provides
    @Singleton
    fun provideReadNoticeDao(db: AppDatabase): ReadNoticeDao {
        return db.readNoticeDao()
    }
}
