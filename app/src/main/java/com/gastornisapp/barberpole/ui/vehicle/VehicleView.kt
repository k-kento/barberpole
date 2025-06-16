package com.gastornisapp.barberpole.ui.vehicle

import android.content.Context
import android.opengl.GLSurfaceView
import android.view.MotionEvent

class VehicleView(context: Context) : GLSurfaceView(context) {

    private var renderer: VehicleRenderer?

    var onVehicleTouchStart: (() -> Unit)? = null

    init {
        setEGLContextClientVersion(3)
        renderer = VehicleRenderer(this.context.applicationContext)
        setRenderer(renderer)
    }

    override fun onTouchEvent(event: MotionEvent): Boolean {
        val x = event.x
        val y = event.y

        when (event.action) {
            MotionEvent.ACTION_UP -> {
                queueEvent {
                    renderer?.handleTouchUp()
                }
                performClick()
                return true
            }
            MotionEvent.ACTION_DOWN -> {
                queueEvent {
                    val touch = renderer?.handleTouchDown(x, y, width, height)
                    if (touch == true) {
                        post {
                            onVehicleTouchStart?.invoke()
                        }
                    }
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
