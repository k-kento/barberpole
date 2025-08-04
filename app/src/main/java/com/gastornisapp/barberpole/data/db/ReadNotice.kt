package com.gastornisapp.barberpole.data.db

import androidx.room.Entity
import androidx.room.PrimaryKey

@Entity(tableName = "read_notice")
data class ReadNotice(
    @PrimaryKey val id: String
)