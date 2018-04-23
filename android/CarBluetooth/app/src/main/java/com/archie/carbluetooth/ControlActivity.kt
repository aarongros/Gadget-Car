package com.archie.carbluetooth

import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothDevice
import android.bluetooth.BluetoothSocket
import android.content.pm.ActivityInfo
import android.os.AsyncTask
import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import kotlinx.android.synthetic.main.activity_control.*
import java.io.IOException
import java.util.*


class ControlActivity : AppCompatActivity() {

    companion object {
        private val uuid = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB")
        private var socket: BluetoothSocket? = null
        private lateinit var adapter: BluetoothAdapter
        private var isConnected = false
        private lateinit var address: String
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_control)
        this.supportActionBar?.hide()
        this.requestedOrientation = ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE

        address = this.intent.getStringExtra(MainActivity.ADDRESS_EXTRA)
        ConnectToDevice().execute()

        this.joystick.setOnMoveListener { angle, strength ->
            this.sendCommand(Integer.toString(angle) + "," + Integer.toString(strength))
        }

        this.setHeadlightsSwitchListener()
        this.setHazardSwitchListener()
        this.setLeftBlinkerSwitchListener()
        this.setRightBlinkerSwitchListener()
    }

    private fun updateSwitches(left: Boolean, right: Boolean) {
        if (left && this.rightBlinkerSwitch.isChecked) {
            this.rightBlinkerSwitch.setOnCheckedChangeListener(null)
            this.rightBlinkerSwitch.isChecked = false
            this.setRightBlinkerSwitchListener()
        }
        else if (right && this.leftBlinkerSwitch.isChecked) {
            this.leftBlinkerSwitch.setOnCheckedChangeListener(null)
            this.leftBlinkerSwitch.isChecked = false
            this.setLeftBlinkerSwitchListener()
        }
    }

    private fun setLeftBlinkerSwitchListener() {
        this.leftBlinkerSwitch.setOnCheckedChangeListener { _, bool ->
            this.sendCommand("L" + if (bool) "1" else "0")
            if (bool && this.rightBlinkerSwitch.isChecked) {
                this.updateSwitches(true, false)
            }
        }
    }

    private fun setRightBlinkerSwitchListener() {
        this.rightBlinkerSwitch.setOnCheckedChangeListener { _, bool ->
            this.sendCommand("R" + if (bool) "1" else "0")
            if (bool && this.leftBlinkerSwitch.isChecked) {
                this.updateSwitches(false, true)
            }
        }
    }

    private fun setHeadlightsSwitchListener() {
        this.headlightsSwitch.setOnCheckedChangeListener { _, bool ->
            this.sendCommand("D" + if (bool) "1" else "0")
        }
    }

    private fun setHazardSwitchListener() {
        this.hazardsSwitch.setOnCheckedChangeListener { _, bool ->
            this.sendCommand("Z" + if (bool) "1" else "0")
        }
    }

    override fun onBackPressed() {
        super.onBackPressed()
        this.disconnect()
    }

    private fun sendCommand(cmd: String) {
        if (socket != null) {
            try {
                socket!!.outputStream.write((cmd + "\n").toByteArray())
            } catch (e: IOException) {
                e.printStackTrace()
            }
        }
    }

    private fun disconnect() {
        if (socket != null) {
            try {
                socket!!.close()
                socket = null
                isConnected = false
            } catch (e: IOException) {
                e.printStackTrace()
            }
        }
    }

    private class ConnectToDevice : AsyncTask<Void, Void, String>() {

        private var connectSuccess = true

        override fun doInBackground(vararg p0: Void?): String {
            try {
                if (socket == null || !isConnected) {
                    adapter = BluetoothAdapter.getDefaultAdapter()
                    val device = adapter.getRemoteDevice(address)
                    socket = device.createInsecureRfcommSocketToServiceRecord(uuid)
                    BluetoothAdapter.getDefaultAdapter().cancelDiscovery()
                    socket!!.connect()
                }
            } catch (e: IOException) {
                this.connectSuccess = false
                e.printStackTrace()
            }

            return address
        }

        override fun onPostExecute(result: String?) {
            super.onPostExecute(result)

            if (!this.connectSuccess) {
                Log.i("bt error", "couldn't connect")
            } else {
                isConnected = true
            }
        }

    }

}
