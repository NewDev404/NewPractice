class EquipmentDetailRepository {

    private val dbRef = FirebaseDatabase.getInstance().getReference("equipments")

    fun getSensorData(equipmentId: String, callback: (Sensor?) -> Unit) {
        dbRef.child(equipmentId).child("sensor")
            .addValueEventListener(object : ValueEventListener {
                override fun onDataChange(snapshot: DataSnapshot) {
                    val sensor = snapshot.getValue(Sensor::class.java)
                    callback(sensor)
                }

                override fun onCancelled(error: DatabaseError) {
                    callback(null)
                }
            })
    }
}


OTRO


class EquipmentDetailViewModel(
    private val repository: EquipmentDetailRepository
) : ViewModel() {

    private val _sensorData = MutableLiveData<Sensor>()
    val sensorData: LiveData<Sensor> = _sensorData

    private val _alertMessage = MutableLiveData<String?>()
    val alertMessage: LiveData<String?> = _alertMessage

    fun loadSensorData(equipmentId: String) {
        repository.getSensorData(equipmentId) { sensor ->
            sensor?.let {
                _sensorData.postValue(it)
                detectAnomalies(it)
            }
        }
    }

    private fun detectAnomalies(sensor: Sensor) {
        when {
            sensor.temperature > 90 -> _alertMessage.postValue("¡Temperatura demasiado alta!")
            sensor.levelFuel < 20 -> _alertMessage.postValue("¡Combustible muy bajo!")
            sensor.levelWater < 15 -> _alertMessage.postValue("¡Nivel de agua crítico!")
            else -> _alertMessage.postValue(null) // todo está bien
        }
    }
}


OTRO


class EquipmentDetailViewModelFactory(
    private val repository: EquipmentDetailRepository
) : ViewModelProvider.Factory {
    override fun <T : ViewModel> create(modelClass: Class<T>): T {
        if (modelClass.isAssignableFrom(EquipmentDetailViewModel::class.java)) {
            @Suppress("UNCHECKED_CAST")
            return EquipmentDetailViewModel(repository) as T
        }
        throw IllegalArgumentException("Unknown ViewModel class")
    }
}


OTRO


class EquipmentDetailFragment : Fragment() {

    private lateinit var viewModel: EquipmentDetailViewModel
    private lateinit var equipmentId: String // lo recibes por Bundle

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View {
        val view = inflater.inflate(R.layout.fragment_equipment_detail, container, false)

        // Obtener el ID del equipo
        equipmentId = arguments?.getString("equipmentId") ?: ""

        // ViewModel
        val repository = EquipmentDetailRepository()
        val factory = EquipmentDetailViewModelFactory(repository)
        viewModel = ViewModelProvider(this, factory)[EquipmentDetailViewModel::class.java]

        viewModel.loadSensorData(equipmentId)

        setupObservers(view)

        return view
    }

    private fun setupObservers(view: View) {
        val voltageText = view.findViewById<TextView>(R.id.voltageText)
        val temperatureText = view.findViewById<TextView>(R.id.temperatureText)
        val levelWaterText = view.findViewById<TextView>(R.id.levelWaterText)
        val distanceWaterText = view.findViewById<TextView>(R.id.distanceWaterText)
        val levelFuelText = view.findViewById<TextView>(R.id.levelFuelText)
        val distanceFuelText = view.findViewById<TextView>(R.id.distanceFuelText)

        val alertBox = view.findViewById<TextView>(R.id.alertBox)

        viewModel.sensorData.observe(viewLifecycleOwner) { sensor ->
            voltageText.text = "${sensor.voltage} V"
            temperatureText.text = "${sensor.temperature} °C"
            levelWaterText.text = "${sensor.levelWater} %"
            distanceWaterText.text = "${sensor.distanceWater} cm"
            levelFuelText.text = "${sensor.levelFuel} %"
            distanceFuelText.text = "${sensor.distanceFuel} cm"
        }

        viewModel.alertMessage.observe(viewLifecycleOwner) { message ->
            if (message != null) {
                alertBox.visibility = View.VISIBLE
                alertBox.text = message
            } else {
                alertBox.visibility = View.GONE
            }
        }
    }
}

