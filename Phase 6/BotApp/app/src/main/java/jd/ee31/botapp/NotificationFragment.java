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

        for (String msg : pendingMsgs) {
            if (msg.substring(0,1).equals("m")) {
                String msgType = msg.substring(1,2);
                switch (msg) {
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
                    case "s":
                        last10Notes.add("Sent " + msg.substring(2) + "ms message\n");
                        break;
                }
                if (last10Notes.size() > 10) last10Notes.remove(0);
                pendingMsgs.remove(msg);
            }
        }
    }

    private void print_messages() {
        for (String msg : last10Notes) {
            String former = (String)msgTV.getText();
            former += '\n';
            former += msg;
            msgTV.setText(former);
        }
    }

}
