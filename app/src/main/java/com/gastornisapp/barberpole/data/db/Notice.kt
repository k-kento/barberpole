package com.gastornisapp.barberpole.data.db

import com.gastornisapp.barberpole.data.EpochSecondsAsStringSerializer
import kotlinx.serialization.Serializable

@Serializable
data class NoticeConfig(
    val version: Int,
    val notices: List<Notice>
)

@Serializable
data class Notice(
    val id: String,
    val title: String,
    val message: String,
    val url: String? = null,
    @Serializable(with = EpochSecondsAsStringSerializer::class)
    val startAt: Long,
    @Serializable(with = EpochSecondsAsStringSerializer::class)
    val endAt: Long
)