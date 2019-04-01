package jd.ee31.botapp;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

public class SensorFragment extends Fragment {

    public static SensorFragment newInstance() {
        SensorFragment fragment = new SensorFragment();
        return fragment;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) { super.onCreate(savedInstanceState);}

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                        Bundle savedInstanceState) {
        View sens_view = inflater.inflate(R.layout.sens_activity, container, false);

        /* Set view elements */

        init_sensor_values();
    }

    private void init_sensor_values() {

    }
}
