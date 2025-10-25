package com.gastornisapp.barberpole.data

import android.content.Context
import com.gastornisapp.barberpole.data.datastore.AppPreferencesDataSource
import com.gastornisapp.barberpole.data.db.ReadNotice
import com.gastornisapp.barberpole.data.db.ReadNoticeDao
import com.gastornisapp.barberpole.data.remoteconfig.RemoteConfigDataSource
import com.gastornisapp.barberpole.domain.repository.AppSettingsRepository
import com.gastornisapp.barberpole.domain.entity.Notice
import com.gastornisapp.barberpole.domain.entity.SemVer
import kotlinx.coroutines.flow.Flow

class DefaultAppSettingsRepository(
    private val context: Context,
    private val remoteConfigDataSource: RemoteConfigDataSource,
    private val appPreferencesDataSource: AppPreferencesDataSource,
    private val readNoticeDao: ReadNoticeDao,
) : AppSettingsRepository {

    override suspend fun refreshConfig(): Result<Unit> {
        return remoteConfigDataSource.fetchRemoteConfig()
    }

    override suspend fun getNotice(): Result<List<Notice>> {
        return remoteConfigDataSource.getNoticeConfig().mapCatching { config ->
            config.notices.map {
                Notice(
                    id = it.id,
                    title = it.title,
                    message = it.message,
                    url = it.url,
                    startAt = it.startAt,
                    endAt = it.endAt
                )
            }
        }
    }

    override suspend fun setNoticeId(noticeId: String): Result<Unit> {
        return runCatching {
            readNoticeDao.insert(ReadNotice(noticeId))
        }
    }

    override suspend fun isNoticeRead(noticeId: String): Result<Boolean> {
        return runCatching {
            readNoticeDao.exists(noticeId)
        }
    }

    override suspend fun getLatestTermsOfServiceVersion(): Int {
        return remoteConfigDataSource.getLatestTermsOfServiceVersion()
    }

    override suspend fun getTermsOfServiceAcceptedVersion(): Int? {
        return appPreferencesDataSource.getTermsOfServiceAcceptedVersion()
    }

    override suspend fun setTermsOfServiceAcceptedVersion(version: Int) {
        return appPreferencesDataSource.setTermsOfServiceAcceptedVersion(version)
    }

    override suspend fun getLatestPrivacyPolicyVersion(): Int {
        return remoteConfigDataSource.getLatestPrivacyPolicyVersion()
    }

    override suspend fun getPrivacyPolicyAcceptedVersion(): Int? {
        return appPreferencesDataSource.getPrivacyPolicyAcceptedVersion()
    }

    override suspend fun setPrivacyPolicyAcceptedVersion(version: Int) {
        appPreferencesDataSource.setPrivacyPolicyAcceptedVersion(version)
    }

    override fun isLockEnabled(): Flow<Boolean> {
        return appPreferencesDataSource.isLockEnabled()
    }

    override suspend fun setLockEnabled(enabled: Boolean) {
        appPreferencesDataSource.setLockEnabled(enabled)
    }

    override fun getCurrentAppVersion(): Result<SemVer> {
        val packageInfo = context.packageManager.getPackageInfo(context.packageName, 0)
        val version = packageInfo.versionName ?: return Result.failure(IllegalStateException("versionName is null"))
        return SemVer.parse(version)
    }

    override fun getRequiredAppVersion(): Result<SemVer> {
        return remoteConfigDataSource.getRequiredAppVersion()
    }
}
