package com.gastornisapp.barberpole.barberpole

import android.opengl.GLES20
import android.opengl.GLSurfaceView
import android.opengl.Matrix
import com.gastornisapp.barberpole.ShaderProgram
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10
import kotlin.math.abs


class BarberPoleRenderer : GLSurfaceView.Renderer {

    private var barberPoleModel: BarberPoleModel? = null
    private val shaderProgram = ShaderProgram()
    private val modelMatrix = FloatArray(16)
    private var orientation = true

    override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
        GLES20.glClearColor(0f, 0f, 0f, 1.0f)
        shaderProgram.createProgram()
        shaderProgram.useProgram()
        barberPoleModel = BarberPoleModel(shaderProgram)
    }

    private var positionY = 0.0f // オブジェクトのx座標
    var speed = 0f // 移動速度 (単位: 単位座標/ミリ秒)
    private var lastTime = System.currentTimeMillis() // 前回のフレーム時間

    override fun onDrawFrame(gl: GL10?) {
        val currentTime = System.currentTimeMillis()

        val deltaTime = currentTime - lastTime
        lastTime = currentTime

        positionY += (if (orientation) 1 else -1) * speed * deltaTime

        if (2f < abs(positionY)) {
            positionY = 0f
        }

        GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT)

        Matrix.setIdentityM(modelMatrix, 0)
        Matrix.translateM(modelMatrix, 0, modelMatrix, 0, 0f, positionY, 0.0f)

        val uViewMatrix = GLES20.glGetUniformLocation(shaderProgram.program!!, "uModelViewMatrix");
        GLES20.glUniformMatrix4fv(uViewMatrix, 1, false, modelMatrix, 0)
        barberPoleModel?.draw()
    }

    override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
        GLES20.glViewport(0, 0, width, height)
    }

    fun setColors(colorOne: FloatArray, colorTwo: FloatArray) {
        if (barberPoleModel != null) {
            barberPoleModel?.update(colorOne, colorTwo)
        }
    }

    fun setOrientation(orientation: Boolean) {
        this.orientation = orientation
    }
}
