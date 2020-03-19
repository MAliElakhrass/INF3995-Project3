// Inspired from https://www.youtube.com/watch?v=094Sv3YrzSU

package com.inf3995.PracticeApp.Adapter;

import android.app.Activity;
import android.support.design.widget.BaseTransientBottomBar;
import android.support.design.widget.Snackbar;
import android.support.v7.widget.RecyclerView;
import android.support.v7.widget.helper.ItemTouchHelper;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;
import android.widget.Toast;

import com.inf3995.PracticeApp.R;

import java.io.IOException;
import java.util.Collections;
import java.util.List;

import com.inf3995.PracticeApp.POJO.PositionChangePOJO;
import com.inf3995.PracticeApp.Service.ServerAPI;
import com.inf3995.PracticeApp.Service.ServerService;
import com.inf3995.PracticeApp.POJO.SongPOJO;
import okhttp3.ResponseBody;
import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;

public class ListSongAdapter extends RecyclerView.Adapter<RecyclerView.ViewHolder> {
    private List<SongPOJO> mList;
    private final Activity mActivity;
    private String mUsername;

    private ItemTouchHelper mTouchHelper;

    public ListSongAdapter(Activity activity, List<SongPOJO> list, ItemTouchHelper ith, String username) {
        mActivity = activity;
        mList = list;
        mTouchHelper = ith;
        mUsername = username;
    }

    @Override
    public RecyclerView.ViewHolder onCreateViewHolder(ViewGroup viewGroup, int viewType) {
        View v = View.inflate(mActivity, R.layout.row_song_recycler_view, null);
        return new MyViewHolder(v);
    }

    @Override
    public void onBindViewHolder(RecyclerView.ViewHolder viewHolder, int i) {
        if (viewHolder instanceof MyViewHolder) {
            final MyViewHolder holder = (MyViewHolder) viewHolder;

            SongPOJO song = mList.get(i);

            holder.songName.setText(song.getTitre());
            holder.songArtist.setText(song.getArtiste());
            holder.songLength.setText(song.getMinutes() + "min " + song.getSeconds() + " sec");
            holder.songIp.setText(song.getIp());
            holder.songMAC.setText(song.getMAC());

            holder.colorBar.setOnTouchListener(new View.OnTouchListener() {
                @Override
                public boolean onTouch(View view, MotionEvent motionEvent) {
                    if (mTouchHelper != null)
                        mTouchHelper.startDrag(holder);

                    return true;
                }
            });
        }
    }

    @Override
    public int getItemCount() {
        return mList.size();
    }

    public void onItemMove(int initialPosition, int finalPosition) {
        if (initialPosition < mList.size() && finalPosition < mList.size()) {
            if (initialPosition < finalPosition) {
                for (int i = initialPosition; i < finalPosition; i++) {
                    Collections.swap(mList, i, i + 1);
                }
            } else {
                for (int i = initialPosition; i > finalPosition; i--) {
                    Collections.swap(mList, i, i - 1);
                }
            }

            notifyItemMoved(initialPosition, finalPosition);
        }

        new Thread(new Runnable() {
            @Override
            public void run() {
                ServerAPI serverAPI = new ServerService(true, mActivity.getApplicationContext()).getServerAPI();
                PositionChangePOJO pos = new PositionChangePOJO(initialPosition, finalPosition);
                serverAPI.changePositionList(mUsername, pos).enqueue(new Callback<ResponseBody>() {
                    @Override
                    public void onResponse(Call<ResponseBody> call, Response<ResponseBody> response) {
                        try {
                            if (response.body() != null) {
                                Toast.makeText(mActivity.getApplicationContext(), response.body().string(), Toast.LENGTH_SHORT).show();
                            } else {
                                Toast.makeText(mActivity.getApplicationContext(), response.errorBody().string(), Toast.LENGTH_SHORT).show();
                            }
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                    }

                    @Override
                    public void onFailure(Call<ResponseBody> call, Throwable t) {
                        Toast.makeText(mActivity.getApplicationContext(), t.getMessage(), Toast.LENGTH_SHORT).show();
                    }
                });

            }
        }).start();
    }

    public void removeItem(int position) {
        Snackbar.make(mActivity.findViewById(R.id.constraint), "Please confirm you want to remove this song", 2500)
                .setAction("Delete", new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {
                        SongPOJO songDelete = mList.get(position);
                        int no = songDelete.getNo();
                        ServerAPI serverAPI = new ServerService(true, mActivity.getApplicationContext()).getServerAPI();
                        serverAPI.deleteSongSup(mUsername, no).enqueue(new Callback<ResponseBody>() {
                            @Override
                            public void onResponse(Call<ResponseBody> call, Response<ResponseBody> response) {
                                try {
                                    if (response.body() != null) {
                                        Toast.makeText(mActivity.getApplicationContext(), response.body().string(), Toast.LENGTH_SHORT).show();
                                        mList.remove(position);
                                        notifyItemRemoved(position);
                                    } else {
                                        Toast.makeText(mActivity.getApplicationContext(), response.errorBody().string(), Toast.LENGTH_SHORT).show();
                                    }
                                } catch (IOException e) {
                                    e.printStackTrace();
                                }
                            }

                            @Override
                            public void onFailure(Call<ResponseBody> call, Throwable t) {
                                Toast.makeText(mActivity.getApplicationContext(), t.getMessage(), Toast.LENGTH_SHORT).show();
                            }
                        });

                    }
                }).addCallback(new BaseTransientBottomBar.BaseCallback<Snackbar>() {
            @Override
            public void onDismissed(Snackbar transientBottomBar, int event) {
                super.onDismissed(transientBottomBar, event);
                notifyItemChanged(position);
            }
        }).show();
    }

    private class MyViewHolder extends RecyclerView.ViewHolder {
        private final TextView songName, songArtist, songLength, songIp, songMAC, colorBar;

        MyViewHolder(View itemView) {
            super(itemView);
            songName = (TextView) itemView.findViewById(R.id.song_name);
            songArtist = (TextView) itemView.findViewById(R.id.song_artist);
            songLength = (TextView) itemView.findViewById(R.id.song_length);
            songIp = (TextView) itemView.findViewById(R.id.song_ip);
            songMAC = (TextView) itemView.findViewById(R.id.song_mac);
            colorBar = (TextView) itemView.findViewById(R.id.color_bar);
        }
    }
}
