package com.inf3995.PracticeApp.Service;

import android.content.Context;

import com.inf3995.PracticeApp.Activities.SupervisorActivity;
import com.inf3995.PracticeApp.POJO.ListSongPOJO;
import com.inf3995.PracticeApp.POJO.SongPOJO;

import io.reactivex.SingleObserver;
import io.reactivex.android.schedulers.AndroidSchedulers;
import io.reactivex.disposables.Disposable;
import io.reactivex.schedulers.Schedulers;

public class SupervisorSongListService {
    private String username;
    private Context context;

    public SupervisorSongListService(Context context, String username) {
        this.context = context;
        this.username = username;
    }

    public void getSongList() {
        ServerAPI serverAPI = new ServerService(true, context).getServerAPI();
        serverAPI.getMusicListSupervisor(username)
                .subscribeOn(Schedulers.io())
                .observeOn(AndroidSchedulers.mainThread())
                .subscribe(new SingleObserver<ListSongPOJO>() {
                    @Override
                    public void onSubscribe(Disposable d) { }

                    @Override
                    public void onSuccess(ListSongPOJO listSongPOJO) {
                        SongPOJO currentSong = listSongPOJO.getCurrentSong();
                        if (currentSong != null)
                            SupervisorActivity.initCurrentSong(listSongPOJO.getCurrentSong());
                        if (listSongPOJO.getAsList() != null)
                            SupervisorActivity.initListSongRecyclerView(listSongPOJO.getAsList(), context);
                    }

                    @Override
                    public void onError(Throwable e) {
                        System.out.println(e.getMessage());
                    }
                });
    }
}
