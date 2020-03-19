package com.inf3995.PracticeApp.Service;

import com.inf3995.PracticeApp.POJO.SongPOJO;

import java.io.IOException;
import java.util.concurrent.TimeUnit;

import okhttp3.Call;
import okhttp3.Callback;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.Response;

public class SongCover {
    private static final String LAST_FM_API =
            "http://ws.audioscrobbler.com/2.0//2.0/?method=track.getInfo&api_key=d5d96d9562f8467ba8cd252c25f9cc12&artist=";

    public static void getImage(SongPOJO song) {
        Request request = new Request.Builder()
                .url(getURL(song.getArtiste(), song.getTitre()))
                .build();

        OkHttpClient client = new OkHttpClient.Builder()
                .readTimeout(5, TimeUnit.SECONDS)
                .build();

        client.newCall(request).enqueue(new Callback() {
            @Override
            public void onResponse(Call call, Response response) throws IOException {
                if (response.body() != null) {
                    
                }
            }

            @Override
            public void onFailure(Call call, IOException e) {
                e.printStackTrace();
            }
        });
    }

    private static String getURL(String artist, String name) {
        return LAST_FM_API + artist + "&track=" + name + "&format=json";
    }
}
