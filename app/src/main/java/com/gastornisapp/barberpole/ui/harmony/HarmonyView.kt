package com.gastornisapp.barberpole.ui.harmony

import android.content.Context
import android.opengl.GLSurfaceView
import android.view.MotionEvent

class HarmonyView(context: Context) : GLSurfaceView(context) {
    private val renderer: HarmonyRenderer

    init {
        setEGLContextClientVersion(3)
        renderer = HarmonyRenderer(context)
        setRenderer(renderer)
        renderMode = RENDERMODE_CONTINUOUSLY
    }

    override fun onTouchEvent(event: MotionEvent): Boolean {
        when (event.actionMasked) {
            MotionEvent.ACTION_DOWN, MotionEvent.ACTION_POINTER_DOWN -> {
                val index = event.actionIndex
                val pointerId = event.getPointerId(index)
                val x = event.getX(index)
                val y = event.getY(index)

                queueEvent {
                    renderer.handleTouchDown(
                        x = x, y = y,
                        width = width, height = height,
                        pointerId = pointerId
                    )
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

    override fun performClick(): Boolean {
        // 通常のクリック処理（リスナーがいれば通知される）
        super.performClick()
        // 自分で何かしたい処理があればここに書く
        return true
    }
}
