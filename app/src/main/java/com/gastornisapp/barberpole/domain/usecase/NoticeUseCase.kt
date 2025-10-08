package com.gastornisapp.barberpole.domain.usecase

import com.gastornisapp.barberpole.domain.entity.Notice
import com.gastornisapp.barberpole.domain.repository.AppSettingsRepository
import javax.inject.Inject

class NoticeUseCase @Inject constructor(
    private val appSettingsRepository: AppSettingsRepository
) {

    suspend fun getCurrentNotice(): Result<Notice?> = runCatching {
        val notices = appSettingsRepository.getNotice().getOrThrow()

        val readStatusMap = notices.associate { notice ->
            notice.id to appSettingsRepository.isNoticeRead(notice.id).getOrDefault(false)
        }

        val nowSec = System.currentTimeMillis() / 1000
        val filtered = notices.filter {
            it.startAt <= nowSec && nowSec <= it.endAt && readStatusMap[it.id] == false
        }

        filtered.firstOrNull()
    }

    suspend fun setNoticeId(noticeId: String): Result<Unit> = runCatching {
        appSettingsRepository.setNoticeId(noticeId)
    }
}