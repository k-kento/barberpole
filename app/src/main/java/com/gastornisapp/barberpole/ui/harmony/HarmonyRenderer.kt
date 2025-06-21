package com.gastornisapp.barberpole.ui.harmony

import android.content.Context
import android.opengl.GLES30
import android.opengl.GLSurfaceView
import android.opengl.Matrix
import com.gastornisapp.barberpole.ui.gl.model.RippleShaderProgram
import com.gastornisapp.barberpole.ui.loadFragmentShader
import com.gastornisapp.barberpole.ui.loadVertexShader
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class HarmonyRenderer(private val context: Context) : GLSurfaceView.Renderer {

    private var startTime: Long = 0

    private val mvpMatrix = FloatArray(16)
    private val projectionMatrix = FloatArray(16)

    private lateinit var buttonRenderModel: ButtonRendererModel
    private lateinit var rippleRendererModel: RippleRendererModel

    // どのボタンが押されているか
    private val activePointers = mutableMapOf<Int, ButtonLogicModel>()


    private val buttonLogicModels = (0 until 5).map { i ->
        val angle = Math.toRadians((90 + i * 72).toDouble()) // 90度スタートで上に1つ
        val r = 0.6f  // 半径
        ButtonLogicModel(
            x = (r * kotlin.math.cos(angle)).toFloat(),
            y = (r * kotlin.math.sin(angle)).toFloat()
        )
    }.toSet()

    override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {

        buttonRenderModel = ButtonRendererModel()
        rippleRendererModel = RippleRendererModel()

        GLES30.glClearColor(1.0f, 1.0f, 1.0f, 1.0f)

        // アルファブレンディングを有効にする
        GLES30.glEnable(GLES30.GL_BLEND)
        // 出力色 = ソース色 × srcFactor + 背景色 × dstFactor
        GLES30.glBlendFunc(GLES30.GL_SRC_ALPHA, GLES30.GL_ONE_MINUS_SRC_ALPHA)

        val asset = context.assets
        val vertexShaderSrc = asset.open("shaders/button.vsh").bufferedReader(Charsets.UTF_8).use { it.readText() }
        val vertexShader = loadVertexShader(vertexShaderSrc)

        val fragmentShaderSrc = asset.open("shaders/button.fsh").bufferedReader(Charsets.UTF_8).use { it.readText() }
        val fragmentShader = loadFragmentShader(fragmentShaderSrc)

        val buttonShaderProgram = GLES30.glCreateProgram().also {
            GLES30.glAttachShader(it, vertexShader)
            GLES30.glAttachShader(it, fragmentShader)
            GLES30.glLinkProgram(it)
        }

        GLES30.glUseProgram(buttonShaderProgram)

        buttonRenderModel.initialize(buttonShaderProgram)

        val rippleShaderProgram = RippleShaderProgram()
        rippleShaderProgram.initialize(context)
        rippleRendererModel.initialize(rippleShaderProgram)

        buttonLogicModels.forEach {
            it.updateModelMatrix()
        }

        startTime = System.currentTimeMillis()
    }

    override fun onDrawFrame(gl: GL10?) {
        GLES30.glClear(GLES30.GL_COLOR_BUFFER_BIT)

        val elapsed = (System.currentTimeMillis() - startTime) / 1000f

        buttonLogicModels.forEach {
            if (it.isPressed) {
                Matrix.multiplyMM(mvpMatrix, 0, projectionMatrix, 0, it.effectModelMatrix, 0)
                rippleRendererModel.draw(mvpMatrix = mvpMatrix, time = elapsed)
            }
            Matrix.multiplyMM(mvpMatrix, 0, projectionMatrix, 0, it.modelMatrix, 0)
            buttonRenderModel.draw(mvpMatrix = mvpMatrix, it.isPressed)
        }
    }

    override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
        GLES30.glViewport(0, 0, width, height)

        val aspect = width.toFloat() / height

        if (height < width) {
            Matrix.orthoM(projectionMatrix, 0, -aspect, aspect, -1f, 1f, -1f, 1f)
        } else {
            Matrix.orthoM(projectionMatrix, 0, -1f, 1f, -1f / aspect, 1f / aspect, -1f, 1f)
        }
    }

    fun handleTouchDown(x: Float, y: Float, width: Int, height: Int, pointerId: Int) {
        // スクリーン座標(x,y)をOpenGL座標系(-1..1)に変換
        val aspect = width.toFloat() / height
        val normalizedX = (x / width.toFloat()) * 2f - 1f
        val normalizedY = -((y / height.toFloat()) * 2f - 1f)

        val glX = if (width > height) normalizedX * aspect else normalizedX
        val glY = if (width > height) normalizedY else normalizedY / aspect

        buttonLogicModels.forEach {
            val radius = ButtonLogicModel.SCALE * ButtonRendererModel.SIZE / 2
            val left = it.x - radius
            val right = it.x + radius
            val top = it.y + radius
            val bottom = it.y - radius

            val isPressed = (glX in left..right) && (glY in bottom..top)
            if (isPressed) {
                activePointers[pointerId] = it
                it.isPressed = true
            }
        }
    }

    fun handleTouchUp(pointerId: Int) {
        val button = activePointers[pointerId]
        button?.isPressed = false
        activePointers.remove(pointerId)
    }
}
