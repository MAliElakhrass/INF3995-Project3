package com.inf3995.PracticeApp.Service;

import android.content.Context;
import android.view.View;
import android.widget.Toast;

import com.inf3995.PracticeApp.Activities.MainActivity;
import com.inf3995.PracticeApp.POJO.SupervisorIdentificationPOJO;
import com.inf3995.PracticeApp.Utils.ErrorInfo;

import java.io.IOException;

import okhttp3.ResponseBody;
import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;

public class SupervisorIdentificationService {
    private Context context;
    private MainActivity mainActivity;

    public SupervisorIdentificationService(Context context, MainActivity activity) {
        this.context = context;
        mainActivity = activity;
    }

    public void login(View view, String userName, String password) {
        ServerAPI serverAPI = new ServerService(true, context).getServerAPI();
        SupervisorIdentificationPOJO identificationPOJO = new SupervisorIdentificationPOJO(userName, password);
        serverAPI.loginSupervisor(identificationPOJO).enqueue(new Callback<ResponseBody>() {
            @Override
            public void onResponse(Call<ResponseBody> call, Response<ResponseBody> response) {
                try {
                    if (response.body() != null) {
                        Toast.makeText(context, response.body().string(), Toast.LENGTH_SHORT).show();
                        mainActivity.launchSupervisorActivity(view);
                        MainActivity.ad.dismiss();
                    } else {
                        Toast.makeText(context, response.errorBody().string(), Toast.LENGTH_SHORT).show();
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }

            @Override
            public void onFailure(Call<ResponseBody> call, Throwable t) {
                Toast.makeText(context, ErrorInfo.convertErrorMessage(t.getMessage()), Toast.LENGTH_SHORT).show();
            }
        });
    }
}
