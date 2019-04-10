package com.example.biobot.myapplication;

import android.util.Log;

public class Translation
{
    private int status;
    private content content;

    private static class content
    {
        private String from;
        private String to;
        private String vendor;
        private String out;
        private int errNo;
    }

    public void show()
    {
        Log.d("Translation", "show: "+content.to);
    }

}
