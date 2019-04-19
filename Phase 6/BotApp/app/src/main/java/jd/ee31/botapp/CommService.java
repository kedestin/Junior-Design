package jd.ee31.botapp;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;

public class CommService extends Service {

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        return Service.START_STICKY;
    }

    public void onCreate() {

    }

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

}
