package com.gastornisapp.barberpole.data

import android.util.Log
import com.gastornisapp.barberpole.data.db.NoticeConfig
import com.google.firebase.remoteconfig.FirebaseRemoteConfig
import kotlinx.coroutines.tasks.await
import kotlinx.serialization.json.Json

class DefaultRemoteConfigDataSource() : RemoteConfigDataSource {
    private val remoteConfig: FirebaseRemoteConfig = FirebaseRemoteConfig.getInstance()

    companion object {
        private const val KEY_REQUIRED_APP_VERSION = "required_app_version"
        private const val KEY_TOS_VERSION = "latest_terms_of_service_version"
        private const val KEY_PRIVACY_POLICY_VERSION = "latest_privacy_policy_version"

        private val jsonParser = Json { ignoreUnknownKeys = true } // 拡張性をもたせる

        private const val NOTICE_KEY = "notice"

        private const val TAG = "DefaultRemoteConfigDataSource"
    }

    override suspend fun fetchRemoteConfig(): Result<Unit> {
        return try {
            val result = remoteConfig.fetchAndActivate().await()
            Log.d(TAG, "fetchAndActivate result: $result")
            Result.success(Unit)
        } catch (e: Exception) {
            Result.failure(e)
        }
    }

    override suspend fun getNoticeConfig(): Result<NoticeConfig> {
        val jsonString = remoteConfig.getString(NOTICE_KEY)
        return try {
            val config = jsonParser.decodeFromString<NoticeConfig>(jsonString)
            Result.success(config)
        } catch (e: Exception) {
            Log.e("RemoteConfig", "JSON parse error", e)
            Result.failure(e)
        }
    }

    override fun getRequiredAppVersion(): String {
        return remoteConfig.getString(KEY_REQUIRED_APP_VERSION)
    }

    override fun getLatestTermsOfServiceVersion(): Int {
        return remoteConfig.getLong(KEY_TOS_VERSION).toInt()
    }

    override fun getLatestPrivacyPolicyVersion(): Int {
        return remoteConfig.getLong(KEY_PRIVACY_POLICY_VERSION).toInt()
    }
}
