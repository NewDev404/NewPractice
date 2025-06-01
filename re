package com.yourcompany.yourapp.data.repository

import com.google.firebase.auth.FirebaseAuth
import com.google.firebase.database.FirebaseDatabase

class SignUpUserRepository {
    private val auth: FirebaseAuth = FirebaseAuth.getInstance()
    private val database = FirebaseDatabase.getInstance().reference

    fun createUser(
        name: String,
        email: String,
        password: String,
        onSuccess: () -> Unit,
        onError: (String) -> Unit
    ) {
        auth.createUserWithEmailAndPassword(email, password)
            .addOnCompleteListener { task ->
                if (task.isSuccessful) {
                    val uid = auth.currentUser?.uid ?: return@addOnCompleteListener
                    val userMap = mapOf(
                        "name" to name,
                        "email" to email,
                        "role" to "usuario"
                    )
                    database.child("users").child(uid).setValue(userMap)
                        .addOnSuccessListener { onSuccess() }
                        .addOnFailureListener { onError(it.localizedMessage ?: "Error en la base de datos") }
                } else {
                    val message = when {
                        task.exception?.message?.contains("email address is already in use") == true ->
                            "Este correo ya está registrado"
                        else -> task.exception?.localizedMessage ?: "Error al crear usuario"
                    }
                    onError(message)
                }
            }
    }
}

OTRO

package com.yourcompany.yourapp.features.sign_up_user.viewmodel

import androidx.lifecycle.LiveData
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.ViewModel
import com.yourcompany.yourapp.data.repository.SignUpUserRepository

class SignUpUserViewModel(private val repository: SignUpUserRepository) : ViewModel() {

    private val _isLoading = MutableLiveData(false)
    val isLoading: LiveData<Boolean> = _isLoading

    private val _errorMessage = MutableLiveData<String?>()
    val errorMessage: LiveData<String?> = _errorMessage

    private val _signUpSuccess = MutableLiveData(false)
    val signUpSuccess: LiveData<Boolean> = _signUpSuccess

    fun createUser(name: String, email: String, password: String) {
        _isLoading.value = true
        repository.createUser(
            name,
            email,
            password,
            onSuccess = {
                _isLoading.value = false
                _signUpSuccess.value = true
            },
            onError = {
                _isLoading.value = false
                _errorMessage.value = it
            }
        )
    }

    fun clearError() {
        _errorMessage.value = null
    }

    fun resetSuccess() {
        _signUpSuccess.value = false
    }
}

OTRO

package com.yourcompany.yourapp.features.sign_up_user.viewmodel

import androidx.lifecycle.ViewModel
import androidx.lifecycle.ViewModelProvider
import com.yourcompany.yourapp.data.repository.SignUpUserRepository

class SignUpUserViewModelFactory(
    private val repository: SignUpUserRepository
) : ViewModelProvider.Factory {
    
    override fun <T : ViewModel> create(modelClass: Class<T>): T {
        if (modelClass.isAssignableFrom(SignUpUserViewModel::class.java)) {
            return SignUpUserViewModel(repository) as T
        }
        throw IllegalArgumentException("Unknown ViewModel class")
    }
}

OTRO

package com.yourcompany.yourapp.features.sign_up_user.ui

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.fragment.app.Fragment
import androidx.lifecycle.ViewModelProvider
import com.yourcompany.yourapp.databinding.FragmentSignUpUserBinding
import com.yourcompany.yourapp.data.repository.SignUpUserRepository
import com.yourcompany.yourapp.features.sign_up_user.viewmodel.SignUpUserViewModel
import com.yourcompany.yourapp.features.sign_up_user.viewmodel.SignUpUserViewModelFactory
import com.yourcompany.yourapp.core.utils.ViewUtils

class SignUpUserFragment : Fragment() {

    private lateinit var binding: FragmentSignUpUserBinding
    private lateinit var viewModel: SignUpUserViewModel

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View {
        binding = FragmentSignUpUserBinding.inflate(inflater, container, false)
        return binding.root
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        val factory = SignUpUserViewModelFactory(SignUpUserRepository())
        viewModel = ViewModelProvider(this, factory)[SignUpUserViewModel::class.java]

        binding.btnSignUp.setOnClickListener {
            validarYCrearUsuario()
        }

        viewModel.isLoading.observe(viewLifecycleOwner) {
            binding.progressBar.visibility = if (it) View.VISIBLE else View.GONE
        }

        viewModel.errorMessage.observe(viewLifecycleOwner) {
            it?.let {
                ViewUtils.showToast(requireContext(), it)
                viewModel.clearError()
            }
        }

        viewModel.signUpSuccess.observe(viewLifecycleOwner) {
            if (it) {
                ViewUtils.showToast(requireContext(), "¡Registro exitoso!")
                limpiarCampos()
                viewModel.resetSuccess()
                // Aquí puedes redirigir si lo deseas
            }
        }
    }

    private fun validarYCrearUsuario() {
        val name = binding.inputName.text.toString().trim()
        val email = binding.inputEmail.text.toString().trim()
        val password = binding.inputPassword.text.toString().trim()
        val confirmPassword = binding.inputConfirmPassword.text.toString().trim()

        var valido = true

        if (name.isEmpty()) {
            binding.inputNameLayout.error = "Ingresa tu nombre"
            valido = false
        } else binding.inputNameLayout.error = null

        if (email.isEmpty()) {
            binding.inputEmailLayout.error = "Ingresa un correo"
            valido = false
        } else binding.inputEmailLayout.error = null

        if (password.isEmpty()) {
            binding.inputPasswordLayout.error = "Ingresa una contraseña"
            valido = false
        } else binding.inputPasswordLayout.error = null

        if (confirmPassword != password) {
            binding.inputConfirmPasswordLayout.error = "Las contraseñas no coinciden"
            valido = false
        } else binding.inputConfirmPasswordLayout.error = null

        if (valido) {
            viewModel.createUser(name, email, password)
        }
    }

    private fun limpiarCampos() {
        binding.inputName.setText("")
        binding.inputEmail.setText("")
        binding.inputPassword.setText("")
        binding.inputConfirmPassword.setText("")
    }
}

