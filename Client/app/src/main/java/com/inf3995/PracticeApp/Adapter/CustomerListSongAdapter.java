package com.inf3995.PracticeApp.Adapter;

import android.app.Activity;
import android.graphics.Color;
import android.support.design.widget.BaseTransientBottomBar;
import android.support.design.widget.Snackbar;
import android.support.v7.widget.RecyclerView;
import android.support.v7.widget.helper.ItemTouchHelper;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import com.inf3995.PracticeApp.Fragment.CustomerServerMusicList;
import com.inf3995.PracticeApp.POJO.SongPOJO;
import com.inf3995.PracticeApp.R;
import com.inf3995.PracticeApp.Service.CustomerSongListService;
import com.inf3995.PracticeApp.Service.ServerAPI;
import com.inf3995.PracticeApp.Service.ServerService;

import java.io.IOException;
import java.util.List;

import okhttp3.ResponseBody;
import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;

public class CustomerListSongAdapter extends RecyclerView.Adapter<RecyclerView.ViewHolder> {
    private List<SongPOJO> mList;
    private final Activity mActivity;
    private String mToken;
    private ItemTouchHelper mTouchHelper;
    private String mNom;
    private CustomerServerMusicList mFragmentServerList;

    public CustomerListSongAdapter(Activity activity, List<SongPOJO> list, ItemTouchHelper ith, String token, String nom,
                                   CustomerServerMusicList fragmentServerList) {
        mActivity = activity;
        mList = list;
        mTouchHelper = ith;
        mToken = token;
        mNom = nom;
        mFragmentServerList = fragmentServerList;
    }

    @Override
    public RecyclerView.ViewHolder onCreateViewHolder(ViewGroup viewGroup, int viewType) {
        View v = View.inflate(mActivity, R.layout.row_song_customer_recycler_view, null);
        return new CustomerListSongAdapter.MyViewHolder(v);
    }

    @Override
    public void onBindViewHolder(RecyclerView.ViewHolder viewHolder, int i) {
        if (viewHolder instanceof CustomerListSongAdapter.MyViewHolder) {
            final CustomerListSongAdapter.MyViewHolder holder = (CustomerListSongAdapter.MyViewHolder) viewHolder;

            SongPOJO song = mList.get(i);

            holder.songName.setText(song.getTitre());
            holder.songArtist.setText(song.getArtiste());
            holder.songLength.setText(song.getMinutes() + "min " + song.getSeconds() + " sec");
            if (song.getProposeePar().equals(mNom)) {
                holder.songName.setTextColor(Color.RED);
                holder.songArtist.setTextColor(Color.RED);
                holder.songLength.setTextColor(Color.RED);
            }

            if (song.getTitre().equals("Tom's Diner")) {
                holder.albumArt.setImageResource(R.drawable.suzanne_vega);
            }

            holder.albumArt.setOnLongClickListener(new View.OnLongClickListener() {
                @Override
                public boolean onLongClick(View v) {
                    CustomerSongListService songListService = new CustomerSongListService(mActivity, mToken, mNom, mFragmentServerList);
                    songListService.getLyrics(song);
                    return true;
                }
            });
        }
    }

    @Override
    public int getItemCount() {
        return mList.size();
    }

    public void removeItem(int position) {
        Snackbar.make(mActivity.findViewById(R.id.customer_constraint), "Please confirm you want to remove this song", 2500)
                .setAction("Delete", new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {
                        SongPOJO songDelete = mList.get(position);
                        int no = songDelete.getNo();
                        ServerAPI serverAPI = new ServerService(false, mActivity.getApplicationContext()).getServerAPI();
                        serverAPI.deleteSongUser(mToken, no).enqueue(new Callback<ResponseBody>() {
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
        private final TextView songName, songArtist, songLength;
        private final ImageView albumArt;

        MyViewHolder(View itemView) {
            super(itemView);
            songName = (TextView) itemView.findViewById(R.id.textView_customer_song_title);
            songArtist = (TextView) itemView.findViewById(R.id.textView_customer_artist);
            songLength = (TextView) itemView.findViewById(R.id.textView_customer_length);
            albumArt = (ImageView) itemView.findViewById(R.id.imageView_Customer_AlbumArt);
        }
    }
}
