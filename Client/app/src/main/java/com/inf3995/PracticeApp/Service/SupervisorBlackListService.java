package com.inf3995.PracticeApp.Service;

import android.content.Context;
import android.widget.Toast;

import com.inf3995.PracticeApp.Activities.SupervisorActivity;
import com.inf3995.PracticeApp.POJO.BlackListUserPOJO;

import java.io.IOException;

import io.reactivex.SingleObserver;
import io.reactivex.android.schedulers.AndroidSchedulers;
import io.reactivex.disposables.Disposable;
import io.reactivex.schedulers.Schedulers;
import okhttp3.ResponseBody;
import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;

public class SupervisorBlackListService {
    private Context context;
    private String username;
    private SupervisorActivity activity;

    public SupervisorBlackListService(Context context, SupervisorActivity activity, String username) {
        this.context = context;
        this.activity = activity;
        this.username = username;
    }

    public void addUserToBlock(BlackListUserPOJO.User user) {
        ServerAPI serverAPI = new ServerService(true, context).getServerAPI();
        serverAPI.blockUser(username, user).enqueue(new Callback<ResponseBody>() {
            @Override
            public void onResponse(Call<ResponseBody> call, Response<ResponseBody> response) {
                try {
                    if (response.body() != null) {
                        Toast.makeText(context, response.body().string(), Toast.LENGTH_SHORT).show();
                        getBlackList();
                    } else {
                        Toast.makeText(context, response.errorBody().string(), Toast.LENGTH_SHORT).show();
                    }
                } catch (IOException e){
                    e.printStackTrace();
                }

            }

            @Override
            public void onFailure(Call<ResponseBody> call, Throwable t) {

            }
        });
    }

    public void getBlackList() {
        ServerAPI serverAPI = new ServerService(true, context).getServerAPI();
        serverAPI.getBlockedUsers(username)
                .subscribeOn(Schedulers.io())
                .observeOn(AndroidSchedulers.mainThread())
                .subscribe(new SingleObserver<BlackListUserPOJO>() {
                    @Override
                    public void onSubscribe(Disposable d) { }

                    @Override
                    public void onSuccess(BlackListUserPOJO blackListUserPOJO) {
                        SupervisorActivity.initListIdentifiedUserRecyclerView(blackListUserPOJO.getUserIdentifiedAsList(), activity);
                        SupervisorActivity.initListBlockedUserRecyclerView(blackListUserPOJO.getAsList(), activity);
                    }

                    @Override
                    public void onError(Throwable e) {

                    }
                });
    }
}
