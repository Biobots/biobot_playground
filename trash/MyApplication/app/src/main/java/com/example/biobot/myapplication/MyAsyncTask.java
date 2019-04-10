package com.example.biobot.myapplication;

import android.os.AsyncTask;
import android.widget.ProgressBar;

import com.example.biobot.myapplication.DelayOperator;


public class MyAsyncTask extends AsyncTask<Integer, Integer, String>
{
    private ProgressBar progressBar;

    public MyAsyncTask(ProgressBar progressBar)
    {
        super();
        this.progressBar = progressBar;
    }

    @Override
    protected String doInBackground(Integer[] ints) {
        DelayOperator dop = new DelayOperator();
        int i = 0;
        for(i=10;i<=100;i+=10)
        {
            dop.Delay();
            publishProgress(i);
        }
        return  i + ints[0] + "";
    }

    @Override
    protected void onPreExecute() {
        super.onPreExecute();
    }

    @Override
    protected void onProgressUpdate(Integer[] values) {
        super.onProgressUpdate(values);
        int value = values[0];
        progressBar.setProgress(value);
    }
}
