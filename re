package com.project.features.equipment_list.view

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.core.widget.addTextChangedListener
import androidx.fragment.app.Fragment
import androidx.lifecycle.ViewModelProvider
import androidx.recyclerview.widget.LinearLayoutManager
import com.project.databinding.FragmentEquipmentListBinding
import com.project.features.equipment_list.adapter.EquipmentAdapter
import com.project.features.equipment_list.repository.EquipmentListRepository
import com.project.features.equipment_list.view_model.EquipmentListViewModel
import com.project.utils.ToastUtils

class EquipmentListFragment : Fragment() {

    private var _binding: FragmentEquipmentListBinding? = null
    private val binding get() = _binding!!
    private lateinit var viewModel: EquipmentListViewModel
    private lateinit var adapter: EquipmentAdapter

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View {
        _binding = FragmentEquipmentListBinding.inflate(inflater, container, false)
        return binding.root
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        viewModel = ViewModelProvider(
            this,
            EquipmentListViewModel.Factory(EquipmentListRepository())
        )[EquipmentListViewModel::class.java]

        setupRecyclerView()
        setupObservers()
        setupListeners()

        viewModel.loadEquipment()
    }

    private fun setupRecyclerView() {
        adapter = EquipmentAdapter { equipment ->
            // Navegación personalizada
            ToastUtils.show(requireContext(), "Seleccionado: ${equipment.name}")
            // Aquí puedes redirigir a EquipmentDetailFragment con equipment.id
        }
        binding.recyclerEquipment.layoutManager = LinearLayoutManager(requireContext())
        binding.recyclerEquipment.adapter = adapter
    }

    private fun setupObservers() {
        viewModel.filteredEquipmentList.observe(viewLifecycleOwner) { equipmentList ->
            adapter.submitList(equipmentList)
        }

        viewModel.isLoading.observe(viewLifecycleOwner) { isLoading ->
            binding.progressBar.visibility = if (isLoading) View.VISIBLE else View.GONE
        }
    }

    private fun setupListeners() {
        binding.swipeRefresh.setOnRefreshListener {
            viewModel.loadEquipment()
            binding.swipeRefresh.isRefreshing = false
        }

        binding.inputSearch.addTextChangedListener {
            val query = it.toString().trim()
            viewModel.filterEquipment(query)
        }
    }

    override fun onDestroyView() {
        super.onDestroyView()
        _binding = null
    }
}




OTRO




private val _filteredEquipmentList = MutableLiveData<List<Equipment>>()
val filteredEquipmentList: LiveData<List<Equipment>> = _filteredEquipmentList

private var allEquipment = listOf<Equipment>()

fun loadEquipment() {
    _isLoading.value = true
    repository.getAllEquipment { equipmentList ->
        allEquipment = equipmentList
        _filteredEquipmentList.value = allEquipment
        _isLoading.value = false
    }
}

fun filterEquipment(query: String) {
    val filtered = if (query.isEmpty()) {
        allEquipment
    } else {
        allEquipment.filter {
            it.name.contains(query, ignoreCase = true) ||
            it.type.contains(query, ignoreCase = true)
        }
    }
    _filteredEquipmentList.value = filtered
}

