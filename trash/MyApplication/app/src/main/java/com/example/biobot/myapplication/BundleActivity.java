package com.example.biobot.myapplication;

import android.content.Intent;
import android.os.Bundle;
import android.app.Activity;
import android.os.PersistableBundle;

public class BundleActivity extends Activity {

    private int testNum;

    @Override
    public void onCreate(Bundle savedInstanceState, PersistableBundle persistentState) {
        super.onCreate(savedInstanceState, persistentState);
        setContentView(R.layout.activity_bundle);

        Intent intent = getIntent();
        Bundle bd = intent.getExtras();
        testNum = (bd != null ? bd.getInt("testNum") : 0);

    }

    @Override
    public void onSaveInstanceState(Bundle outState, PersistableBundle outPersistentState) {
        super.onSaveInstanceState(outState, outPersistentState);
        outState.putInt("testNum", 2);
    }

    @Override
    public void onRestoreInstanceState(Bundle savedInstanceState, PersistableBundle persistentState) {
        super.onRestoreInstanceState(savedInstanceState, persistentState);
        testNum=savedInstanceState.getInt("testNum");

    }
}
