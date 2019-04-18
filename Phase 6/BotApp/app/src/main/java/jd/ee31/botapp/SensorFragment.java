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

        update_sensor_values();
        return sens_view;
    }

    public void update_sensor_values() {
        pendingMsgs = ((MainActivity) getActivity()).pendingMsgs;

        for (String msg : pendingMsgs) {
            if (msg.substring(0,1).equals("s")) {
                String sens = msg.substring(1,2);
                switch (sens) {
                    case "c":
                        ((MainActivity) getActivity()).currColor = msg.substring(2);
                        break;
                    case "s":
                        ((MainActivity) getActivity()).speed = msg.substring(2);
                        tvSpeedVal.setText(((MainActivity) getActivity()).speed);
                        break;
                    case "p":
                        ((MainActivity) getActivity()).prox = msg.substring(2);
                        break;
                    case "m":
                        ((MainActivity) getActivity()).magField = msg.substring(2);
                        break;
                    case "g":
                        ((MainActivity) getActivity()).gear = msg.substring(2);
                        break;
                }
                pendingMsgs.remove(msg);
            }
        }
        
    }
}
