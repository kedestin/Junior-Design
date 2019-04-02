package jd.ee31.botapp;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

public class InstructionFragment extends Fragment {
    public static InstructionFragment newInstance() {
        InstructionFragment fragment = new InstructionFragment();
        return fragment;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) { super.onCreate(savedInstanceState);}

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                        Bundle savedInstanceState) {
        View inst_view = inflater.inflate(R.layout.activity_inst, container, false);

        /* Set view elements */

        return inst_view;
    }
}
