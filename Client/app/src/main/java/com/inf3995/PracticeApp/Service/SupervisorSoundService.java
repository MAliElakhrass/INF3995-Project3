package com.inf3995.PracticeApp.Service;

import android.content.Context;
import android.widget.Toast;

import com.inf3995.PracticeApp.Activities.SupervisorActivity;
import com.inf3995.PracticeApp.POJO.SoundPOJO;
import com.inf3995.PracticeApp.R;

import java.io.IOException;

import io.reactivex.SingleObserver;
import io.reactivex.android.schedulers.AndroidSchedulers;
import io.reactivex.disposables.Disposable;
import io.reactivex.schedulers.Schedulers;
import okhttp3.ResponseBody;
import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;

public class SupervisorSoundService {
    private Context context;
    private String username;

    public SupervisorSoundService(Context context, String username) {
        this.context = context;
        this.username = username;
    }

    public void getVolume() {
        ServerAPI serverAPI = new ServerService(true, context).getServerAPI();
        serverAPI.getVolume(username)
                .subscribeOn(Schedulers.io())
                .observeOn(AndroidSchedulers.mainThread())
                .subscribe(new SingleObserver<SoundPOJO>() {
                    @Override
                    public void onSubscribe(Disposable d) { }

                    @Override
                    public void onSuccess(SoundPOJO soundPOJO) {
                        SupervisorActivity.progress = (int) soundPOJO.getVolume();
                        SupervisorActivity.mSeekbar.setProgress(SupervisorActivity.progress);
                        SupervisorActivity.isMuted = soundPOJO.isSourdine();
                        if (SupervisorActivity.isMuted) {
                            SupervisorActivity.currentImage = 1;
                            SupervisorActivity.mLowImgView.setImageResource(R.drawable.ic_sourdine);
                        }
                    }

                    @Override
                    public void onError(Throwable e) {
                        Toast.makeText(context, e.getMessage(), Toast.LENGTH_SHORT).show();
                        e.printStackTrace();
                    }
                });
    }

    public void mute() {
        ServerAPI serverAPI = new ServerService(true, context).getServerAPI();
        serverAPI.enableMute(username).enqueue(new Callback<ResponseBody>() {
            @Override
            public void onResponse(Call<ResponseBody> call, Response<ResponseBody> response) { }

            @Override
            public void onFailure(Call<ResponseBody> call, Throwable t) { }
        });
    }

    public void unmute() {
        ServerAPI serverAPI = new ServerService(true, context).getServerAPI();
        serverAPI.disableMute(username).enqueue(new Callback<ResponseBody>() {
            @Override
            public void onResponse(Call<ResponseBody> call, Response<ResponseBody> response) { }

            @Override
            public void onFailure(Call<ResponseBody> call, Throwable t) { }
        });
    }

    public void increaseVolume(int pc, int newProgress) {
        ServerAPI serverAPI = new ServerService(true, context).getServerAPI();
        serverAPI.increaseVolume(username, Integer.toString(pc)).enqueue(new Callback<ResponseBody>() {
            @Override
            public void onResponse(Call<ResponseBody> call, Response<ResponseBody> response) {
                try {
                    if (response.body() != null) {
                        Toast.makeText(context, response.body().string(), Toast.LENGTH_SHORT).show();
                        SupervisorActivity.progress = newProgress;
                    } else {
                        Toast.makeText(context, response.errorBody().string(), Toast.LENGTH_SHORT).show();
                        SupervisorActivity.mSeekbar.setProgress(SupervisorActivity.progress);
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }

            @Override
            public void onFailure(Call<ResponseBody> call, Throwable t) {
                SupervisorActivity.mSeekbar.setProgress(SupervisorActivity.progress);
                t.printStackTrace();
            }
        });
    }

    public void decreaseVolume(int pc, int newProgress) {
        ServerAPI serverAPI = new ServerService(true, context).getServerAPI();
        serverAPI.decreaseVolume(username, Integer.toString(pc)).enqueue(new Callback<ResponseBody>() {
            @Override
            public void onResponse(Call<ResponseBody> call, Response<ResponseBody> response) {
                try {
                    if (response.body() != null) {
                        Toast.makeText(context, response.body().string(), Toast.LENGTH_SHORT).show();
                        SupervisorActivity.progress = newProgress;
                    } else {
                        Toast.makeText(context, response.errorBody().string(), Toast.LENGTH_SHORT).show();
                        SupervisorActivity.mSeekbar.setProgress(SupervisorActivity.progress);
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }

            @Override
            public void onFailure(Call<ResponseBody> call, Throwable t) {
                SupervisorActivity.mSeekbar.setProgress(SupervisorActivity.progress);
                t.printStackTrace();
            }
        });
    }
}
