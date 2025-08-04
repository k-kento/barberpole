package com.gastornisapp.barberpole.viewmodel

import com.gastornisapp.barberpole.domain.model.Notice
import com.gastornisapp.barberpole.ui.home.DialogStatus
import com.gastornisapp.barberpole.ui.home.HomeViewModel
import com.gastornisapp.barberpole.usecase.ForceUpdateUseCase
import com.gastornisapp.barberpole.usecase.NoticeUseCase
import io.mockk.coEvery
import io.mockk.coVerify
import io.mockk.mockk
import kotlinx.coroutines.test.UnconfinedTestDispatcher
import kotlinx.coroutines.test.runTest
import org.junit.jupiter.api.Assertions.assertEquals
import org.junit.jupiter.api.BeforeEach
import org.junit.jupiter.params.ParameterizedTest
import org.junit.jupiter.params.provider.MethodSource
import org.junit.jupiter.params.provider.ValueSource

class HomeViewModelTest {
    private lateinit var forceUpdateUseCase: ForceUpdateUseCase
    private lateinit var noticeUseCase: NoticeUseCase

    @BeforeEach
    fun setup() {
        forceUpdateUseCase = mockk(relaxed = true)
        noticeUseCase = mockk(relaxed = true)
    }

    data class DialogTestCase(
        val forceUpdate: Boolean,
        val notice: Result<Notice?>,
        val expected: DialogStatus
    )

    @ParameterizedTest
    @MethodSource("provideCases")
    fun testDialogStatus(case: DialogTestCase) = runTest {
        coEvery { forceUpdateUseCase.isForceUpdateRequired() } returns case.forceUpdate
        coEvery { noticeUseCase.getCurrentNotice() } returns case.notice

        val viewModel = HomeViewModel(
            forceUpdateUseCase = forceUpdateUseCase,
            noticeUseCase = noticeUseCase,
            dispatcher = UnconfinedTestDispatcher()
        )

        val actual = viewModel.dialogStatus.value
        assertEquals(case.expected, actual)
    }

    @ParameterizedTest(name = "setNoticeId called with id={0}")
    @ValueSource(strings = ["1", "2"])
    fun `setNoticeId called with various ids`(noticeId: String) = runTest {
        coEvery { forceUpdateUseCase.isForceUpdateRequired() } returns false
        coEvery { noticeUseCase.getCurrentNotice() } returns Result.failure(Exception())
        coEvery { noticeUseCase.setNoticeId(any()) } returns Result.success(Unit)

        val viewModel = HomeViewModel(
            forceUpdateUseCase = forceUpdateUseCase,
            noticeUseCase = noticeUseCase,
            dispatcher = UnconfinedTestDispatcher()
        )

        viewModel.onNoticeDismissed(noticeId)

        coVerify(exactly = 1) { noticeUseCase.setNoticeId(noticeId) }
    }
    companion object {
        @JvmStatic
        fun provideCases() = listOf(
            DialogTestCase(false, Result.failure(Exception()), DialogStatus.None),
            DialogTestCase(false, Result.success(null), DialogStatus.None),
            DialogTestCase(false, Result.success(makeNotice()), DialogStatus.ShowNotice(makeNotice())),
            DialogTestCase(true, Result.failure(Exception()), DialogStatus.ForceUpdateRequired),
            DialogTestCase(true, Result.success(null), DialogStatus.ForceUpdateRequired),
            DialogTestCase(true, Result.success(makeNotice()), DialogStatus.ForceUpdateRequired),
        )

        fun makeNotice(): Notice {
            return Notice(
                id = "id",
                title = "title",
                message = "message",
                startAt = 0, endAt = 0
            )
        }
    }
}