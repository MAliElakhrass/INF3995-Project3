package com.inf3995.PracticeApp.Fragment;

import android.app.Dialog;
import android.os.Bundle;
import android.support.design.widget.FloatingActionButton;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.support.v7.widget.helper.ItemTouchHelper;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.Window;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import com.inf3995.PracticeApp.Activities.CustomerActivity;
import com.inf3995.PracticeApp.Adapter.CustomerListSongAdapter;
import com.inf3995.PracticeApp.Listener.CustomerDynamicEventsHelper;
import com.inf3995.PracticeApp.POJO.SongPOJO;
import com.inf3995.PracticeApp.R;
import com.inf3995.PracticeApp.Service.CustomerSongListService;

import java.util.List;

public class CustomerServerMusicList extends Fragment {

    private RecyclerView mRecyclerView;
    private static String mToken;
    private CustomerListSongAdapter mAdapter;
    private SongPOJO mCurrentSong;
    private ImageView imgView;
    private CustomerSongListService songListService;

    public static CustomerServerMusicList newInstance(String token) {
        mToken = token;

        return new CustomerServerMusicList();
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        View rootView = inflater.inflate(R.layout.layout_customer_list_songs, container, false);

        return rootView;
    }

    @Override
    public void onViewCreated(View view, Bundle savedInstance) {
        mRecyclerView = (RecyclerView) view.findViewById(R.id.recyclerView_customer_list_songs);
        mRecyclerView.setNestedScrollingEnabled(false);

        FloatingActionButton helpButton = view.findViewById(R.id.help_customer_list_song);
        helpButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Dialog d = new Dialog(getContext());
                d.requestWindowFeature(Window.FEATURE_NO_TITLE);
                d.setContentView(R.layout.dialog_help_customer_playlist);
                // Button thankyou = (Button) d.findViewById(R.id.thankyou);
                d.show();
            }
        });

        mToken = ((CustomerActivity) getActivity()).getToken();
        String customerName = ((CustomerActivity) getActivity()).mUser;
        songListService = new CustomerSongListService(getContext(), mToken, customerName, this);

        imgView = (ImageView) getActivity().findViewById(R.id.imageView_Customer_AlbumArt_cs);
        imgView.setOnLongClickListener(new View.OnLongClickListener() {
            @Override
            public boolean onLongClick(View v) {
                if (mCurrentSong != null)
                    songListService.getLyrics(mCurrentSong);
                else
                    Toast.makeText(getContext(), "There is no song currently playing!", Toast.LENGTH_SHORT).show();
                return false;
            }
        });

    }

    public void getSongsList() {
        mToken = ((CustomerActivity) getActivity()).getToken();
        String customerName = ((CustomerActivity) getActivity()).mUser;
        songListService = new CustomerSongListService(getContext(), mToken, customerName, this);
        songListService.getSongList();
    }

    public void initCurrentSongView(SongPOJO song) {
        mCurrentSong = song;

        TextView textViewTitle = (TextView) getActivity().findViewById(R.id.textView_customer_song_title_cs);
        TextView textViewArtist = (TextView) getActivity().findViewById(R.id.textView_customer_artist_cs);
        TextView textViewLength = (TextView) getActivity().findViewById(R.id.textView_customer_length_cs);
        if (song.getTitre().equals("Tom's Diner"))
            imgView.setImageResource(R.drawable.suzanne_vega);
        else
            imgView.setImageResource(R.drawable.mp3);
        textViewTitle.setText(song.getTitre());
        textViewArtist.setText(song.getArtiste());
        textViewLength.setText(song.getMinutes() + " minutes " + song.getSeconds() + " seconds");
    }

    public void setLyricsView(String lyrics) {
        AppCompatActivity activity = (AppCompatActivity) getActivity();
        FragmentManager fm = activity.getSupportFragmentManager();
        LyricsFragment lyricsFragment = LyricsFragment.newInstance(lyrics);
        lyricsFragment.show(fm, "lyrics_fragment");
    }

    public void initListSongRecyclerView(List<SongPOJO> list, String nom) {
        LinearLayoutManager mLayoutManager = new LinearLayoutManager(getContext());
        mLayoutManager.setOrientation(LinearLayoutManager.VERTICAL);
        mRecyclerView.setLayoutManager(mLayoutManager);

        CustomerDynamicEventsHelper.DynamicEventsCallback callback = new CustomerDynamicEventsHelper.DynamicEventsCallback() {
            @Override
            public void removeItem(int position) {
                mAdapter.removeItem(position);
            }
        };

        ItemTouchHelper itemTouchHelper = new ItemTouchHelper(new CustomerDynamicEventsHelper(callback));
        itemTouchHelper.attachToRecyclerView(mRecyclerView);

        mAdapter = new CustomerListSongAdapter(getActivity(), list, itemTouchHelper, mToken, nom, this);
        mRecyclerView.setAdapter(mAdapter);
    }
}
