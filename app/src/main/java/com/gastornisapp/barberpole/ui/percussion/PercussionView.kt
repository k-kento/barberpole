package com.gastornisapp.barberpole.ui.percussion

import android.content.Context
import android.opengl.GLSurfaceView
import android.view.MotionEvent
import android.view.SurfaceHolder
import com.gastornisapp.soundlib.AudioResource

class PercussionView(context: Context) : GLSurfaceView(context) {
    private val renderer: PercussionRenderer

    var onClicked: ((AudioResource) -> Unit)? = null

    init {
        setEGLContextClientVersion(3)
        renderer = PercussionRenderer(context)
        setRenderer(renderer)
    }

    override fun onTouchEvent(event: MotionEvent): Boolean {
        when (event.actionMasked) {
            MotionEvent.ACTION_DOWN, MotionEvent.ACTION_POINTER_DOWN -> {
                val index = event.actionIndex
                val pointerId = event.getPointerId(index)
                val x = event.getX(index)
                val y = event.getY(index)

                queueEvent {
                    val percussionType = renderer.handleTouchDown(
                        x = x, y = y,
                        width = width, height = height,
                        pointerId = pointerId
                    )
                    if (percussionType != null) {
                        post {
                            onClicked?.invoke(percussionType)
                        }
                    }
                }
            }

            MotionEvent.ACTION_POINTER_UP, MotionEvent.ACTION_UP -> {
                val index = event.actionIndex
                val pointerId = event.getPointerId(index)

                queueEvent {
                    renderer.handleTouchUp(pointerId = pointerId)
                }
                performClick()
            }
        }

        return true
    }

    override fun surfaceDestroyed(holder: SurfaceHolder) {
        renderer.release()
        super.surfaceDestroyed(holder)
    }

    override fun performClick(): Boolean {
        // 通常のクリック処理（リスナーがいれば通知される）
        super.performClick()
        // 自分で何かしたい処理があればここに書く
        return true
    }
}
