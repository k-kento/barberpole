package com.gastornisapp.barberpole.ui.swarm

import android.annotation.SuppressLint
import android.content.Context
import android.opengl.GLSurfaceView
import android.view.MotionEvent
import androidx.lifecycle.Lifecycle
import com.gastornisapp.barberpole.ui.common.LifecycleAwareGLSurfaceView

class SwarmView(context: Context, lifecycle: Lifecycle) : LifecycleAwareGLSurfaceView(context, lifecycle) {
    private val renderer: SwarmRenderer
    private var activePointerId = -1

    init {
        setEGLContextClientVersion(3)
        renderer = SwarmRenderer(context)
        setRenderer(renderer)
    }

    @SuppressLint("ClickableViewAccessibility")
    override fun onTouchEvent(event: MotionEvent): Boolean {
        when(event.actionMasked) {
            MotionEvent.ACTION_DOWN -> {
                // 画面に最初に触れた指だけを追跡
                val index = event.actionIndex
                activePointerId = event.getPointerId(index)
                queueEvent {
                    renderer.handleTouchDownAndMove(
                        x = event.getX(index),
                        y = event.getY(index),
                        width = width,
                        height = height
                    )
                }
            }

            MotionEvent.ACTION_MOVE -> {
                // 追跡中の指だけ座標を取得
                val index = event.findPointerIndex(activePointerId)
                if (index != -1) {
                    queueEvent {
                        renderer.handleTouchDownAndMove(
                            x = event.getX(index),
                            y = event.getY(index),
                            width = width,
                            height = height
                        )
                    }
                }
            }

            MotionEvent.ACTION_UP, MotionEvent.ACTION_POINTER_UP -> {
                val index = event.actionIndex
                val pointerId = event.getPointerId(index)
                if (pointerId == activePointerId) {
                    // 追跡中の指が離れたらリセット
                    activePointerId = -1
                    queueEvent {
                        renderer.handleTouchUp()
                    }
                }
                // 他の指は無視
            }
        }

        return true
    }

    fun release() {
        queueEvent {
            renderer.release()
        }
    }
}
