package jd.ee31.botapp;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;

public class SensorFragment extends Fragment {

    private InputStream inStream;
    private OutputStream outStream;
    private ArrayList<String> pendingMsgs;
    TextView tvGearVal, tvSpeedVal, tvColorVal, tvProxVal, tvMagVal;


    public static SensorFragment newInstance() {
        SensorFragment fragment = new SensorFragment();
        return fragment;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                        Bundle savedInstanceState) {
        View sens_view = inflater.inflate(R.layout.activity_sens, container, false);

        /* Set view elements */
        tvColorVal = sens_view.findViewById(R.id.tvColorVal);
        tvGearVal = sens_view.findViewById(R.id.tvGearVal);
        tvMagVal = sens_view.findViewById(R.id.tvMagVal);
        tvProxVal = sens_view.findViewById(R.id.tvProxVal);
        tvSpeedVal = sens_view.findViewById(R.id.tvSpeedVal);

        init_sensor_values();
        return sens_view;
    }

    private void init_sensor_values() {
        inStream = ((MainActivity) getActivity()).inStream;
        outStream = ((MainActivity) getActivity()).outStream;
        pendingMsgs = ((MainActivity) getActivity()).pendingMsgs;

        for (String msg : pendingMsgs) {
            if (msg.substring(0,3).equals("Sen")) {
                String sens = msg.substring(3,6);
                switch (sens) {
                    case "col":
                        ((MainActivity) getActivity()).currColor = msg.substring(6);
                        break;
                    case "spd":
                        ((MainActivity) getActivity()).speed = msg.substring(6);
                        break;
                    case "pro":
                        ((MainActivity) getActivity()).prox = msg.substring(6);
                        break;
                    case "mag":
                        ((MainActivity) getActivity()).magField = msg.substring(6);
                        break;
                    case "gea":
                        ((MainActivity) getActivity()).gear = msg.substring(6);
                        break;
                }
                pendingMsgs.remove(msg);
            }
        }
        
    }
}
