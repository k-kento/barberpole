package com.gastornisapp.barberpole.ui.barberpole

import android.content.Context
import android.opengl.GLES30
import android.opengl.GLSurfaceView
import android.opengl.Matrix
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10
import kotlin.math.abs

class BarberPoleRenderer(val context: Context) : GLSurfaceView.Renderer {

    private lateinit var barberPoleModel: BarberPoleModel
    private val barberPoleShader = BarberPoleShader()
    private val modelMatrix = FloatArray(16)
    var orientation = true
    private val firstColor = FloatArray(4)
    private val secondColor = FloatArray(4)
    private var positionY = 0.0f // オブジェクトのy座標
    private var lastFrameTime = 0L // 前回のフレーム時間
    var speed = 0f
    var isPlaying = false

    override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
        GLES30.glClearColor(0f, 2f, 0f, 1.0f)
        barberPoleShader.initialize(context = context)
        barberPoleModel = BarberPoleModel(barberPoleShader).apply {
            updateColorsBuffer(firstColor, secondColor)
        }
        lastFrameTime = System.currentTimeMillis()
    }

    override fun onDrawFrame(gl: GL10?) {
        barberPoleShader.useProgram()
        val currentFrameTime = System.currentTimeMillis()
        if (isPlaying) {
            val deltaFrameTime = currentFrameTime - lastFrameTime
            positionY += (if (orientation) 1 else -1) * speed * deltaFrameTime
            if (2.0f < abs(positionY)) {
                positionY = 0f
            }
        }
        lastFrameTime = currentFrameTime

        GLES30.glClear(GLES30.GL_COLOR_BUFFER_BIT)

        Matrix.setIdentityM(modelMatrix, 0)
        Matrix.translateM(modelMatrix, 0, modelMatrix, 0, 0f, positionY, 0.0f)

        GLES30.glUniformMatrix4fv(barberPoleShader.modelLocation, 1, false, modelMatrix, 0)
        barberPoleModel.draw()
    }

    override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
        GLES30.glViewport(0, 0, width, height)
    }

    fun setColors(firstColor: FloatArray, secondColor: FloatArray) {
        firstColor.copyInto(this.firstColor)
        secondColor.copyInto(this.secondColor)
        barberPoleModel.updateColorsBuffer(firstColor, secondColor)
    }

    fun release() {
        barberPoleShader.release()
        barberPoleModel.release()
    }
}
