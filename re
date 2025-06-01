package com.yourcompany.yourapp.features.equipment_list.viewmodel

import androidx.lifecycle.LiveData
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.ViewModel
import com.yourcompany.yourapp.data.model.Equipment
import com.yourcompany.yourapp.data.repository.EquipmentListRepository

class EquipmentListViewModel(private val repository: EquipmentListRepository) : ViewModel() {

    private val _equipmentList = MutableLiveData<List<Equipment>>()
    val equipmentList: LiveData<List<Equipment>> = _equipmentList

    fun fetchEquipment() {
        repository.getEquipmentList { list ->
            _equipmentList.value = list
        }
    }
}

OTRO

package com.yourcompany.yourapp.data.repository

import com.google.firebase.database.*
import com.yourcompany.yourapp.data.model.Equipment

class EquipmentListRepository {

    private val database = FirebaseDatabase.getInstance().getReference("equipment")

    fun getEquipmentList(onResult: (List<Equipment>) -> Unit) {
        database.addListenerForSingleValueEvent(object : ValueEventListener {
            override fun onDataChange(snapshot: DataSnapshot) {
                val list = mutableListOf<Equipment>()
                for (child in snapshot.children) {
                    val equipment = child.getValue(Equipment::class.java)
                    equipment?.let {
                        list.add(it.copy(id = child.key ?: ""))
                    }
                }
                onResult(list)
            }

            override fun onCancelled(error: DatabaseError) {
                onResult(emptyList())
            }
        })
    }
}

OTRO

package com.yourcompany.yourapp.features.equipment_list.ui

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.fragment.app.Fragment
import androidx.lifecycle.ViewModelProvider
import androidx.recyclerview.widget.LinearLayoutManager
import com.yourcompany.yourapp.databinding.FragmentEquipmentListBinding
import com.yourcompany.yourapp.features.equipment_list.ui.adapter.EquipmentAdapter
import com.yourcompany.yourapp.features.equipment_list.viewmodel.EquipmentListViewModel
import com.yourcompany.yourapp.utils.ToastUtils

class EquipmentListFragment : Fragment() {

    private lateinit var binding: FragmentEquipmentListBinding
    private lateinit var viewModel: EquipmentListViewModel
    private lateinit var adapter: EquipmentAdapter

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?, savedInstanceState: Bundle?
    ): View {
        binding = FragmentEquipmentListBinding.inflate(inflater, container, false)
        return binding.root
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        viewModel = ViewModelProvider(this)[EquipmentListViewModel::class.java]

        adapter = EquipmentAdapter { equipmentId ->
            ToastUtils.show(requireContext(), "ID seleccionado: $equipmentId")
            // Aquí puedes redirigir al fragmento de detalles
        }

        binding.recyclerEquipment.layoutManager = LinearLayoutManager(requireContext())
        binding.recyclerEquipment.adapter = adapter

        binding.swipeRefresh.setOnRefreshListener {
            viewModel.fetchEquipment()
        }

        viewModel.equipmentList.observe(viewLifecycleOwner) { list ->
            adapter.submitList(list)
            binding.swipeRefresh.isRefreshing = false
        }

        viewModel.fetchEquipment()
    }
}

