package jd.ee31.botapp;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.Iterator;

public class NotificationFragment extends Fragment {
    private InputStream inStream;
    private OutputStream outStream;
    private ArrayList<String> pendingMsgs;
    private ArrayList<String> last10Notes;
    private TextView msgTV;

    public static NotificationFragment newInstance() {
        NotificationFragment fragment = new NotificationFragment();
        return fragment;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) { super.onCreate(savedInstanceState);}

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        View note_view = inflater.inflate(R.layout.activity_note, container, false);
        msgTV = note_view.findViewById(R.id.msgTV);


        return note_view;
    }

    public void print_messages() {
        last10Notes = ((MainActivity) getActivity()).last10Notes;
        String output = "";
        for (String msg : last10Notes) {
            output += msg;
        }
        msgTV.setText(output);
    }

}
