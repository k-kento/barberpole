package com.gastornisapp.barberpole.ui.utils

import android.content.Context
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.graphics.Canvas
import android.graphics.drawable.VectorDrawable
import android.opengl.GLES30
import android.opengl.GLUtils
import android.util.Log
import android.util.SizeF
import androidx.annotation.DrawableRes
import androidx.core.content.ContextCompat
import androidx.core.graphics.createBitmap
import com.gastornisapp.barberpole.ui.ViewBounds
import androidx.core.graphics.scale

private const val TAG = "GlUtil"

fun loadVertexShader(shaderCode: String): Int {
    return loadShader(GLES30.GL_VERTEX_SHADER, shaderCode)
}

fun loadFragmentShader(shaderCode: String): Int {
    return loadShader(GLES30.GL_FRAGMENT_SHADER, shaderCode)
}


// シェーダーをロードしてコンパイル
fun loadShader(type: Int, shaderCode: String): Int {
    val shader = GLES30.glCreateShader(type)
    if (shader == 0) {
        throw RuntimeException("Failed to create shader of type: $type")
    }

    // 2. シェーダーのソースコードを設定してコンパイル
    GLES30.glShaderSource(shader, shaderCode)
    GLES30.glCompileShader(shader)


    // 3. コンパイル結果を取得
    val compileStatus = IntArray(1)
    GLES30.glGetShaderiv(shader, GLES30.GL_COMPILE_STATUS, compileStatus, 0)

    if (compileStatus[0] == 0) {
        val shaderTypeStr = when (type) {
            GLES30.GL_VERTEX_SHADER -> "VERTEX"
            GLES30.GL_FRAGMENT_SHADER -> "FRAGMENT"
            else -> "UNKNOWN"
        }
        Log.e(TAG, "[$shaderTypeStr SHADER ERROR]")
        Log.e(TAG, GLES30.glGetShaderInfoLog(shader))

        // メモリリーク防止のためシェーダーを削除
        GLES30.glDeleteShader(shader)
        throw RuntimeException("$shaderTypeStr shader compilation failed")
    }

    return shader
}

object GlUtil {

    /**
     * リソースからテクスチャを生成
     * width・height指定があれば、そのサイズに合わせて読み込む（ラスター・ベクター共通）
     * 指定なければ元サイズで読み込む
     */
    fun loadTexture(
        context: Context,
        @DrawableRes resId: Int,
        width: Int? = null,
        height: Int? = null
    ): Int {
        val textureIds = IntArray(1)
        GLES30.glGenTextures(1, textureIds, 0)
        if (textureIds[0] == 0) {
            throw RuntimeException("Failed to generate texture ID")
        }

        GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, textureIds[0])

        // テクスチャパラメータ設定
        GLES30.glTexParameteri(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_MIN_FILTER, GLES30.GL_LINEAR)
        GLES30.glTexParameteri(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_MAG_FILTER, GLES30.GL_LINEAR)
        GLES30.glTexParameteri(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_WRAP_S, GLES30.GL_CLAMP_TO_EDGE)
        GLES30.glTexParameteri(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_WRAP_T, GLES30.GL_CLAMP_TO_EDGE)

        val bitmap: Bitmap = when (val drawable = ContextCompat.getDrawable(context, resId)) {
            is VectorDrawable -> {
                // VectorDrawableはwidth, height 必須
                val w = width ?: throw IllegalArgumentException("Width required for VectorDrawable")
                val h = height ?: throw IllegalArgumentException("Height required for VectorDrawable")
                val bmp = createBitmap(w, h)
                val canvas = Canvas(bmp)
                drawable.setBounds(0, 0, w, h)
                drawable.draw(canvas)
                bmp
            }
            else -> {
                // ラスター画像はwidth,height指定があればリサイズ読み込み
                if (width != null && height != null) {
                    // BitmapFactoryで縮小読み込み
                    val options = BitmapFactory.Options().apply {
                        inJustDecodeBounds = true
                    }
                    BitmapFactory.decodeResource(context.resources, resId, options)

                    options.inSampleSize = calculateInSampleSize(options, width, height)
                    options.inJustDecodeBounds = false

                    val decoded = BitmapFactory.decodeResource(context.resources, resId, options)
                        ?: throw IllegalArgumentException("Bitmap decode failed")

                    decoded.scale(width, height).also {
                        if (decoded != it) decoded.recycle()
                    }
                } else {
                    // サイズ指定なしは元サイズ読み込み
                    BitmapFactory.decodeResource(context.resources, resId)
                        ?: throw IllegalArgumentException("Bitmap decode failed")
                }
            }
        }

        GLUtils.texImage2D(GLES30.GL_TEXTURE_2D, 0, bitmap, 0)
        bitmap.recycle()

        GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, 0)

        return textureIds[0]
    }

    // BitmapFactoryで効率的に縮小読み込みするための計算
    private fun calculateInSampleSize(
        options: BitmapFactory.Options,
        reqWidth: Int,
        reqHeight: Int
    ): Int {
        val (height: Int, width: Int) = options.run { outHeight to outWidth }
        var inSampleSize = 1

        if (height > reqHeight || width > reqWidth) {
            val halfHeight = height / 2
            val halfWidth = width / 2

            while (halfHeight / inSampleSize >= reqHeight && halfWidth / inSampleSize >= reqWidth) {
                inSampleSize *= 2
            }
        }

        return inSampleSize
    }

    fun calculateModelSizePx(
        baseWidth: Float,           // モデルの元の幅（例: 2f）
        baseHeight: Float = baseWidth, // モデルの元の高さ（指定なければ正方形）
        scale: Float,               // モデルスケール
        viewBounds: ViewBounds,     // OpenGL上のビュー領域
        screenWidthPx: Int,         // 実画面ピクセル横幅
        screenHeightPx: Int         // 実画面ピクセル縦幅
    ): SizeF {
        val glWidth = viewBounds.width
        val glHeight = viewBounds.height

        val pxPerGlUnitX = screenWidthPx / glWidth
        val pxPerGlUnitY = screenHeightPx / glHeight

        val widthPx = baseWidth * scale * pxPerGlUnitX
        val heightPx = baseHeight * scale * pxPerGlUnitY

        return SizeF(widthPx, heightPx)
    }
}
