package jd.ee31.botapp;

import android.Manifest;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.design.widget.BottomNavigationView;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentTransaction;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.MenuItem;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.Queue;
import java.util.Set;
import java.util.UUID;

import static android.content.pm.PackageManager.PERMISSION_GRANTED;

public class MainActivity extends AppCompatActivity {

    private BTConnection btConn;
    private BluetoothAdapter btA;
    private BluetoothSocket btSock = null;
    public InputStream inStream;
    public OutputStream outStream;
    public ArrayList<String> pendingMsgs;
    SensorFragment sens_frag;
    InstructionFragment inst_frag;
    NotificationFragment note_frag;
    boolean permGranted = false;
    public BluetoothAdapter myBluetooth;
    public int connectionStatus = -1;
    public BluetoothDevice arduinoConn = null;

    public String gear, currColor, speed, magField, prox;
    public ArrayList<String> last10Notes;

    static final UUID myUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");

    private BottomNavigationView.OnNavigationItemSelectedListener mOnNavigationItemSelectedListener
            = new BottomNavigationView.OnNavigationItemSelectedListener() {

        @Override
        public boolean onNavigationItemSelected(@NonNull MenuItem item) {
            Fragment selectedFrag = null;
            Log.d("ugh", "Before switch");
            switch (item.getItemId()) {
                case R.id.navigation_sensor:
                    sens_frag = SensorFragment.newInstance();
                    selectedFrag = sens_frag;
                    break;
                case R.id.navigation_inst:
                    inst_frag = InstructionFragment.newInstance();
                    selectedFrag = inst_frag;
                    break;
                case R.id.navigation_notifications:
                    note_frag = NotificationFragment.newInstance();
                    selectedFrag = note_frag;
                    break;
            }
            Log.d("UGH", "After switch");
            FragmentTransaction transaction = getSupportFragmentManager().beginTransaction();
            transaction.replace(R.id.frame_layout, selectedFrag);
            transaction.commit();
            return true;
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        sens_frag = SensorFragment.newInstance();

        BottomNavigationView navigation = (BottomNavigationView) findViewById(R.id.navigation);
        navigation.setOnNavigationItemSelectedListener(mOnNavigationItemSelectedListener);
        FragmentTransaction transaction = getSupportFragmentManager().beginTransaction();
        transaction.replace(R.id.frame_layout, sens_frag);
        transaction.commit();

        Log.d("whoops", "yikes");
        btConn = new BTConnection();
        pendingMsgs = new ArrayList<String>();
        last10Notes = new ArrayList<String>();

        if (checkSelfPermission(Manifest.permission.BLUETOOTH) != PackageManager.PERMISSION_GRANTED) {
            String[] arr = {Manifest.permission.BLUETOOTH, Manifest.permission.BLUETOOTH_ADMIN};
            requestPermissions(arr, 0);
        }
        else {
            bt_init();
        }

    }

    @Override
    public void onRequestPermissionsResult(int a, String[] arr, int[] brr) {
        if (brr[0] == PERMISSION_GRANTED && brr[1] == PERMISSION_GRANTED) {
            bt_init();
        }
    }

    private void bt_init() {
        BluetoothAdapter btA = BluetoothAdapter.getDefaultAdapter();

        if (btA == null) {
            Log.d("oh", "shit");
            Toast.makeText(this, "Bluetooth unavailable", Toast.LENGTH_SHORT).show();
        } else if (!btA.isEnabled()) {
            Intent startBT = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(startBT, 1);
        } else {
            Set<BluetoothDevice> pairedDevices = btA.getBondedDevices();
            Log.d("ruh", "roh");
            for (BluetoothDevice bt : pairedDevices) {
                Log.d("woo!", bt.getName());
                if (bt.getName().equals("HC-05")) {
                    arduinoConn = bt;
                }
            }
        }
        if (arduinoConn != null) {
            btSock = null;
            try {
                btSock = arduinoConn.createInsecureRfcommSocketToServiceRecord(myUUID);
                myBluetooth.cancelDiscovery();
                btSock.connect();
                inStream = btSock.getInputStream();
                outStream = btSock.getOutputStream();
            } catch (IOException e){
                Toast.makeText(this, "Encountered error connecting to BT Device.", Toast.LENGTH_SHORT).show();
            }
        }
        btA.cancelDiscovery();
    }

}
