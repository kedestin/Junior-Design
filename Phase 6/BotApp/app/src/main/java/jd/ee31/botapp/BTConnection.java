package jd.ee31.botapp;
import android.bluetooth.*;
import java.util.Set;


public class BTConnection {
    public BluetoothAdapter myBluetooth;
    public int connectionStatus = -1;
    public BluetoothDevice arduinoConn = null;


    public void initBTConn() {
        myBluetooth = BluetoothAdapter.getDefaultAdapter();
        if (myBluetooth == null) {
            connectionStatus = -1;
            return;
        } else if (!myBluetooth.isEnabled()) {
            connectionStatus = -2;
            return;
        } else {
            Set<BluetoothDevice> pairedDevices = myBluetooth.getBondedDevices();

            for (BluetoothDevice bt : pairedDevices) {
                if (bt.getName().equals("HC-05")) {
                    arduinoConn = bt;
                }
            }
        }
    }


}
