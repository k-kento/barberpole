package com.gastornisapp.barberpole.ui.vehicle

import android.content.Context
import android.opengl.GLES30
import android.opengl.GLSurfaceView
import android.opengl.Matrix
import com.gastornisapp.barberpole.R
import com.gastornisapp.barberpole.ui.ViewBounds
import com.gastornisapp.barberpole.ui.gl.GlUtil
import com.gastornisapp.barberpole.ui.gl.mesh.SquareMesh
import com.gastornisapp.barberpole.ui.gl.shader.TexturedShaderProgram
import com.gastornisapp.barberpole.ui.gl.shader.TexturedTintShaderProgram
import com.gastornisapp.barberpole.ui.vehicle.logic.BusLogicModel
import com.gastornisapp.barberpole.ui.vehicle.logic.CarLogicModel
import com.gastornisapp.barberpole.ui.vehicle.logic.LightTruckLogicModel
import com.gastornisapp.barberpole.ui.vehicle.logic.VehicleLogicModel
import com.gastornisapp.barberpole.ui.vehicle.logic.VehicleManager
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10
import kotlin.reflect.KClass

class VehicleRenderer(private val context: Context) : GLSurfaceView.Renderer {

    private lateinit var vehicleManager: VehicleManager

    private lateinit var viewBounds: ViewBounds
    private val modelMatrix = FloatArray(16)
    private val projectionMatrix = FloatArray(16)

    private lateinit var mainProgram: TexturedShaderProgram
    private lateinit var bodyProgram: TexturedTintShaderProgram
    private lateinit var vehicleMainMesh: SquareMesh
    private lateinit var vehicleBodyMesh: SquareMesh

    private var textures: Map<KClass<out VehicleLogicModel>, VehicleTextureSet> = emptyMap()
    private val vehicleResources = mapOf(
        CarLogicModel::class to VehicleResourceSet(R.drawable.car_main, R.drawable.car_body),
        BusLogicModel::class to VehicleResourceSet(R.drawable.bus_main, R.drawable.bus_body),
        LightTruckLogicModel::class to VehicleResourceSet(R.drawable.light_truck_main, R.drawable.light_truck_body)
    )

    // 前回のフレーム時間
    private var lastFrameTime: Long = 0L

    override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {

        GLES30.glClearColor(1f, 1f, 1f, 1.0f)

        // アルファブレンディングを有効にする
        GLES30.glEnable(GLES30.GL_BLEND)
        // 出力色 = ソース色 × srcFactor + 背景色 × dstFactor
        GLES30.glBlendFunc(GLES30.GL_SRC_ALPHA, GLES30.GL_ONE_MINUS_SRC_ALPHA)

        bodyProgram = TexturedTintShaderProgram().apply {
            initialize(context)
        }
        mainProgram = TexturedShaderProgram().apply {
            initialize(context)
        }

        vehicleMainMesh = SquareMesh(mainProgram.aPosition, mainProgram.aTexCoord)
        vehicleBodyMesh = SquareMesh(bodyProgram.aPosition, bodyProgram.aTexCoord)

        textures = loadVehicleTextures(context, vehicleResources)

        textures.flatMap { listOf(it.value.main, it.value.body) }
            .forEach { texture ->
                GLES30.glActiveTexture(GLES30.GL_TEXTURE0 + texture.textureUnit)
                GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, texture.textureId)
            }

        vehicleManager = VehicleManager()
        lastFrameTime = System.currentTimeMillis()
    }

    override fun onDrawFrame(gl: GL10?) {
        GLES30.glClear(GLES30.GL_COLOR_BUFFER_BIT)

        val currentTime = System.currentTimeMillis()
        val deltaFrameTime = currentTime - lastFrameTime
        lastFrameTime = currentTime

        vehicleManager.update(deltaTime = deltaFrameTime)

        bodyProgram.useProgram()

        for (vehicle in vehicleManager.activeVehicles) {
            calculateMvpMatrix(vehicle)

            bodyProgram.apply {
                setColor(vehicle.color)
                setMvpMatrix(vehicle.mvpMatrix)
                setTextureUnit(textures[vehicle::class]!!.body.textureUnit)
            }

            vehicleBodyMesh.draw()
        }

        mainProgram.useProgram()

        for (vehicle in vehicleManager.activeVehicles) {
            mainProgram.apply {
                setMvpMatrix(vehicle.mvpMatrix)
                setTextureUnit(textures[vehicle::class]!!.main.textureUnit)
            }
            vehicleMainMesh.draw()
        }

        lastFrameTime = currentTime
    }

    override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
        GLES30.glViewport(0, 0, width, height)

        val aspect = width.toFloat() / height

        viewBounds = if (width > height) {
            ViewBounds(-aspect, aspect, -1.0f, 1.0f)
        } else {
            ViewBounds(-1.0f, 1.0f, -1f / aspect, 1f / aspect)
        }

        vehicleManager.viewBounds = viewBounds

        Matrix.orthoM(
            projectionMatrix,
            0,
            viewBounds.left, viewBounds.right,
            viewBounds.bottom, viewBounds.top,
            -1f, 1f
        )
    }

    fun handleTouchDown(screenX: Float, screenY: Float, screenWidth: Int, screenHeight: Int): Boolean {
        // モデル座標系（-1〜1）に変換
        val touchX = (screenX / screenWidth) * viewBounds.width - viewBounds.right
        val touchY = -((screenY / screenHeight) * viewBounds.height - viewBounds.top)
        return vehicleManager.handleTouchDown(touchX = touchX, touchY = touchY)
    }

    fun handleTouchUp() {
        vehicleManager.handleTouchUp()
    }

    fun release() {
        textures.flatMap { listOf(it.value.main, it.value.body) }
            .forEach { texture ->
                GLES30.glDeleteTextures(1, intArrayOf(texture.textureId), 0)
            }
        mainProgram.release()
        vehicleBodyMesh.release()
    }

    private fun calculateMvpMatrix(vehicle: VehicleLogicModel) {
        Matrix.setIdentityM(modelMatrix, 0)
        Matrix.translateM(modelMatrix, 0, vehicle.posX, vehicle.posY, 0f)
        Matrix.scaleM(modelMatrix, 0, vehicle.orientation.value * vehicle.scale, vehicle.scale, 1f)
        Matrix.multiplyMM(vehicle.mvpMatrix, 0, projectionMatrix, 0, modelMatrix, 0)
    }

    private fun loadVehicleTextures(
        context: Context,
        vehicleResources: Map<KClass<out VehicleLogicModel>, VehicleResourceSet>
    ): Map<KClass<out VehicleLogicModel>, VehicleTextureSet> {

        var textureUnitCounter = 0

        return vehicleResources.mapValues { (_, resource) ->
            val mainTexture = Texture(
                textureId = GlUtil.loadTexture(context, resource.main),
                textureUnit = textureUnitCounter++
            )
            val bodyTexture = Texture(
                textureId = GlUtil.loadTexture(context, resource.body),
                textureUnit = textureUnitCounter++
            )
            VehicleTextureSet(main = mainTexture, body = bodyTexture)
        }
    }
}

private data class VehicleResourceSet(val main: Int, val body: Int)
private data class VehicleTextureSet(val main: Texture, val body: Texture)
private data class Texture(val textureId: Int, val textureUnit: Int)
