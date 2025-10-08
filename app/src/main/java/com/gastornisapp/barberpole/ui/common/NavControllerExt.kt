package com.gastornisapp.barberpole.ui.common

import android.net.Uri
import androidx.navigation.NavController

fun NavController.navigateToWebPage(url: String) {
    val encodedUrl = Uri.encode(url)
    this.navigate("webpage?url=$encodedUrl")
}