package com.gastornisapp.barberpole.viewmodel

import com.gastornisapp.barberpole.domain.model.Notice
import com.gastornisapp.barberpole.domain.model.SemVer
import com.gastornisapp.barberpole.domain.repository.AppSettingsRepository
import com.gastornisapp.barberpole.ui.home.HomeViewModel
import io.mockk.coEvery
import io.mockk.mockk
import junit.framework.TestCase.assertEquals
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.ExperimentalCoroutinesApi
import kotlinx.coroutines.flow.filterNotNull
import kotlinx.coroutines.flow.first
import kotlinx.coroutines.test.StandardTestDispatcher
import kotlinx.coroutines.test.resetMain
import kotlinx.coroutines.test.runTest
import kotlinx.coroutines.test.setMain
import kotlinx.coroutines.withTimeoutOrNull
import org.junit.After
import org.junit.Before
import org.junit.Test

class HomeViewModelTest {

    private lateinit var repository: AppSettingsRepository
    private val testDispatcher = StandardTestDispatcher()

    @OptIn(ExperimentalCoroutinesApi::class)
    @Before
    fun setup() {
        Dispatchers.setMain(testDispatcher)
        repository = mockk()
        coEvery { repository.getRequiredAppVersion() } returns Result.failure(Exception())
        coEvery { repository.getCurrentAppVersion() } returns Result.failure(Exception())
        coEvery { repository.getNotice() } returns Result.failure(Exception())
        coEvery { repository.isNoticeRead(any()) } returns Result.failure(Exception())
    }

    @OptIn(ExperimentalCoroutinesApi::class)
    @After
    fun tearDown() {
        Dispatchers.resetMain()
    }

    // region state

    @Test
    fun `isForceUpdateRequired returns false when getRequiredAppVersion fails`() = runTest {
        coEvery { repository.getRequiredAppVersion() } returns Result.failure(Exception())
        coEvery { repository.getCurrentAppVersion() } returns Result.success(SemVer(0, 0, 0))

        val viewModel = HomeViewModel(repository)

        assertEquals("Required version failure should return false", false, viewModel.isForceUpdateRequired.value)
    }

    @Test
    fun `isForceUpdateRequired returns false when getCurrentAppVersion fails`() = runTest {
        coEvery { repository.getRequiredAppVersion() } returns Result.success(SemVer(0, 0, 0))
        coEvery { repository.getCurrentAppVersion() } returns Result.failure(Exception())

        val viewModel = HomeViewModel(repository)

        assertEquals("Current version failure should return false", false, viewModel.isForceUpdateRequired.value)
    }

    @Test
    fun `isForceUpdateRequired returns false when current version equals required`() = runTest {
        coEvery { repository.getRequiredAppVersion() } returns Result.success(SemVer(0, 0, 0))
        coEvery { repository.getCurrentAppVersion() } returns Result.success(SemVer(0, 0, 0))

        val viewModel = HomeViewModel(repository)

        assertEquals("Same version should not require update", false, viewModel.isForceUpdateRequired.value)
    }

    @Test
    fun `isForceUpdateRequired returns false when current version is newer`() = runTest {
        coEvery { repository.getRequiredAppVersion() } returns Result.success(SemVer(0, 0, 0))
        coEvery { repository.getCurrentAppVersion() } returns Result.success(SemVer(0, 0, 1))

        val viewModel = HomeViewModel(repository)

        assertEquals("Newer version should not require update", false, viewModel.isForceUpdateRequired.value)
    }

    @Test
    fun `isForceUpdateRequired returns true when current version is older`() = runTest {
        coEvery { repository.getRequiredAppVersion() } returns Result.success(SemVer(0, 0, 1))
        coEvery { repository.getCurrentAppVersion() } returns Result.success(SemVer(0, 0, 0))

        val viewModel = HomeViewModel(repository)

        assertEquals("Older version should require update", true, viewModel.isForceUpdateRequired.value)
    }

    /*
    | パターン | getNotice の返り値 | isNoticeRead の返り値 | 時刻条件 | 期待される currentNotice |
    |----------|---------------------|------------------------|-----------|--------------------------|
    | ①        | 1件                 | false                  | 範囲内    | その Notice             |
    | ②        | 1件                 | true                   | 範囲内    | null                    |
    | ③        | 1件                 | false                  | 範囲外    | null                    |
    | ④        | 複数               | 全て false             | 範囲内    | 最初の Notice           |
    | ⑤        | 複数               | 全て true              | 範囲内    | null                    |
    | ⑥        | 複数               | 一部 false             | 一部範囲内| 最初に一致した Notice   |
    | ⑦        | 空リスト            | ―                     | ―         | null                    |
    | ⑧        | 1件                 | Result.failure         | 範囲内    | その Notice（未読扱い）|
    */

    private fun makeNotice(id: String, startOffset: Long, endOffset: Long): Notice {
        val now = System.currentTimeMillis() / 1000
        return Notice(
            id = id, title = "title", message = "message", startAt = now + startOffset, endAt = now + endOffset
        )
    }

    private suspend fun runNoticeTest(
        noticeInput: List<Notice>,
        readStatusMap: Map<String, Result<Boolean>>,
        expected: Notice?
    ) {
        coEvery { repository.getNotice() } returns Result.success(noticeInput)
        for ((id, result) in readStatusMap) {
            coEvery { repository.isNoticeRead(id) } returns result
        }

        val viewModel = HomeViewModel(repository)

        val actual = withTimeoutOrNull(10000) {
            viewModel.currentNotice.filterNotNull().first()
        }
        assertEquals(expected, actual)
    }

    @Test
    fun `case 1 unread in range`() = runTest {
        val notice = makeNotice("1", -10, 10)
        runNoticeTest(listOf(notice), mapOf("1" to Result.success(false)), notice)
    }

    @Test
    fun `case 2 read in range`() = runTest {
        val notice = makeNotice("2", -10, 10)
        runNoticeTest(listOf(notice), mapOf("2" to Result.success(true)), null)
    }

    @Test
    fun `case 3 unread out of range`() = runTest {
        val notice = makeNotice("3", 10, 20)
        runNoticeTest(listOf(notice), mapOf("3" to Result.success(false)), null)
    }

    @Test
    fun `case 4 two unread valid, pick first`() = runTest {
        val notices = listOf(
            makeNotice("4-1", -10, 10), makeNotice("4-2", -10, 10)
        )
        runNoticeTest(
            notices, mapOf(
                "4-1" to Result.success(false), "4-2" to Result.success(false)
            ), notices[0]
        )
    }

    @Test
    fun `case 5 all read`() = runTest {
        val notices = listOf(
            makeNotice("5-1", -10, 10), makeNotice("5-2", -10, 10)
        )
        runNoticeTest(
            notices, mapOf(
                "5-1" to Result.success(true), "5-2" to Result.success(true)
            ), null
        )
    }

    @Test
    fun `case 6 one read one unread`() = runTest {
        val notices = listOf(
            makeNotice("6-1", -10, 10), makeNotice("6-2", -10, 10)
        )
        runNoticeTest(
            notices, mapOf(
                "6-1" to Result.success(true), "6-2" to Result.success(false)
            ), notices[1]
        )
    }

    @Test
    fun `case 7 empty list`() = runTest {
        runNoticeTest(emptyList(), emptyMap(), null)
    }

    @Test
    fun `case 8 read check failure treated as unread`() = runTest {
        val notice = makeNotice("8", -10, 10)
        runNoticeTest(listOf(notice), mapOf("8" to Result.failure(Exception("fail"))), notice)
    }
}