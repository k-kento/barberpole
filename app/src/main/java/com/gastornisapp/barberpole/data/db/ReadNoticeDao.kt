package com.gastornisapp.barberpole.data.db

import androidx.room.Dao
import androidx.room.Insert
import androidx.room.OnConflictStrategy
import androidx.room.Query

@Dao
interface ReadNoticeDao {
    @Insert(onConflict = OnConflictStrategy.IGNORE)
    suspend fun insert(readNotice: ReadNotice)

    @Query("SELECT EXISTS(SELECT 1 FROM read_notice WHERE id = :id)")
    suspend fun exists(id: String): Boolean

    @Query("DELETE FROM read_notice WHERE id = :id")
    suspend fun delete(id: String)
}
