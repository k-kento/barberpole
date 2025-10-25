package com.gastornisapp.barberpole.ui.confiramtion

import android.app.Activity
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.Button
import androidx.compose.material3.ExperimentalMaterial3Api
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Text
import androidx.compose.material3.TextButton
import androidx.compose.material3.TopAppBar
import androidx.compose.runtime.Composable
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.runtime.collectAsState
import androidx.compose.runtime.getValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.res.stringResource
import androidx.compose.ui.unit.dp
import androidx.hilt.navigation.compose.hiltViewModel
import androidx.navigation.NavController
import com.gastornisapp.barberpole.Constants
import com.gastornisapp.barberpole.R
import com.gastornisapp.barberpole.ui.common.navigateToWebPage

/**
 * 利用規約確認画面
 */
@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun ConfirmationPage(
    navController: NavController,
    viewModel: ConfirmationViewModel = hiltViewModel()
) {
    Scaffold(
        topBar = { TopAppBar(title = { Text(stringResource(R.string.terms_of_service)) }) },
    ) { paddingValues ->
        val context = LocalContext.current

        val isTermsAccepted by viewModel.isTermsAccepted.collectAsState()

        LaunchedEffect(isTermsAccepted) {
            if (isTermsAccepted) {
                // 利用規約画面をポップし、ホーム画面へ遷移する。
                navController.navigate("home") {
                    popUpTo("confirmation") { inclusive = true }
                }
            }
        }

        Box(
            Modifier
                .fillMaxSize()
                .padding(paddingValues)
                .padding(16.dp)
        ) {
            Column(
                modifier = Modifier
                    .fillMaxSize()
            ) {
                Text(
                    text = stringResource(R.string.confirmation_page_description),
                )
                Spacer(modifier = Modifier.height(16.dp))
                TextButton(onClick = {
                    navController.navigateToWebPage(Constants.termsUrl)
                }) {
                    Text(stringResource(R.string.terms_of_service))
                }
                TextButton(onClick = {
                    navController.navigateToWebPage(Constants.privacyUrl)
                }) {
                    Text(stringResource(R.string.privacy_policy))
                }
            }
            Row(
                modifier = Modifier
                    .align(Alignment.BottomCenter)
                    .fillMaxWidth(),
                horizontalArrangement = Arrangement.SpaceBetween
            ) {
                Button(
                    onClick = {
                        val activity = context as? Activity
                        activity?.finish()
                    },
                ) {
                    Text(stringResource(R.string.exit_app))
                }

                Button(
                    onClick = {
                        viewModel.acceptTerms()
                    },
                ) {
                    Text(stringResource(R.string.agree))
                }
            }
        }
    }
}
