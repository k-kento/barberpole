package com.gastornisapp.barberpole.ui.percussion

import android.content.Context
import android.opengl.GLES30
import android.opengl.GLSurfaceView
import android.opengl.Matrix
import com.gastornisapp.barberpole.ui.ViewBounds
import com.gastornisapp.barberpole.ui.gl.model.CircleRendererModel
import com.gastornisapp.barberpole.ui.gl.shader.CircleShaderProgram
import com.gastornisapp.soundlib.AudioResource
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class PercussionRenderer(private val context: Context) : GLSurfaceView.Renderer {

    private var startTime: Long = 0

    private val mvpMatrix = FloatArray(16)
    private val projectionMatrix = FloatArray(16)

    private lateinit var circleRendererModel: CircleRendererModel

    private lateinit var buttonLogicModels: Set<PercussionLogicModel>

    override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
        circleRendererModel = CircleRendererModel()
        // rippleRendererModel = RippleRendererModel()

        GLES30.glClearColor(1.0f, 1.0f, 1.0f, 1.0f)

        // アルファブレンディングを有効にする
        GLES30.glEnable(GLES30.GL_BLEND)
        // 出力色 = ソース色 × srcFactor + 背景色 × dstFactor
        GLES30.glBlendFunc(GLES30.GL_SRC_ALPHA, GLES30.GL_ONE_MINUS_SRC_ALPHA)

        val circleShaderProgram = CircleShaderProgram()
        circleShaderProgram.initialize(context)
        circleShaderProgram.useProgram()

        circleRendererModel.initialize(circleShaderProgram)

//        val rippleShaderProgram = RippleShaderProgram()
//        rippleShaderProgram.initialize(context)
//        rippleRendererModel.initialize(rippleShaderProgram)

        startTime = System.currentTimeMillis()
    }

    override fun onDrawFrame(gl: GL10?) {
        GLES30.glClear(GLES30.GL_COLOR_BUFFER_BIT)

        buttonLogicModels.forEach {
            if (it.isPressed) {
                Matrix.multiplyMM(mvpMatrix, 0, projectionMatrix, 0, it.effectModelMatrix, 0)
                // rippleRendererModel.draw(mvpMatrix = mvpMatrix, time = elapsed)
            }
            Matrix.multiplyMM(mvpMatrix, 0, projectionMatrix, 0, it.modelMatrix, 0)

            val color = if (it.isPressed) it.pressedColor else it.color
            circleRendererModel.draw(mvpMatrix = mvpMatrix, color = color)
        }
    }

    override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
        GLES30.glViewport(0, 0, width, height)

        val aspect = width.toFloat() / height

        val viewBounds: ViewBounds

        if (height < width) {
            viewBounds = ViewBounds(-aspect, aspect, -1f, 1f)
        } else {
            viewBounds = ViewBounds(-1f, 1f, -1f / aspect, 1f / aspect)
        }

        Matrix.orthoM(projectionMatrix, 0, viewBounds.left, viewBounds.right, viewBounds.bottom, viewBounds.top, -1f, 1f)
        buttonLogicModels = PercussionLogicModel.buildButtonLogicModels(viewBounds)
        buttonLogicModels.forEach {
            it.updateModelMatrix()
        }
    }

    fun handleTouchDown(x: Float, y: Float, width: Int, height: Int, pointerId: Int): AudioResource? {
        // スクリーン座標(x,y)をOpenGL座標系(-1..1)に変換
        val aspect = width.toFloat() / height
        val normalizedX = (x / width.toFloat()) * 2f - 1f
        val normalizedY = -((y / height.toFloat()) * 2f - 1f)

        val glX = if (width > height) normalizedX * aspect else normalizedX
        val glY = if (width > height) normalizedY else normalizedY / aspect

        var percussionType: AudioResource? = null

        buttonLogicModels.forEach {
            val radius = it.scale * CircleRendererModel.SIZE / 2
            val left = it.x - radius
            val right = it.x + radius
            val top = it.y + radius
            val bottom = it.y - radius

            val isPressed = (glX in left..right) && (glY in bottom..top)
            if (isPressed) {
                it.isPressed = true
                percussionType = it.type
                return@forEach
            }
        }

        return percussionType
    }

    fun handleTouchUp(pointerId: Int) {
        buttonLogicModels.forEach {
            it.isPressed = false
        }
    }

    fun release() {

    }
}
