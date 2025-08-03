package com.gastornisapp.barberpole.domain.model

class Notice(
    val id: String,
    val title: String,
    val message: String,
    val url: String? = null,
    /**
     * 公開開始時(秒)
     */
    val startAt: Long,
    /**
     * 公開終了時(秒)
     */
    val endAt: Long
)