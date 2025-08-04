package com.gastornisapp.barberpole.usecase

import com.gastornisapp.barberpole.domain.model.Notice
import com.gastornisapp.barberpole.domain.repository.AppSettingsRepository
import io.mockk.coEvery
import io.mockk.mockk
import org.junit.jupiter.api.Assertions.assertEquals
import kotlinx.coroutines.test.runTest
import org.junit.jupiter.params.ParameterizedTest
import org.junit.jupiter.params.provider.MethodSource

class NoticeUseCaseTest {

    private lateinit var repository: AppSettingsRepository
    private lateinit var useCase: NoticeUseCase

    companion object {
        data class TestCase(
            val noticeInput: List<Notice>,
            val readStatusMap: Map<String, Result<Boolean>>,
            val expected: Notice?
        )

        @JvmStatic
        fun noticeTestCases() = listOf(
            TestCase(
                noticeInput = listOf(makeNotice("1", TimeRange.WITHIN)),
                readStatusMap = mapOf("1" to Result.success(false)),
                expected = makeNotice("1", TimeRange.WITHIN)
            ),
            TestCase(
                noticeInput = listOf(makeNotice("2", TimeRange.WITHIN)),
                readStatusMap = mapOf("2" to Result.success(true)),
                expected = null
            ),
            TestCase(
                noticeInput = listOf(makeNotice("3", TimeRange.AFTER)),
                readStatusMap = mapOf("3" to Result.success(false)),
                expected = null
            ),
            TestCase(
                noticeInput = listOf(
                    makeNotice("4-1", TimeRange.WITHIN),
                    makeNotice("4-2", TimeRange.WITHIN)
                ),
                readStatusMap = mapOf(
                    "4-1" to Result.success(false),
                    "4-2" to Result.success(false)
                ),
                expected = makeNotice("4-1", TimeRange.WITHIN)
            ),
            TestCase(
                noticeInput = listOf(
                    makeNotice("5-1", TimeRange.WITHIN),
                    makeNotice("5-2", TimeRange.WITHIN)
                ),
                readStatusMap = mapOf(
                    "5-1" to Result.success(true),
                    "5-2" to Result.success(true)
                ),
                expected = null
            ),
            TestCase(
                noticeInput = listOf(
                    makeNotice("6-1", TimeRange.WITHIN),
                    makeNotice("6-2", TimeRange.WITHIN)
                ),
                readStatusMap = mapOf(
                    "6-1" to Result.success(true),
                    "6-2" to Result.success(false)
                ),
                expected = makeNotice("6-2", TimeRange.WITHIN)
            ),
            TestCase(
                noticeInput = emptyList(),
                readStatusMap = emptyMap(),
                expected = null
            ),
            TestCase(
                noticeInput = listOf(makeNotice("8", TimeRange.WITHIN)),
                readStatusMap = mapOf("8" to Result.failure(Exception("fail"))),
                expected = makeNotice("8", TimeRange.WITHIN)
            )
        )

        private fun makeNotice(id: String, timeRange: TimeRange): Notice {
            val now = System.currentTimeMillis() / 1000
            return Notice(
                id = id,
                title = "title",
                message = "message",
                startAt = now + timeRange.startOffset,
                endAt = now + timeRange.endOffset
            )
        }
    }

    @ParameterizedTest
    @MethodSource("noticeTestCases")
    fun test_getCurrentNotice_parametrized(testCase: TestCase) = runTest {
        repository = mockk()
        useCase = NoticeUseCase(repository)

        coEvery { repository.getNotice() } returns Result.success(testCase.noticeInput)
        for ((id, result) in testCase.readStatusMap) {
            coEvery { repository.isNoticeRead(id) } returns result
        }

        val actual = useCase.getCurrentNotice()
        assertEquals(Result.success(testCase.expected), actual)
    }
}

private enum class TimeRange(val startOffset: Long, val endOffset: Long) {
    BEFORE(-100, -50),
    WITHIN(-10, 10),
    AFTER(50, 100)
}
