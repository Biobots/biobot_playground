package com.example.biobot.myapplication;

import retrofit2.Call;
import retrofit2.http.GET;

public interface GetRequest_Interface
{
    @GET("ajax.php?a=fy&f=auto&t=auto&w=hello%20world")
    Call<Translation> getCall();
}
