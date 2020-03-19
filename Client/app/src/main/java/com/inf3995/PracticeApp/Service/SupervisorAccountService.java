package com.inf3995.PracticeApp.Service;

import android.content.Context;
import android.widget.Toast;

import com.inf3995.PracticeApp.Activities.SupervisorActivity;
import com.inf3995.PracticeApp.POJO.SupervisorIdentificationPOJO;

import java.io.IOException;

import okhttp3.ResponseBody;
import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;

public class SupervisorAccountService {
    private Context context;
    private String username;
    private SupervisorActivity activity;

    public SupervisorAccountService(Context context, String username, SupervisorActivity supervisorActivity) {
        this.context = context;
        this.username = username;
        this.activity = supervisorActivity;
    }

    public void logout() {
        ServerAPI serverAPI = new ServerService(true, context).getServerAPI();
        serverAPI.logoutSupervisor(username).enqueue(new Callback<ResponseBody>() {
            @Override
            public void onResponse(Call<ResponseBody> call, Response<ResponseBody> response) {
                try{
                    if (response.body() != null) {
                        Toast.makeText(context, response.body().string(), Toast.LENGTH_SHORT).show();
                        activity.finish();
                    } else {
                        Toast.makeText(context, response.errorBody().string(), Toast.LENGTH_SHORT).show();
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }

            @Override
            public void onFailure(Call<ResponseBody> call, Throwable t) {
                t.printStackTrace();
            }
        });
    }

    public void changePassword(SupervisorIdentificationPOJO.NewPasswordPOJO newPwdPOJO, Context context, String newPassword) {
        ServerAPI serverAPI = new ServerService(true, context).getServerAPI();
        serverAPI.changeSupervisorPassword(username, newPwdPOJO).enqueue(new Callback<ResponseBody>() {
            @Override
            public void onResponse(Call<ResponseBody> call, Response<ResponseBody> response) {
                try {
                    if (response.body() != null) {
                        SupervisorActivity.password = newPassword;
                        Toast.makeText(context, response.body().string(), Toast.LENGTH_SHORT).show();
                        SupervisorActivity.adChangePwd.dismiss();
                    } else {
                        Toast.makeText(context, response.errorBody().string(), Toast.LENGTH_SHORT).show();
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }

            @Override
            public void onFailure(Call<ResponseBody> call, Throwable t) {
                t.printStackTrace();
            }
        });
    }
}
