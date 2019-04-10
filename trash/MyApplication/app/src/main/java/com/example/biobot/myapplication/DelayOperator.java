package com.example.biobot.myapplication;

public class DelayOperator
{
    public void Delay()
    {
        try
        {
            Thread.sleep(1000);
        }
        catch (InterruptedException e)
        {
            e.printStackTrace();
        }
    }
}
