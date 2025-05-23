package com.gastornisapp.barberpole.ui.vehicle

import android.opengl.GLES30
import com.gastornisapp.barberpole.ui.loadShader

class VehicleShaderProgram {

    var uModelMatrixLocation = -1
    var uProjectionMatrixLocation = -1
    var aPosition = -1
    var uTextureLocation: Int = -1
    var aTexCoord = -1
    var uColorLocation = -1

    fun initialize() {
        // シェーダーのコンパイルとプログラムの作成
        val vertexShader = loadShader(GLES30.GL_VERTEX_SHADER, VERTEX_SHADER_CODE)
        val fragmentShader = loadShader(GLES30.GL_FRAGMENT_SHADER, FRAGMENT_SHADER_CODE)

        val program = GLES30.glCreateProgram().also {
            GLES30.glAttachShader(it, vertexShader)
            GLES30.glAttachShader(it, fragmentShader)
            GLES30.glLinkProgram(it)
        }

        GLES30.glUseProgram(program)

        aPosition = GLES30.glGetAttribLocation(program, "a_Position")
        uTextureLocation = GLES30.glGetUniformLocation(program, "u_Texture")
        uModelMatrixLocation = GLES30.glGetUniformLocation(program, "u_ModelMatrix")
        uProjectionMatrixLocation = GLES30.glGetUniformLocation(program, "u_ProjectionMatrix")
        aTexCoord = GLES30.glGetAttribLocation(program, "a_TexCoord")
        uColorLocation = GLES30.glGetUniformLocation(program, "u_ReplaceColor")
    }

    companion object {
        private const val VERTEX_SHADER_CODE = """
            attribute vec4 a_Position;
            attribute vec2 a_TexCoord;
            varying vec2 v_TexCoord;
            uniform mat4 u_ModelMatrix;
            uniform mat4 u_ProjectionMatrix;
            void main() {
                gl_Position = u_ProjectionMatrix * u_ModelMatrix * a_Position;
                v_TexCoord = a_TexCoord;
            }
    """

        private const val FRAGMENT_SHADER_CODE = """
            precision mediump float;
            uniform sampler2D u_Texture;
            varying vec2 v_TexCoord;
            uniform vec3 u_ReplaceColor;   // 差し替える色

            void main() {
                vec4 texColor = texture2D(u_Texture, v_TexCoord);
                
                 // 固定した元の色
                vec3 fixedTargetColor = vec3(1.0, 0.0, 0.0); // R=1.0, G=0.0, B=0.0
            
                float diff = distance(texColor.rgb, fixedTargetColor);
                if (diff < 0.05) { // 許容できる誤差か
                    gl_FragColor = vec4(u_ReplaceColor, texColor.a);
                } else {
                    gl_FragColor = texColor;
                }
            }
        """
    }
}