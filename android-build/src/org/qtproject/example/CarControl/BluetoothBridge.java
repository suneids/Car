package org.qtproject.example.CarControl;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Context;
import android.util.Log;


import java.io.OutputStream;
import java.io.IOException;
import java.util.UUID;

public class BluetoothBridge {
    private static final String TAG = "BluetoothBridge";
    private static final UUID SPP_UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
    private BluetoothAdapter adapter;
    private BluetoothSocket socket;
    private OutputStream out;

    private static Context getContext(){
        try{
            return org.qtproject.qt.android.bindings.QtActivity.getContext();
        }
        catch(Exception e){
            Log.e(TAG, "Failed to get context: " + e.getMessage);
            return null;
        }
    }

    
    static{
        Context context = getContext();
        if(context!=null){
            adapter = BluetoothAdapter.getDefaultAdapter();
            if(adapter==null){
                Log.e(TAG, "Bluetooth not supported");
            }
        }
        else{
            Log.e(TAG, "Context is null, Bluetooth initialization failed");
        }
    }


    public BluetoothBridge(){
        if(adapter==null){
            Log.e(TAG, "Bluetooth not supported");
        }
    }

    public boolean connect(String macAdress){
        if(adapter == null){
            Log.e(TAG, "Bluetooth not supported");
        }
        BluetoothDevice device = adapter.getRemoteDevice(macAdress);
        try {
            adapter.cancelDiscovery();
            socket = device.createRfcommSocketToServiceRecord(SPP_UUID);
            socket.connect();
            out = socket.getOutputStream();
            Log.d(TAG, "Connected to" + macAdress);
            return true;
        }
        catch (IOException e){
            Log.e(TAG, "Connection failed", e);
            return false;
        }
    }


    public boolean send(String message){
        if(out == null) return false;
        try {
            out.write(message.getBytes());
            out.flush();
            Log.d(TAG, "Message sent"+message);
            return true;
        }
        catch(IOException e){
            Log.e(TAG, "Send failed" + e);
            return false;
        }
    }

    public void disconnect(){
        try{
            if(socket!=null) socket.close();
        }
        catch(IOException e){
            e.printStackTrace();
        }
    }
}