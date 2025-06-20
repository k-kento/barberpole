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
        val x = event.x
        val y = event.y

        when (event.action) {
            MotionEvent.ACTION_UP -> {
                queueEvent {
                    renderer.handleTouchUp()
                }
                performClick()
                return true
            }
            MotionEvent.ACTION_DOWN -> {
                queueEvent {
                    renderer.handleTouchDown(x, y, width, height)
                }
                return true
            }
            else -> return super.onTouchEvent(event)
        }
    }

    override fun performClick(): Boolean {
        // 通常のクリック処理（リスナーがいれば通知される）
        super.performClick()
        // 自分で何かしたい処理があればここに書く
        return true
    }
}
