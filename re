package com.tuapp.data.repository

import com.google.firebase.auth.FirebaseAuth
import com.google.firebase.database.FirebaseDatabase

class SignUpUserRepository {

    private val auth: FirebaseAuth = FirebaseAuth.getInstance()
    private val db = FirebaseDatabase.getInstance().getReference("users")

    fun signUpUser(
        fullName: String,
        email: String,
        password: String,
        role: String,
        onSuccess: () -> Unit,
        onFailure: (String) -> Unit
    ) {
        auth.createUserWithEmailAndPassword(email, password)
            .addOnSuccessListener { result ->
                val userId = result.user?.uid ?: return@addOnSuccessListener
                val userMap = mapOf(
                    "userId" to userId,
                    "name" to fullName,
                    "email" to email,
                    "role" to role
                )
                db.child(userId).setValue(userMap)
                    .addOnSuccessListener { onSuccess() }
                    .addOnFailureListener { onFailure(it.message ?: "Error en la base de datos") }
            }
            .addOnFailureListener { onFailure(it.message ?: "Error en el registro") }
    }
}

OTRO

package com.tuapp.features.auth.viewmodel

import androidx.lifecycle.ViewModel
import com.tuapp.data.repository.SignUpUserRepository

class SignUpUserViewModel(private val repository: SignUpUserRepository) : ViewModel() {

    fun signUp(
        name: String,
        email: String,
        password: String,
        confirmPassword: String,
        role: String,
        onSuccess: () -> Unit,
        onError: (String) -> Unit
    ) {
        if (name.isBlank() || email.isBlank() || password.isBlank() || confirmPassword.isBlank()) {
            onError("Todos los campos son obligatorios")
            return
        }
        if (!android.util.Patterns.EMAIL_ADDRESS.matcher(email).matches()) {
            onError("Correo inválido")
            return
        }
        if (password.length < 6) {
            onError("La contraseña debe tener al menos 6 caracteres")
            return
        }
        if (password != confirmPassword) {
            onError("Las contraseñas no coinciden")
            return
        }

        repository.signUpUser(name, email, password, role, onSuccess, onError)
    }
}

OTRO

package com.tuapp.features.auth.ui

import android.os.Bundle
import android.view.View
import androidx.fragment.app.Fragment
import androidx.lifecycle.ViewModelProvider
import com.tuapp.R
import com.tuapp.data.repository.SignUpUserRepository
import com.tuapp.databinding.FragmentSignUpUserBinding
import com.tuapp.features.auth.viewmodel.SignUpUserViewModel
import com.tuapp.utils.ViewUtils

class SignUpUserFragment : Fragment(R.layout.fragment_sign_up_user) {

    private var _binding: FragmentSignUpUserBinding? = null
    private val binding get() = _binding!!

    private lateinit var viewModel: SignUpUserViewModel

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        _binding = FragmentSignUpUserBinding.bind(view)

        val repository = SignUpUserRepository()
        viewModel = ViewModelProvider(this, object : ViewModelProvider.Factory {
            override fun <T : androidx.lifecycle.ViewModel> create(modelClass: Class<T>): T {
                return SignUpUserViewModel(repository) as T
            }
        })[SignUpUserViewModel::class.java]

        binding.btnSignUp.setOnClickListener {
            val name = binding.inputName.text.toString().trim()
            val email = binding.inputEmail.text.toString().trim()
            val password = binding.inputPassword.text.toString()
            val confirmPassword = binding.inputConfirmPassword.text.toString()
            val role = "usuario" // Por ahora estático, puedes personalizarlo

            clearErrors()

            viewModel.signUp(name, email, password, confirmPassword, role,
                onSuccess = {
                    ViewUtils.showToast(requireContext(), "Registro exitoso")
                    // Navegación a Home o donde tú desees
                },
                onError = { error ->
                    handleError(error)
                }
            )
        }
    }

    private fun clearErrors() {
        binding.inputNameLayout.error = null
        binding.inputEmailLayout.error = null
        binding.inputPasswordLayout.error = null
        binding.inputConfirmPasswordLayout.error = null
    }

    private fun handleError(error: String) {
        when {
            error.contains("nombre", true) -> binding.inputNameLayout.error = error
            error.contains("correo", true) -> binding.inputEmailLayout.error = error
            error.contains("contraseña", true) -> binding.inputPasswordLayout.error = error
            error.contains("coinciden", true) -> binding.inputConfirmPasswordLayout.error = error
            else -> ViewUtils.showToast(requireContext(), error)
        }
    }

    override fun onDestroyView() {
        super.onDestroyView()
        _binding = null
    }
}

OTRO

package com.tuapp.features.auth.ui

import android.os.Bundle
import android.view.View
import androidx.fragment.app.Fragment
import androidx.lifecycle.ViewModelProvider
import com.tuapp.R
import com.tuapp.data.repository.SignUpUserRepository
import com.tuapp.databinding.FragmentSignUpUserBinding
import com.tuapp.features.auth.viewmodel.SignUpUserViewModel
import com.tuapp.utils.ViewUtils

class SignUpUserFragment : Fragment(R.layout.fragment_sign_up_user) {

    private var _binding: FragmentSignUpUserBinding? = null
    private val binding get() = _binding!!

    private lateinit var viewModel: SignUpUserViewModel

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        _binding = FragmentSignUpUserBinding.bind(view)

        val repository = SignUpUserRepository()
        viewModel = ViewModelProvider(this, object : ViewModelProvider.Factory {
            override fun <T : androidx.lifecycle.ViewModel> create(modelClass: Class<T>): T {
                return SignUpUserViewModel(repository) as T
            }
        })[SignUpUserViewModel::class.java]

        binding.btnSignUp.setOnClickListener {
            val name = binding.inputName.text.toString().trim()
            val email = binding.inputEmail.text.toString().trim()
            val password = binding.inputPassword.text.toString()
            val confirmPassword = binding.inputConfirmPassword.text.toString()
            val role = "usuario" // Por ahora estático, puedes personalizarlo

            clearErrors()

            viewModel.signUp(name, email, password, confirmPassword, role,
                onSuccess = {
                    ViewUtils.showToast(requireContext(), "Registro exitoso")
                    // Navegación a Home o donde tú desees
                },
                onError = { error ->
                    handleError(error)
                }
            )
        }
    }

    private fun clearErrors() {
        binding.inputNameLayout.error = null
        binding.inputEmailLayout.error = null
        binding.inputPasswordLayout.error = null
        binding.inputConfirmPasswordLayout.error = null
    }

    private fun handleError(error: String) {
        when {
            error.contains("nombre", true) -> binding.inputNameLayout.error = error
            error.contains("correo", true) -> binding.inputEmailLayout.error = error
            error.contains("contraseña", true) -> binding.inputPasswordLayout.error = error
            error.contains("coinciden", true) -> binding.inputConfirmPasswordLayout.error = error
            else -> ViewUtils.showToast(requireContext(), error)
        }
    }

    override fun onDestroyView() {
        super.onDestroyView()
        _binding = null
    }
}
