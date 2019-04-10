package com.example.biobot.myapplication;

import android.app.Service;
import android.content.Intent;
import android.os.Binder;
import android.os.IBinder;
import android.util.Log;

public class MyService extends Service {

    private int count = 0;
    private boolean quit;
    private String TAG = "service";

    public class MyBinder extends Binder
    {
        public int getCount()
        {
            return count;
        }

        public boolean getStatus()
        {
            return quit;
        }

        public void setCallback(Callback cb)
        {
            callback = cb;
        }
    }

    private MyBinder binder = new MyBinder();

    public interface Callback
    {
        int anyFunc();
    }

    private Callback callback;


    @Override
    public IBinder onBind(Intent intent) {
        return binder;
    }

    @Override
    public void onCreate() {
        super.onCreate();
        new Thread(new Runnable() {
            @Override
            public void run()
            {
                while(!quit)
                {
                    try
                    {
                        Thread.sleep(1000);
                    }
                    catch(InterruptedException e){e.printStackTrace();}
                    count+=callback.anyFunc();
                }
            }
        }).start();
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        Log.i(TAG, "onStartCommand: ");
        return super.onStartCommand(intent, flags, startId);
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        Log.i(TAG, "onDestroy: ");
        quit=true;
    }

    @Override
    public boolean onUnbind(Intent intent) {
        Log.i(TAG, "onUnbind: ");
        return super.onUnbind(intent);
    }
}
