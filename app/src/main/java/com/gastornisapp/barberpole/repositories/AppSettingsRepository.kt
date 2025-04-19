package com.gastornisapp.barberpole.repositories

import android.content.Context
import android.util.Log
import androidx.datastore.core.DataStore
import androidx.datastore.preferences.core.Preferences
import androidx.datastore.preferences.core.edit
import androidx.datastore.preferences.core.intPreferencesKey
import androidx.datastore.preferences.preferencesDataStore
import kotlinx.coroutines.flow.Flow
import kotlinx.coroutines.flow.map
import kotlinx.coroutines.flow.onCompletion

val Context.dataStore: DataStore<Preferences> by preferencesDataStore(name = "app_settings")

class AppSettingsRepository(private val dataStore: DataStore<Preferences>) {

    val version: Flow<Int> = dataStore.data
        .map { it[KEY_TOS_VERSION] ?: 0 }
        .onCompletion { cause ->
            if (cause == null) {
                Log.i("AppSettingsRepository", "正常終了")
            } else {
                Log.i("AppSettingsRepository", "error :$cause")
            }
        }

    suspend fun saveVersion(version: Int = 1) {
        dataStore.edit { it[KEY_TOS_VERSION] = version }
    }

    companion object {
        private val KEY_TOS_VERSION = intPreferencesKey("terms_of_service_version")
    }
}