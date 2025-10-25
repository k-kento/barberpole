package com.gastornisapp.barberpole.ui.vehicle

import android.content.Context
import android.view.MotionEvent
import androidx.lifecycle.Lifecycle
import com.gastornisapp.barberpole.ui.common.LifecycleAwareGLSurfaceView
import com.gastornisapp.barberpole.ui.vehicle.model.VehicleType

class VehicleView(context: Context, lifecycle: Lifecycle) : LifecycleAwareGLSurfaceView(context, lifecycle) {

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
                        queueEvent {
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

    fun addVehicle(vehicleType: VehicleType) {
        queueEvent {
            renderer?.addVehicle(vehicleType)
        }
    }

    fun release() {
        queueEvent {
            renderer?.release()
        }
    }
}
