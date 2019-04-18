package jd.ee31.botapp;

import android.graphics.Color;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.Iterator;

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

        for (Iterator<String> iterator = pendingMsgs.iterator(); ((Iterator) iterator).hasNext();) {
            String msg = iterator.next();
            if (msg.substring(0,1).equals("s")) {
                String sens = msg.substring(1,2);
                switch (sens) {
                    case "c":
                        ((MainActivity) getActivity()).currColor = msg.substring(2);
                        break;
                    case "s":
                        ((MainActivity) getActivity()).speed = msg.substring(2);
                        break;
                    case "p":
                        ((MainActivity) getActivity()).prox = msg.substring(2);
                        break;
                    case "a":
                        ((MainActivity) getActivity()).magField = msg.substring(2);
                        break;
                    case "g":
                        ((MainActivity) getActivity()).gear = msg.substring(2);
                        break;
                }
            }
        }
        pendingMsgs.remove(pendingMsgs.contains("m"));
        print_values();
    }

    private void print_values() {
        tvSpeedVal.setText(((MainActivity) getActivity()).speed);
        tvProxVal.setText(((MainActivity) getActivity()).prox);
        switch (((MainActivity) getActivity()).magField) {
            case "000":
                tvMagVal.setText("No");
                break;
            case "111":
                tvMagVal.setText("Yes!");
                break;
        }
        switch (((MainActivity) getActivity()).currColor) {
            case "000":
                tvColorVal.setText("Black");
                tvColorVal.setTextColor(Color.BLACK);
                break;
            case "001":
                tvColorVal.setText("Blue");
                tvColorVal.setTextColor(Color.BLUE);
                break;
            case "010":
                tvColorVal.setText("Red");
                tvColorVal.setTextColor(Color.RED);
                break;
            case "100":
                tvColorVal.setText("Yellow");
                tvColorVal.setTextColor(Color.YELLOW);
                break;
        }
        switch (((MainActivity) getActivity()).gear) {
            case "000":
                tvGearVal.setText("Park");
                break;
            case "001":
                tvGearVal.setText("Neutral");
                break;
            case "010":
                tvGearVal.setText("Drive");
                break;
            case "100":
                tvGearVal.setText("Reverse");
                break;
        }
    }
}
