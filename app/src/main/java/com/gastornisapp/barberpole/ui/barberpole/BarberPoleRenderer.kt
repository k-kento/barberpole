package com.gastornisapp.barberpole.ui.barberpole

import android.content.Context
import android.opengl.GLES30
import android.opengl.GLSurfaceView
import android.opengl.Matrix
import com.gastornisapp.barberpole.ui.utils.colorToFloatArray
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10
import kotlin.math.abs

class BarberPoleRenderer(val context: Context) : GLSurfaceView.Renderer {

    private lateinit var barberPoleMesh: BarberPoleMesh
    private val barberPoleShader = BarberPoleShader()

    private val modelMatrix = FloatArray(16)

    var orientation = Orientation.Left
    var speed = 0f
    var isPlaying = false

    private val firstColor = floatArrayOf(0f, 0f, 0f, 0f)
    private val secondColor = floatArrayOf(0f, 0f, 0f, 0f)
    private var colorUpdated = false

    private var positionY = 0.0f // オブジェクトのy座標
    private var lastFrameTime = 0L // 前回のフレーム時間

    override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
        GLES30.glClearColor(0f, 0.5f, 0f, 1.0f)
        barberPoleShader.initialize(context = context)
        barberPoleMesh = BarberPoleMesh(positionLocation = barberPoleShader.positionLocation, colorLocation = barberPoleShader.colorLocation)
        lastFrameTime = System.currentTimeMillis()
    }

    override fun onDrawFrame(gl: GL10?) {
        GLES30.glClear(GLES30.GL_COLOR_BUFFER_BIT)

        barberPoleShader.useProgram()

        val currentFrameTime = System.currentTimeMillis()
        if (isPlaying) {
            val deltaFrameTime = currentFrameTime - lastFrameTime
            positionY += (if (orientation == Orientation.Right) 1 else -1) * speed * deltaFrameTime
            if (2.0f < abs(positionY)) {
                positionY = 0f
            }
        }
        lastFrameTime = currentFrameTime

        GLES30.glClear(GLES30.GL_COLOR_BUFFER_BIT)

        Matrix.setIdentityM(modelMatrix, 0)
        Matrix.translateM(modelMatrix, 0, modelMatrix, 0, 0f, positionY, 0.0f)

        barberPoleShader.setMvpModel(modelMatrix)
        if (colorUpdated) {
            barberPoleMesh.updateColorsBuffer(firstColor = firstColor, secondColor = secondColor)
            colorUpdated = false
        }
        barberPoleMesh.draw()
    }

    override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
        GLES30.glViewport(0, 0, width, height)
    }

    fun setColors(firstColor: Int, secondColor: Int) {
        colorToFloatArray(firstColor).copyInto(this.firstColor)
        colorToFloatArray(secondColor).copyInto(this.secondColor)
        colorUpdated = true
    }

    fun release() {
        barberPoleShader.release()
        barberPoleMesh.release()
    }
}
