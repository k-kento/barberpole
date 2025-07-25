package com.gastornisapp.barberpole.ui.percussion

import android.content.Context
import android.opengl.GLES30
import android.opengl.GLSurfaceView
import android.opengl.Matrix
import android.util.Log
import com.gastornisapp.barberpole.ui.ViewBounds
import com.gastornisapp.barberpole.ui.gl.model.CircleRendererModel
import com.gastornisapp.barberpole.ui.gl.model.TexturedQuadRenderer
import com.gastornisapp.barberpole.ui.gl.shader.CircleShaderProgram
import com.gastornisapp.barberpole.ui.gl.shader.TexturedShaderProgram
import com.gastornisapp.barberpole.ui.gl.GlUtil
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class PercussionRenderer(private val context: Context) : GLSurfaceView.Renderer {

    private var startTime: Long = 0

    private val mvpMatrix = FloatArray(16)
    private val projectionMatrix = FloatArray(16)

    private lateinit var circleRendererModel: CircleRendererModel
    private lateinit var texturedQuadRenderer: TexturedQuadRenderer

    private lateinit var texturedShaderProgram: TexturedShaderProgram

    private lateinit var buttonLogicModels: Set<PercussionLogicModel>

    private var textureIds: Map<PercussionType, Int> = emptyMap()

    override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {

        // rippleRendererModel = RippleRendererModel()

        GLES30.glClearColor(1.0f, 1.0f, 1.0f, 1.0f)

        // アルファブレンディングを有効にする
        GLES30.glEnable(GLES30.GL_BLEND)
        // 出力色 = ソース色 × srcFactor + 背景色 × dstFactor
        GLES30.glBlendFunc(GLES30.GL_SRC_ALPHA, GLES30.GL_ONE_MINUS_SRC_ALPHA)

        val circleShaderProgram = CircleShaderProgram()
        circleShaderProgram.initialize(context)
        circleShaderProgram.useProgram()

        circleRendererModel = CircleRendererModel()
        circleRendererModel.initialize(circleShaderProgram)

        texturedShaderProgram = TexturedShaderProgram()
        texturedShaderProgram.initialize(context)

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
            val color = if (it.isPressed) it.pressedBackgroundColor else it.backgroundColor
            circleRendererModel.draw(mvpMatrix = mvpMatrix, color = color)

            val textureId = textureIds[it.type] ?: textureIds.values.first()
            Matrix.multiplyMM(mvpMatrix, 0, projectionMatrix, 0, it.imageModelMatrix, 0)
            texturedQuadRenderer.draw(mvpMatrix = mvpMatrix, color = it.iconColor, textureId)
        }
    }

    override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
        GLES30.glViewport(0, 0, width, height)

        val aspect = width.toFloat() / height

        val viewBounds = if (height < width) {
            ViewBounds(-aspect, aspect, -1f, 1f)
        } else {
            ViewBounds(-1f, 1f, -1f / aspect, 1f / aspect)
        }

        Matrix.orthoM(projectionMatrix, 0, viewBounds.left, viewBounds.right, viewBounds.bottom, viewBounds.top, -1f, 1f)
        buttonLogicModels = PercussionLogicModelFactory().create(viewBounds)

        val size = GlUtil.calculateModelSizePx(
            baseWidth = CircleRendererModel.SIZE.toFloat(),
            scale = buttonLogicModels.first().iconScale,
            viewBounds = viewBounds,
            screenWidthPx = width,
            screenHeightPx = height
        )

        Log.d("PercussionRenderer", "size:${size}")

        textureIds = PercussionType.entries.associateWith { type ->
            GlUtil.loadTexture(
                context = context,
                resId = type.drawableRes,
                width = size.width.toInt(),
                height = size.height.toInt()
            )
        }

        texturedQuadRenderer = TexturedQuadRenderer(program = texturedShaderProgram)
    }

    fun handleTouchDown(x: Float, y: Float, width: Int, height: Int, pointerId: Int): PercussionType? {
        // スクリーン座標(x,y)をOpenGL座標系(-1..1)に変換
        val aspect = width.toFloat() / height
        val normalizedX = (x / width.toFloat()) * 2f - 1f
        val normalizedY = -((y / height.toFloat()) * 2f - 1f)

        val glX = if (width > height) normalizedX * aspect else normalizedX
        val glY = if (width > height) normalizedY else normalizedY / aspect

        var percussionType: PercussionType? = null

        buttonLogicModels.forEach { model ->
            val radius = model.scale * CircleRendererModel.SIZE / 2
            val left = model.x - radius
            val right = model.x + radius
            val top = model.y + radius
            val bottom = model.y - radius

            val isPressed = (glX in left..right) && (glY in bottom..top)
            if (isPressed) {
                model.isPressed = true
                percussionType = model.type
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
        circleRendererModel.release()
    }
}
