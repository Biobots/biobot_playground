package com.example.biobot.myapplication;

import android.content.ComponentName;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.Handler;
import android.os.IBinder;
import android.os.Looper;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ProgressBar;
import android.widget.Toast;

import static java.lang.System.exit;

public class MainActivity extends AppCompatActivity implements View.OnClickListener, ServiceConnection {

    public static final String EXTRA_MESSAGE = "com.example.myfirstapp.MESSAGE";
    private ProgressBar progressBar;
    private static boolean isExit = false;
    public Button btn4;
    //private boolean serviceOk = false;

    private static Handler mHandler = new Handler(){
        @Override
        public void handleMessage(Message msg)
        {
            super.handleMessage(msg);
            isExit=false;
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        progressBar=(ProgressBar)findViewById(R.id.progressBar);
        btn4=findViewById(R.id.btn4);
        btn4.setOnClickListener(this);
        calThread = new CalThread();
        calThread.start();
    }

    public void sendMessage(View view)
    {
        Intent intent = new Intent(this, DisplayMessageActivity.class);
        EditText editText = findViewById(R.id.editText);
        String message = editText.getText().toString();
        intent.putExtra(EXTRA_MESSAGE, message);
        startActivity(intent);
    }

    public void beginTask(View view)
    {
        MyAsyncTask mat=new MyAsyncTask(progressBar);
        mat.execute(1000);
    }

    public void sendMessages(View view)
    {
        Intent intent = new Intent(this, BundleActivity.class);
        Bundle bundle = new Bundle();
        bundle.putInt("testNum", 1);
        intent.putExtras(bundle);
        startActivity(intent);
    }

    public boolean onKeyDown(int keyCode, KeyEvent keyEvent)
    {
        if (keyCode == KeyEvent.KEYCODE_BACK) {
            if (!isExit) {
                isExit = true;
                Toast.makeText(getApplicationContext(), "再按一次退出程序",
                        Toast.LENGTH_SHORT).show();
                mHandler.sendEmptyMessageDelayed(0, 2000);
            } else {
                finish();
            }
            return false;
        }
        return super.onKeyDown(keyCode, keyEvent);
    }

    @Override
    public void onClick(View v) {
        Toast.makeText(this, "callback", Toast.LENGTH_SHORT).show();
    }

    class CalThread extends Thread
    {
        Handler mHandler;
        /*Handler nHandler = new Handler(){
            @Override
            public void handleMessage(Message msg) {
                super.handleMessage(msg);
                if(msg.what == 0x123)
                {
                    Toast.makeText(MainActivity.this, "aaa", Toast.LENGTH_SHORT).show();
                }
            }
        };*///此处定义运行在主线程

        @Override
        public void run() {
            super.run();
            Looper.prepare();
            mHandler = new Handler(){
                @Override
                public void handleMessage(Message msg) {
                    super.handleMessage(msg);
                    if(msg.what == 0x123)
                    {
                        Log.i("handler", "aaa");
                    }
                }
            }; //此处定义运行在子线程，toast无效
            Looper.loop();
        }
    }

    public void Cal(View view)
    {
        Message msg = new Message();
        msg.what = 0x123;
        calThread.mHandler.sendMessage(msg);
    }

    CalThread calThread;

    MyService.MyBinder binder;

    @Override
    public void onServiceConnected(ComponentName name, IBinder service) {
        binder = (MyService.MyBinder)service;
        binder.setCallback(() -> 2);
        Log.i("service", Integer.toString(binder.getCount()));
        new Thread(new Runnable() {
            @Override
            public void run()
            {
                while (!binder.getStatus())
                {
                    try {
                        Thread.sleep(1000);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                    Log.i("service", Integer.toString(binder.getCount()));
                }
            }
        }).start();
    }

    @Override
    public void onServiceDisconnected(ComponentName name) {
        Log.i("service", "service disconnected");
    }

    public void ConnectService(View view)
    {
        bindService(new Intent(this, MyService.class), this, BIND_AUTO_CREATE);
    }

    public void DisconnectService(View view)
    {
        unbindService(this);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        unbindService(this);
    }

    public void openFragment(View view)
    {
        startActivity(new Intent(this, FragmentActivity.class));
    }

    public void openHttp(View view)
    {
        startActivity(new Intent(this, FragmentActivity.class));
    }
}
