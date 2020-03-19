package com.inf3995.PracticeApp.Activities;

import android.app.Activity;
import android.content.Context;
import android.os.Build;
import android.os.Bundle;
import android.support.annotation.RequiresApi;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.NavigationView;
import android.support.v4.view.GravityCompat;
import android.support.v4.widget.DrawerLayout;
import android.support.v7.app.ActionBarDrawerToggle;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.support.v7.widget.Toolbar;
import android.support.v7.widget.helper.ItemTouchHelper;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.SeekBar;
import android.widget.TextView;

import java.util.List;

import com.inf3995.PracticeApp.Adapter.BlockedUserAdapter;
import com.inf3995.PracticeApp.Adapter.IdentifiedUserAdapter;
import com.inf3995.PracticeApp.Listener.DynamicEventsHelper;
import com.inf3995.PracticeApp.POJO.BlackListUserPOJO;
import com.inf3995.PracticeApp.Adapter.ListSongAdapter;
import com.inf3995.PracticeApp.POJO.SupervisorIdentificationPOJO;
import com.inf3995.PracticeApp.R;
import com.inf3995.PracticeApp.POJO.SongPOJO;
import com.inf3995.PracticeApp.Service.SupervisorAccountService;
import com.inf3995.PracticeApp.Service.SupervisorBlackListService;
import com.inf3995.PracticeApp.Service.SupervisorSongListService;
import com.inf3995.PracticeApp.Service.SupervisorSoundService;

public class SupervisorActivity extends AppCompatActivity
        implements NavigationView.OnNavigationItemSelectedListener {

    private static String username;
    public static String password;

    private LinearLayout mLayoutListSongs;
    private RelativeLayout mLayoutListBlockedUsers;
    private RelativeLayout mLayoutSound;

    private static RecyclerView mRecyclerView;
    private static ListSongAdapter mAdapter;
    private static RecyclerView mBURecyclerView;
    private static BlockedUserAdapter mBUAdapter;
    private static RecyclerView mIdRecyclerView;
    private static IdentifiedUserAdapter mIdAdapter;

    public static SeekBar mSeekbar;
    public static ImageView mLowImgView;
    public static int progress = 0;
    public static boolean isMuted = false;
    public static int currentImage = 0;

    private static TextView songName, songArtist, songLength, songIp, songMAC;

    public static AlertDialog adChangePwd;

    private SupervisorBlackListService blackListService;
    private SupervisorAccountService accountService;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_supervisor);

        username = getIntent().getStringExtra("UserName");
        password = getIntent().getStringExtra("Password");

        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        ActionBarDrawerToggle toggle = new ActionBarDrawerToggle(
                this, drawer, toolbar, R.string.navigation_drawer_open, R.string.navigation_drawer_close);
        drawer.addDrawerListener(toggle);
        toggle.syncState();

        NavigationView navigationView = (NavigationView) findViewById(R.id.nav_view);
        navigationView.setNavigationItemSelectedListener(this);

        initVariable();
        initSupervisorActivity();
    }

    @Override
    public void onBackPressed() {
        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        if (drawer.isDrawerOpen(GravityCompat.START)) {
            drawer.closeDrawer(GravityCompat.START);
        } else {
            super.onBackPressed();
        }
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    @SuppressWarnings("StatementWithEmptyBody")
    @Override
    public boolean onNavigationItemSelected(MenuItem item) {
        // Handle navigation view item clicks here.
        int id = item.getItemId();

        if (id == R.id.nav_song_list) {
            getSupervisorList();
        } else if (id == R.id.nav_black_list) {
            getBlackList();
        } else if (id == R.id.nav_changement_mdp) {
            changePassword(this.getCurrentFocus(), this.getLayoutInflater());
        } else if (id == R.id.nav_volume) {
            setSound();
        } else if (id == R.id.nav_logout) {
            accountService.logout();
        }

        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        drawer.closeDrawer(GravityCompat.START);
        return true;
    }

    private void initVariable() {
        songName = (TextView) findViewById(R.id.song_name_current);
        songArtist = (TextView) findViewById(R.id.song_artist_current);
        songLength = (TextView) findViewById(R.id.song_length_current);
        songIp = (TextView) findViewById(R.id.song_ip_current);
        songMAC = (TextView) findViewById(R.id.song_mac_current);

        mBURecyclerView = (RecyclerView) findViewById(R.id.blocked_user_recycler_view);
        mRecyclerView = (RecyclerView) findViewById(R.id.list_music_recycler_view);
        mIdRecyclerView = (RecyclerView) findViewById(R.id.identified_user_recycler_view);

        blackListService = new SupervisorBlackListService(getApplicationContext(), this, username);
        accountService = new SupervisorAccountService(getApplicationContext(), username, this);
    }

    private void initSupervisorActivity() {
        mLayoutListSongs = (LinearLayout) findViewById(R.id.layout_list_songs);
        mLayoutListSongs.setVisibility(View.GONE);

        mLayoutListBlockedUsers = (RelativeLayout) findViewById(R.id.layout_list_blocked_user);
        mLayoutListBlockedUsers.setVisibility(View.GONE);

        mLayoutSound = (RelativeLayout) findViewById(R.id.layout_sound);
        mLayoutSound.setVisibility(View.GONE);

        mSeekbar = (SeekBar) findViewById(R.id.seekBarSound);
        mSeekbar.setMax(100);
        mLowImgView = (ImageView) findViewById(R.id.low_sound_imageView);
    }

    private void getSupervisorList() {
        initSupervisorActivity();
        mLayoutListSongs.setVisibility(View.VISIBLE);

        SupervisorSongListService songListService = new SupervisorSongListService(this, username);
        songListService.getSongList();
    }

    private void getBlackList() {
        initSupervisorActivity();
        mLayoutListBlockedUsers.setVisibility(View.VISIBLE);

        blackListService.getBlackList();
    }

    private void setSound() {
        initSupervisorActivity();
        mLayoutSound.setVisibility(View.VISIBLE);

        SupervisorSoundService soundService = new SupervisorSoundService(getApplicationContext(), username);
        soundService.getVolume();

        mLowImgView.setOnClickListener(new View.OnClickListener() {
            @RequiresApi(api = Build.VERSION_CODES.LOLLIPOP)
            @Override
            public void onClick(View v) {
                int[] images = {R.drawable.ic_low, R.drawable.ic_sourdine};
                currentImage++;
                currentImage = currentImage % 2;
                mLowImgView.setImageResource(images[currentImage]);

                if (currentImage == 1)
                    soundService.mute();
                else
                    soundService.unmute();

            }
        });

        mSeekbar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) { }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) { }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                int newProgress = seekBar.getProgress();

                if (newProgress > progress) {
                    int pc = newProgress - progress;
                    soundService.increaseVolume(pc, newProgress);
                } else if (progress > newProgress) {
                    int pc = progress - newProgress;
                    soundService.decreaseVolume(pc, newProgress);
                }
            }
        });
    }

    private void changePassword(View v, LayoutInflater inflater) {
        initSupervisorActivity();
        v = inflater.inflate(R.layout.change_pwd_dialog, null);

        adChangePwd = new AlertDialog.Builder(v.getContext())
                .setTitle("Change password!")
                .setView(v)
                .show();

        EditText oldPwd = (EditText) v.findViewById(R.id.text_old_password);
        EditText newPwd = (EditText) v.findViewById(R.id.text_new_password);
        Button confirm = (Button) v.findViewById(R.id.button_change_mdp);
        Button cancel = (Button) v.findViewById(R.id.button_cancel_change_mdp);

        confirm.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String oldPassword = oldPwd.getText().toString();
                String newPassword = newPwd.getText().toString();
                if (!oldPassword.equals("") && !newPassword.equals("")) {
                    if (oldPassword.equals(password)) {
                        SupervisorIdentificationPOJO.NewPasswordPOJO newPwdPOJO = new SupervisorIdentificationPOJO.NewPasswordPOJO(oldPassword, newPassword);
                        accountService.changePassword(newPwdPOJO, v.getContext(), newPassword);
                    }
                }
            }
        });

        cancel.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                adChangePwd.dismiss();
            }
        });
    }

    public static void initCurrentSong(SongPOJO song) {
        songName.setText(song.getTitre());
        songArtist.setText(song.getArtiste());
        songLength.setText(song.getMinutes() + " min " + song.getSeconds() + " sec");
        songIp.setText(song.getIp());
        songMAC.setText(song.getMAC());
    }

    public static void initListSongRecyclerView(List<SongPOJO> list, Context context) {
        LinearLayoutManager mLayoutManager = new LinearLayoutManager(context);
        mLayoutManager.setOrientation(LinearLayoutManager.VERTICAL);
        mRecyclerView.setLayoutManager(mLayoutManager);

        DynamicEventsHelper.DynamicEventsCallback callback = new DynamicEventsHelper.DynamicEventsCallback() {
            @Override
            public void onItemMove(int initialPosition, int finalPosition) {
                mAdapter.onItemMove(initialPosition, finalPosition);
            }

            @Override
            public void removeItem(int position) {
                mAdapter.removeItem(position);
            }
        };

        ItemTouchHelper itemTouchHelper = new ItemTouchHelper(new DynamicEventsHelper(callback));
        itemTouchHelper.attachToRecyclerView(mRecyclerView);

        mAdapter = new ListSongAdapter((SupervisorActivity) context, list, itemTouchHelper, username);
        mRecyclerView.setAdapter(mAdapter);
    }

    public static void initListIdentifiedUserRecyclerView(List<BlackListUserPOJO.User> list, Activity activity) {
        LinearLayoutManager mLayoutManager = new LinearLayoutManager(activity.getApplicationContext());
        mLayoutManager.setOrientation(LinearLayoutManager.VERTICAL);
        mIdRecyclerView.setLayoutManager(mLayoutManager);

        mIdAdapter = new IdentifiedUserAdapter(list, activity, username);
        mIdRecyclerView.setAdapter(mIdAdapter);
    }

    public static void initListBlockedUserRecyclerView(List<BlackListUserPOJO.User> list, Activity activity) {
        LinearLayoutManager mLayoutManager = new LinearLayoutManager(activity.getApplicationContext());
        mLayoutManager.setOrientation(LinearLayoutManager.VERTICAL);
        mBURecyclerView.setLayoutManager(mLayoutManager);

        mBUAdapter = new BlockedUserAdapter(list, activity, username);
        mBURecyclerView.setAdapter(mBUAdapter);
    }
}
