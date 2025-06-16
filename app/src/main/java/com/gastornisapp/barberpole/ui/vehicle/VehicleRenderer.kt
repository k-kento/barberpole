package com.gastornisapp.barberpole.ui.vehicle

import android.content.Context
import android.opengl.GLES30
import android.opengl.GLSurfaceView
import android.opengl.Matrix
import com.gastornisapp.barberpole.R
import com.gastornisapp.barberpole.ui.ScreenInfo
import com.gastornisapp.barberpole.ui.loadTexture
import com.gastornisapp.barberpole.ui.vehicle.logic.CarLogicModel
import com.gastornisapp.barberpole.ui.vehicle.logic.BusLogicModel
import com.gastornisapp.barberpole.ui.vehicle.logic.VehicleLogicModel
import com.gastornisapp.barberpole.ui.vehicle.logic.VehicleManager
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10
import kotlin.reflect.KClass

class VehicleRenderer(private val context: Context) : GLSurfaceView.Renderer {

    private lateinit var vehicleManager: VehicleManager

    private lateinit var screenInfo: ScreenInfo
    private val projectionMatrix = FloatArray(16)

    private var lastFrameTime: Long = 0L // 前回のフレーム時間

    private lateinit var program: VehicleShaderProgram

    private var rendererModels = mutableMapOf<KClass<out VehicleLogicModel>, VehicleRendererModel>()

    override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {

        GLES30.glClearColor(1f, 1f, 1f, 1.0f)

        // アルファブレンディングを有効にする
        GLES30.glEnable(GLES30.GL_BLEND)
        // 出力色 = ソース色 × srcFactor + 背景色 × dstFactor
        GLES30.glBlendFunc(GLES30.GL_SRC_ALPHA, GLES30.GL_ONE_MINUS_SRC_ALPHA)

        // シェーダーをコンパイルし、使用する
        program = VehicleShaderProgram()
        program.initialize()

        val carTextureId = loadTexture(context = context, R.drawable.car)
        val busTextureId = loadTexture(context = context, R.drawable.bus)

        rendererModels[CarLogicModel::class] = VehicleRendererModel(program = program, textureId = carTextureId)
        rendererModels[BusLogicModel::class] = VehicleRendererModel(program = program, textureId = busTextureId)

        vehicleManager = VehicleManager()

        lastFrameTime = System.currentTimeMillis()
    }

    override fun onDrawFrame(gl: GL10?) {
        GLES30.glClear(GLES30.GL_COLOR_BUFFER_BIT)

        val currentTime = System.currentTimeMillis()

        // 初回 onDrawFrame はスキップ
        if (lastFrameTime == 0L) {
            lastFrameTime = currentTime
            return
        }

        val deltaFrameTime = currentTime - lastFrameTime
        lastFrameTime = currentTime

        vehicleManager.update(deltaTime = deltaFrameTime, screenInfo = screenInfo)

        for (vehicle in vehicleManager.activeVehicles()) {
            rendererModels[vehicle::class]?.apply {
                updateModelMatrix(
                    posX = vehicle.posX,
                    posY = vehicle.posY,
                    orientation = vehicle.orientation.value,
                    scale = vehicle.scale,
                )
                draw(color = vehicle.color)
            }
        }

        lastFrameTime = currentTime
    }

    override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
        GLES30.glViewport(0, 0, width, height)

        val aspect = width.toFloat() / height

        screenInfo = if (width > height) {
            // 横画面
            ScreenInfo(-aspect, aspect, -1.0f, 1.0f)
        } else {
            // 縦画面
            ScreenInfo(-1.0f, 1.0f, -1f / aspect, 1f / aspect)
        }

        vehicleManager.screenInfo = screenInfo

        Matrix.orthoM(
            projectionMatrix,
            0,
            screenInfo.left, screenInfo.right,
            screenInfo.bottom, screenInfo.top,
            -1f, 1f
        )

        GLES30.glUniformMatrix4fv(program.uProjectionMatrixLocation, 1, false, projectionMatrix, 0)
    }

    fun handleTouchDown(screenX: Float, screenY: Float, screenWidth: Int, screenHeight: Int) : Boolean {
        // モデル座標系（-1〜1）に変換
        val touchX = (screenX / screenWidth) * screenInfo.width - screenInfo.right
        val touchY = -((screenY / screenHeight) * screenInfo.height - screenInfo.top)
        return vehicleManager.handleTouchDown(touchX = touchX, touchY = touchY)
    }

    fun handleTouchUp() {
        vehicleManager.handleTouchUp()
    }
}
