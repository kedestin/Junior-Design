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

import java.util.Iterator;
import java.util.concurrent.Semaphore;

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
    boolean locked = false;
    static Semaphore lock = new Semaphore(1);

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
        public void update_sensor_values(String msg) {
            if (msg.substring(0,1).equals("s")) {
                String sens = msg.substring(1, 2);
                switch (sens) {
                    case "c":
                        currColor = msg.substring(2);
                        break;
                    case "d":
                        speed = msg.substring(2);
                        break;
                    case "p":
                        prox = msg.substring(2);
                        break;
                    case "a":
                        magField = msg.substring(2);
                        break;
                    case "g":
                        gear = msg.substring(2);
                        break;
                }
            }
        }

        public void handle_pending(String msg) {
            if (msg.substring(0,1).equals("m")) {
                String msgType = msg.substring(1,2);
                switch (msgType) {
                    case "c":
                        switch (msg.substring(2)) {
                            case "001":
                                last10Notes.add("Rear Bumper Collided with Object!\n");
                                break;
                            case "010":
                                last10Notes.add("Left Front Bumper Collided with Object!\n");
                                break;
                            case "100":
                                last10Notes.add("Right Front Bumper Collided with Object!\n");
                                break;
                        }
                        break;
                    case "r":
                        last10Notes.add("Received " + msg.substring(2) + "ms message\n");
                        break;
                    case "t":
                        last10Notes.add("Sent " + msg.substring(2) + "ms message\n");
                        break;
                }
                if (last10Notes == null) Log.d("huh", "whoops?");
                if (last10Notes.size() > 20) last10Notes.remove(0);
            }
        }

        @Override
        protected void onProgressUpdate(Void... values) {
            switch (frame) {
                case 0:
                    sens_frag.print_values();
                    break;
                case 2:
                    note_frag.print_messages();
                    break;
            }
        }

        @Override
        public Void doInBackground(MainActivity... act) {
            InputStream inStream = act[0].inStream;
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
                        char firstChar = (char)inStream.read();
                        while (firstChar != 's' && firstChar != 'm') {
                            firstChar = (char)inStream.read();
                        }
                        byte[] msg = new byte[4];
                        inStream.read(msg, 0, 4);
                        String msgFinal = Character.toString(firstChar) + new String(msg);
                        switch (frame) {
                            case 0:
                                update_sensor_values(msgFinal);
                                break;
                            case 1:
                                handle_pending(msgFinal);
                                break;
                        }
                        publishProgress();
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
