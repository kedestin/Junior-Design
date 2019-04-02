package jd.ee31.botapp;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;

public class NotificationFragment extends Fragment {
    private InputStream inStream;
    private OutputStream outStream;
    private ArrayList<String> pendingMsgs;
    private ArrayList<String> last10Notes;

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

        /* Set view elements */
        handle_pending();
        print_messages();


        return note_view;
    }


    private void handle_pending() {
        inStream = ((MainActivity) getActivity()).inStream;
        outStream = ((MainActivity) getActivity()).outStream;
        pendingMsgs = ((MainActivity) getActivity()).pendingMsgs;
        last10Notes = ((MainActivity) getActivity()).last10Notes;

        for (String msg : pendingMsgs) {
            if (msg.substring(0, 3).equals("Msg")) {
                String msgType = msg.substring(3, 6);
                switch (msg) {
                    case "col":
                        switch (msg.substring(6)) {
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
                    case "rec":
                        last10Notes.add("Received " + msg.substring(6) + "ms message\n");
                        break;
                    case "sen":
                        last10Notes.add("Sent " + msg.substring(6) + "ms message\n");
                        break;
                }
                if (last10Notes.size() > 10) last10Notes.remove(0);
                pendingMsgs.remove(msg);
            }
        }
    }

    private void print_messages() {
        String tv;
        for (String msg : last10Notes) {
            /*Get TextView and append to text.*/
        }
    }

}
