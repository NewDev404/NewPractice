package com.project.features.equipment_list.view_model

import androidx.lifecycle.ViewModel
import androidx.lifecycle.ViewModelProvider
import com.project.data.repository.EquipmentListRepository

class EquipmentListViewModelFactory(
    private val repository: EquipmentListRepository
) : ViewModelProvider.Factory {

    override fun <T : ViewModel> create(modelClass: Class<T>): T {
        if (modelClass.isAssignableFrom(EquipmentListViewModel::class.java)) {
            return EquipmentListViewModel(repository) as T
        }
        throw IllegalArgumentException("Unknown ViewModel class")
    }
}



OTRO

val repository = EquipmentListRepository()
val factory = EquipmentListViewModelFactory(repository)
viewModel = ViewModelProvider(this, factory)[EquipmentListViewModel::class.java]
