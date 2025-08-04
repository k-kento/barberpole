plugins {
    alias(libs.plugins.android.application)
    alias(libs.plugins.kotlin.android)
    alias(libs.plugins.kotlin.compose)
    alias(libs.plugins.aboutlibraries)
    alias(libs.plugins.google.gms.google.services)
    alias(libs.plugins.kotlin.kapt)
    alias(libs.plugins.hilt.android.gradle)
}

android {
    namespace = "com.gastornisapp.barberpole"
    compileSdk = 35

    defaultConfig {
        applicationId = "com.gastornisapp.barberpole"
        minSdk = 29
        targetSdk = 35
        versionCode = 1
        versionName = "1.0.0"

        testInstrumentationRunner = "com.gastornisapp.barberpole.CustomTestRunner"
    }

    buildTypes {
        release {
            isMinifyEnabled = false
            proguardFiles(
                getDefaultProguardFile("proguard-android-optimize.txt"),
                "proguard-rules.pro"
            )
        }
    }
    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_11
        targetCompatibility = JavaVersion.VERSION_11
    }
    kotlinOptions {
        jvmTarget = "11"
    }
    buildFeatures {
        compose = true
    }
    testOptions {
        unitTests.isIncludeAndroidResources = true
    }
}

dependencies {
    implementation(libs.androidx.core.ktx)
    implementation(libs.androidx.core.splashscreen)
    implementation(libs.androidx.lifecycle.runtime.ktx)
    implementation(libs.androidx.activity.compose)
    implementation(platform(libs.androidx.compose.bom))
    implementation(libs.androidx.material.icons.extended.android)
    implementation(libs.androidx.ui)
    implementation(libs.androidx.ui.graphics)
    implementation(libs.androidx.ui.tooling.preview)
    implementation(libs.androidx.material3)
    implementation(libs.androidx.lifecycle.common.jvm)
    implementation(libs.androidx.navigation.compose)
    implementation(libs.androidx.datastore)
    implementation(libs.aboutlibraries.core)
    implementation(libs.aboutlibraries.compose)
    implementation(libs.hilt.android)
    implementation(libs.androidx.hilt.navigation.compose)
    implementation(libs.androidx.navigation.testing.android)
    kapt(libs.hilt.compiler)
    implementation(project(":audio"))
    implementation(platform(libs.firebase.bom))
    implementation(libs.firebase.config)

//    debugImplementation(libs.androidx.ui.tooling)
//    debugImplementation(libs.androidx.ui.test.manifest)

    testImplementation(libs.mockk)
    testImplementation(libs.kotlinx.coroutines.test)
    // JUnit 5 本体
    testImplementation(libs.junit.jupiter)
    // パラメータ化テストサポート
    testImplementation(libs.junit.jupiter.params)
    // Android Gradle Plugin で JUnit 5 を有効にするためのプラグイン
    testRuntimeOnly(libs.junit.platform.launcher)
    testImplementation(kotlin("test"))

    androidTestImplementation(libs.androidx.junit)
    androidTestImplementation(libs.androidx.espresso.core)
    androidTestImplementation(platform(libs.androidx.compose.bom))
    androidTestImplementation(libs.androidx.ui.test.junit4)
    androidTestImplementation(libs.androidx.navigation.testing)
    androidTestImplementation(libs.hilt.android.testing)
    androidTestImplementation(libs.ui.test.junit4)
    kaptAndroidTest(libs.hilt.android.compiler)
    androidTestImplementation(libs.mockk.android)
}


tasks.withType<Test> {
    useJUnitPlatform()
}