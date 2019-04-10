package com.example.biobot.myapplication;

import android.content.Context;
import android.net.Uri;
import android.os.Bundle;
import android.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import static android.content.ContentValues.TAG;


public class TestFragment extends Fragment {


    public TestFragment() {
        fragFunc = new FragFunc() {
            @Override
            public void Callback(int num) {
                Log.d(TAG, "Callback: "+num);
            }
        };
    }


    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        fragFunc.Callback(1);
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        return inflater.inflate(R.layout.fragment_test, container, false);
    }


    @Override
    public void onAttach(Context context) {
        super.onAttach(context);
    }

    @Override
    public void onDetach() {
        super.onDetach();
    }


    public interface FragFunc
    {
        public void Callback(int num);
    }

    public FragFunc fragFunc;
}
