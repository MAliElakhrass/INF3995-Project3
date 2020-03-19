package com.inf3995.PracticeApp.Service;

import android.content.Context;
import android.view.View;
import android.widget.Toast;

import com.inf3995.PracticeApp.Fragment.CustomerOwnMusicList;
import com.inf3995.PracticeApp.Utils.ErrorInfo;

import java.io.IOException;
import java.util.List;

import okhttp3.ResponseBody;
import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;

public class CustomerUploadSongService {
    private Context context;
    private List<String> listRequest;
    private int request;

    public CustomerUploadSongService(Context context, List<String> listRequest) {
        this.context = context;
        this.listRequest = listRequest;
    }

    public void uploadSong(String token) {
        CustomerOwnMusicList.mSpinner.setVisibility(View.VISIBLE);
        CustomerOwnMusicList.mUploadBtn.setEnabled(false);

        request = listRequest.size();

        ServerAPI serverAPI = new ServerService(false, context).getServerAPI();

        for (String base64String: listRequest) {
            serverAPI.uploadSong(token, base64String).enqueue(new Callback<ResponseBody>() {
                @Override
                public void onResponse(Call<ResponseBody> call, Response<ResponseBody> response) {
                    if (--request <= 0) {
                        CustomerOwnMusicList.mSpinner.setVisibility(View.GONE);
                        CustomerOwnMusicList.mUploadBtn.setEnabled(true);
                    }

                    try {
                        if (response.body() != null)
                            Toast.makeText(context, response.body().string(), Toast.LENGTH_SHORT).show();
                        else
                            Toast.makeText(context, response.errorBody().string(), Toast.LENGTH_SHORT).show();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }

                @Override
                public void onFailure(Call<ResponseBody> call, Throwable t) {
                    if (--request <= 0) {
                        CustomerOwnMusicList.mSpinner.setVisibility(View.GONE);
                        CustomerOwnMusicList.mUploadBtn.setEnabled(true);
                    }
                    Toast.makeText(context, ErrorInfo.convertErrorMessage(t.getMessage()), Toast.LENGTH_SHORT).show();
                }
            });
        }
    }

}
