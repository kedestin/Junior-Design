package jd.ee31.botapp;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.Spinner;

import java.io.OutputStream;

public class InstructionFragment extends Fragment {
    Spinner dropOp;
    Spinner dropDur;
    ArrayAdapter<CharSequence> adapter1, adapter2;

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
        dropDur = inst_view.findViewById(R.id.dropDur);
        dropOp = inst_view.findViewById(R.id.dropOp);
        adapter1 = ArrayAdapter.createFromResource(getActivity(),
                R.array.dropOp, android.R.layout.simple_spinner_dropdown_item);
        adapter2 = ArrayAdapter.createFromResource(getActivity(),
                R.array.dropDur, android.R.layout.simple_spinner_dropdown_item);
        adapter1.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        adapter2.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        dropDur.setAdapter(adapter2);
        dropOp.setAdapter(adapter1);

        return inst_view;
    }

    public void sendMsg(View v) {
        String msg = "";
        switch (dropOp.getSelectedItem().toString()) {
            case "Forward":
                msg += 'f';
                break;
            case "Reverse":
                msg += 'r';
                break;
            case "Left":
                msg += 'l';
                break;
            case "Right":
                msg += 't';
                break;
            case "Honk":
                msg += 'h';
                break;
            case "Send":
                msg += 's';
                break;
        }
        switch (dropDur.getSelectedItem().toString()) {
            case "50ms":
                msg += '0';
                break;
            case "100ms":
                msg += '1';
                break;
            case "200ms":
                msg += '2';
                break;
            case "300ms":
                msg += '3';
                break;
            case "400ms":
                msg += '4';
                break;
            case "500ms":
                msg += '5';
                break;
        }

        OutputStream ostream = ((MainActivity) getActivity()).outStream;
        try {
            if (ostream == null) {
                Log.d("aa", "fuck");
                return;
            };
            ostream.write(msg.getBytes());
        } catch (Exception e) {
            Log.d("hmm", "ok");
        }

    }
}
