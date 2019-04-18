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

        /* Set view elements */
        handle_pending();
        print_messages();


        return note_view;
    }


    public void handle_pending() {
        pendingMsgs = ((MainActivity) getActivity()).pendingMsgs;
        last10Notes = ((MainActivity) getActivity()).last10Notes;

        for (Iterator<String> iterator = pendingMsgs.iterator(); ((Iterator) iterator).hasNext();) {
            String msg = iterator.next();
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
                if (last10Notes.size() > 20) last10Notes.remove(0);
            }
        }
        pendingMsgs.remove(pendingMsgs.contains("m"));
        print_messages();
    }

    private void print_messages() {
        String output = "";
        for (String msg : last10Notes) {
            output += msg;
        }
        msgTV.setText(output);
    }

}
