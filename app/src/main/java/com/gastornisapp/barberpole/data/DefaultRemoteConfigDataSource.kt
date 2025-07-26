package com.gastornisapp.barberpole.data

import com.google.firebase.remoteconfig.FirebaseRemoteConfig
import com.google.firebase.remoteconfig.remoteConfigSettings
import kotlinx.coroutines.tasks.await

class DefaultRemoteConfigDataSource() : RemoteConfigDataSource {
    private val remoteConfig: FirebaseRemoteConfig = FirebaseRemoteConfig.getInstance()

    companion object {
        private const val KEY_ANNOUNCEMENT = "announcement"
        private const val KEY_FORCE_UPDATE = "force_update_required"
        private const val KEY_TOS_VERSION = "latest_terms_of_service_version"
        private const val KEY_PRIVACY_POLICY_VERSION = "latest_privacy_policy_version"
    }

    init {
        // オプション: 開発中はキャッシュ時間を短く
        remoteConfig.setConfigSettingsAsync(
            remoteConfigSettings {
                // 通常は1時間、開発中は0
                minimumFetchIntervalInSeconds = 3600
            }
        )
    }

    override suspend fun fetchRemoteConfig(): Result<Unit> {
        return try {
            remoteConfig.fetchAndActivate().await()
            Result.success(Unit)
        } catch (e: Exception) {
            Result.failure(e)
        }
    }

    override fun getAnnouncement(): String {
        return remoteConfig.getString(KEY_ANNOUNCEMENT)
    }

    override fun isForceUpdateRequired(): Boolean {
        return remoteConfig.getBoolean(KEY_FORCE_UPDATE)
    }

    override fun getLatestTermsOfServiceVersion(): Int {
        return remoteConfig.getLong(KEY_TOS_VERSION).toInt()
    }

    override fun getLatestPrivacyPolicyVersion(): Int {
        return remoteConfig.getLong(KEY_PRIVACY_POLICY_VERSION).toInt()
    }
}