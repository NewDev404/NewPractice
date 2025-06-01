class EquipmentDetailFragment : Fragment() {

    private var _binding: FragmentEquipmentDetailBinding? = null
    private val binding get() = _binding!!

    private lateinit var viewModel: EquipmentDetailViewModel
    private lateinit var equipmentId: String

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View {
        _binding = FragmentEquipmentDetailBinding.inflate(inflater, container, false)

        equipmentId = arguments?.getString("equipmentId") ?: ""

        val repository = EquipmentDetailRepository()
        val factory = EquipmentDetailViewModelFactory(repository)
        viewModel = ViewModelProvider(this, factory)[EquipmentDetailViewModel::class.java]

        viewModel.loadSensorData(equipmentId)

        setupObservers()

        return binding.root
    }

    private fun setupObservers() {
        viewModel.sensorData.observe(viewLifecycleOwner) { sensor ->
            binding.voltageText.text = "${sensor.voltage} V"
            binding.temperatureText.text = "${sensor.temperature} °C"
            binding.levelWaterText.text = "${sensor.levelWater} %"
            binding.distanceWaterText.text = "${sensor.distanceWater} cm"
            binding.levelFuelText.text = "${sensor.levelFuel} %"
            binding.distanceFuelText.text = "${sensor.distanceFuel} cm"
        }

        viewModel.alertMessage.observe(viewLifecycleOwner) { message ->
            if (message != null) {
                binding.alertBox.visibility = View.VISIBLE
                binding.alertBox.text = message
            } else {
                binding.alertBox.visibility = View.GONE
            }
        }
    }

    override fun onDestroyView() {
        super.onDestroyView()
        _binding = null // Evita fugas de memoria
    }
}
