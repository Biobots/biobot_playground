package com.example.biobot.myapplication;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;

public class FragmentActivity extends AppCompatActivity {

    TestFragment.FragFunc fragFunc;
    TestFragment testFragment;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_fragment);
        testFragment = (TestFragment)getFragmentManager().findFragmentById(R.id.fragmentTest);
        testFragment.fragFunc = new TestFragment.FragFunc() {
            @Override
            public void Callback(int num) {
                Log.d("a", "Callback: "+(num+1));
            }
        };
    }
}
