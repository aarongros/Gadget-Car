package com.archie.carbluetooth

import android.app.Activity
import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothDevice
import android.content.Intent
import android.content.pm.ActivityInfo
import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.widget.AdapterView
import android.widget.ArrayAdapter
import kotlinx.android.synthetic.main.activity_main.*
import org.jetbrains.anko.startActivity
import org.jetbrains.anko.toast

class MainActivity : AppCompatActivity() {

    companion object {
        private const val REQUEST_ENABLE_BLUETOOTH = 1
        const val ADDRESS_EXTRA = "device_address"
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        this.requestedOrientation = ActivityInfo.SCREEN_ORIENTATION_PORTRAIT

        val bluetoothAdapter = BluetoothAdapter.getDefaultAdapter() ?: return

        if (!bluetoothAdapter.isEnabled) {
            this.startActivityForResult(Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE), REQUEST_ENABLE_BLUETOOTH)
        }

        this.refreshButton.setOnClickListener { this.updatePairedDeviceList() }
        this.updatePairedDeviceList()
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        super.onActivityResult(requestCode, resultCode, data)

        val bluetoothAdapter = BluetoothAdapter.getDefaultAdapter() ?: return
        if (requestCode == REQUEST_ENABLE_BLUETOOTH && resultCode == Activity.RESULT_OK && bluetoothAdapter.isEnabled) {
            toast("Bluetooth enabled, hoe")
        }
        else {
            toast("no bluetooth 4u")
        }
    }

    private fun updatePairedDeviceList() {
        val bluetoothAdapter = BluetoothAdapter.getDefaultAdapter() ?: return
        if (!bluetoothAdapter.isEnabled) { return }


        val deviceList = bluetoothAdapter.bondedDevices.toList()
        this.deviceSelectionListView.adapter = ArrayAdapter(this, android.R.layout.simple_list_item_1, deviceList.map { device -> device.name })
        this.deviceSelectionListView.onItemClickListener = AdapterView.OnItemClickListener { _, _, pos, _ ->
            val deviceAddress = deviceList[pos].address
            val intent = Intent(this, ControlActivity::class.java)
            intent.putExtra(ADDRESS_EXTRA, deviceAddress)
            this.startActivity(intent)
        }
    }

}
