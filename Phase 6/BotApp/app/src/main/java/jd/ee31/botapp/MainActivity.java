package jd.ee31.botapp;

import android.Manifest;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.AsyncTask;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.design.widget.BottomNavigationView;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentTransaction;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
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
import java.util.concurrent.TimeUnit;

import static android.content.pm.PackageManager.PERMISSION_GRANTED;

public class MainActivity extends AppCompatActivity {

    private BluetoothAdapter btA;
    private BluetoothSocket btSock = null;
    public InputStream inStream;
    public OutputStream outStream;
    public ArrayList<String> pendingMsgs = new ArrayList<String>();
    SensorFragment sens_frag;
    InstructionFragment inst_frag;
    NotificationFragment note_frag;
    boolean permGranted = false;
    public BluetoothAdapter myBluetooth;
    public int connectionStatus = -1;
    public int frame;
    public BluetoothDevice arduinoConn = null;
    InputListener task;

    public String gear = "Waiting...", currColor = "Waiting...", speed = "Waiting...", magField = "No", prox = "Waiting...";
    public ArrayList<String> last10Notes = new ArrayList<String>();

    /*public class MyBroadcastReceiver extends BroadcastReceiver {
        @Override
        public void onReceive(Context context, Intent intent) {
            Log.d("got it", "yuh");
            switch (frame) {
                case 0:
                    sens_frag.update_sensor_values();
                    break;
                case 2:
                    note_frag.handle_pending();
                    break;
            }
        }
    };*/

    //MyBroadcastReceiver rec = new MyBroadcastReceiver();

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
                    frame = 0;
                    break;
                case R.id.navigation_inst:
                    inst_frag = InstructionFragment.newInstance();
                    selectedFrag = inst_frag;
                    frame = 1;
                    break;
                case R.id.navigation_notifications:
                    note_frag = NotificationFragment.newInstance();
                    selectedFrag = note_frag;
                    frame = 2;
                    break;
            }
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

        pendingMsgs = new ArrayList<String>();
        last10Notes = new ArrayList<String>();

        if (checkSelfPermission(Manifest.permission.BLUETOOTH) != PackageManager.PERMISSION_GRANTED) {
            String[] arr = {Manifest.permission.BLUETOOTH, Manifest.permission.BLUETOOTH_ADMIN};
            requestPermissions(arr, 0);
        }
        else {
            bt_init();
        }

        task = new InputListener();
        task.execute(this);
    }

    private class InputListener extends AsyncTask<MainActivity, Void, Void> {

        @Override
        protected void onProgressUpdate(Void... values) {
            super.onProgressUpdate(values);
            switch (frame) {
                case 0:
                    sens_frag.update_sensor_values();
                    break;
                case 2:
                    note_frag.handle_pending();
                    break;
            }
        }

        @Override
        public Void doInBackground(MainActivity... act) {
            InputStream inStream = act[0].inStream;
            ArrayList<String> pendingMsgs = act[0].pendingMsgs;
            int frame = act[0].frame;
            SensorFragment sens_frag = act[0].sens_frag;
            NotificationFragment note_frag = act[0].note_frag;

            while (true) {
                try {
                    if (inStream == null) {
                        try {
                            TimeUnit.MILLISECONDS.sleep(100);
                        } catch (InterruptedException e) {
                        }
                        continue;
                    }
                    if (inStream.available() >= 5) {
                        byte[] msg = new byte[5];
                        inStream.read(msg, 0, 5);
                        pendingMsgs.add(new String(msg));
                        Intent newIntent = new Intent();
                        publishProgress();
                    } else if (inStream.available() == 2) {
                        byte[] msg = new byte[2];
                        inStream.read(msg, 0, 2);
                    }
                    else {
                        try {
                            TimeUnit.MILLISECONDS.sleep(100);
                        } catch (InterruptedException e) {
                        }
                    }
                } catch (IOException e) {
                }
            }
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
            Toast.makeText(this, "Bluetooth unavailable", Toast.LENGTH_SHORT).show();
            return;
        } else if (!btA.isEnabled()) {
            Intent startBT = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(startBT, 1);
        } else {
            Set<BluetoothDevice> pairedDevices = btA.getBondedDevices();
            for (BluetoothDevice bt : pairedDevices) {
                if (bt.getName().equals("HC-05")) {
                    arduinoConn = bt;
                }
            }
        }
        if (arduinoConn != null) {
            btSock = null;
            try {
                btSock = arduinoConn.createInsecureRfcommSocketToServiceRecord(myUUID);
                btA.cancelDiscovery();
                btSock.connect();
                inStream = btSock.getInputStream();
                outStream = btSock.getOutputStream();
            } catch (IOException e){
                Toast.makeText(this, "Encountered error connecting to BT Device.", Toast.LENGTH_SHORT).show();
            } catch (NullPointerException e) {
                Toast.makeText(this, "Encountered error connecting to BT Device.", Toast.LENGTH_SHORT).show();
            }
        }
        btA.cancelDiscovery();
    }

    public void sendMsg(View view) {
        inst_frag.sendMsgActual();
    }
}
