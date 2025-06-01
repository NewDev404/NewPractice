package com.yourcompany.yourapp.features.equipment_list.ui.adapter

import android.view.LayoutInflater
import android.view.ViewGroup
import androidx.recyclerview.widget.RecyclerView
import com.yourcompany.yourapp.data.model.Equipment
import com.yourcompany.yourapp.databinding.ItemEquipmentBinding

class EquipmentAdapter(
    private val onItemClick: (String) -> Unit
) : RecyclerView.Adapter<EquipmentAdapter.ViewHolder>() {

    private val items = mutableListOf<Equipment>()

    fun submitList(newList: List<Equipment>) {
        items.clear()
        items.addAll(newList)
        notifyDataSetChanged()
    }

    inner class ViewHolder(private val binding: ItemEquipmentBinding) :
        RecyclerView.ViewHolder(binding.root) {
        fun bind(equipment: Equipment) {
            binding.equipmentName.text = equipment.name
            binding.equipmentType.text = equipment.type

            binding.root.setOnClickListener {
                onItemClick(equipment.id)
            }
        }
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int) = ViewHolder(
        ItemEquipmentBinding.inflate(LayoutInflater.from(parent.context), parent, false)
    )

    override fun onBindViewHolder(holder: ViewHolder, position: Int) {
        holder.bind(items[position])
    }

    override fun getItemCount(): Int = items.size
}
