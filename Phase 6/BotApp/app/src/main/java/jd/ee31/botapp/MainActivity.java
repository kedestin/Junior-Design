package jd.ee31.botapp;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.design.widget.BottomNavigationView;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentTransaction;
import android.support.v7.app.AppCompatActivity;
import android.view.MenuItem;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.Queue;
import java.util.UUID;

public class MainActivity extends AppCompatActivity {

    private BTConnection btConn;
    private BluetoothSocket btSock;
    public InputStream inStream;
    public OutputStream outStream;
    public ArrayList<String> pendingMsgs;
    SensorFragment sens_frag;
    InstructionFragment inst_frag;
    NotificationFragment note_frag;

    public String gear, currColor, speed, magField, prox;



    static final UUID myUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");

    private BottomNavigationView.OnNavigationItemSelectedListener mOnNavigationItemSelectedListener
            = new BottomNavigationView.OnNavigationItemSelectedListener() {

        @Override
        public boolean onNavigationItemSelected(@NonNull MenuItem item) {
            Fragment selectedFrag = null;
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
            FragmentTransaction transaction = getSupportFragmentManager().beginTransaction();
            transaction.replace(R.id.frame_layout, selectedFrag);
            return true;
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        BottomNavigationView navigation = (BottomNavigationView) findViewById(R.id.navigation);
        navigation.setOnNavigationItemSelectedListener(mOnNavigationItemSelectedListener);


        btConn = new BTConnection();
        pendingMsgs = new ArrayList<String>();

        do {
            btConn.initBTConn();
            switch (btConn.connectionStatus) {
                case -1:
                    Toast.makeText(this, "Bluetooth unavailable", Toast.LENGTH_SHORT).show();
                    break;
                case -2:
                    Intent startBT = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                    startActivityForResult(startBT, 1);
                    break;
            }
        } while (btConn.arduinoConn == null);

        btSock = null;
        try {
            btSock = btConn.arduinoConn.createInsecureRfcommSocketToServiceRecord(myUUID);
            btConn.myBluetooth.cancelDiscovery();
            btSock.connect();
            inStream = btSock.getInputStream();
            outStream = btSock.getOutputStream();
        } catch (IOException e) {
            Toast.makeText(this, "Encountered error connecting to BT Device.", Toast.LENGTH_SHORT).show();
        }
        btConn.myBluetooth.cancelDiscovery();
    }


}
