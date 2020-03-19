package com.inf3995.PracticeApp.Activities;

import android.content.DialogInterface;
import android.os.Build;
import android.support.annotation.RequiresApi;
import android.support.design.widget.AppBarLayout;
import android.support.design.widget.TabLayout;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;

import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentPagerAdapter;
import android.support.v4.view.ViewPager;
import android.os.Bundle;
import android.text.InputType;
import android.view.Menu;
import android.view.MenuItem;
import android.view.Window;
import android.view.WindowManager;
import android.widget.EditText;

import com.inf3995.PracticeApp.Fragment.CustomerOwnMusicList;
import com.inf3995.PracticeApp.Fragment.CustomerServerMusicList;
import com.inf3995.PracticeApp.R;
import com.inf3995.PracticeApp.Service.CustomerIdentificationService;

public class CustomerActivity extends AppCompatActivity {

    public static String mToken;
    public static String mUser;
    private CustomerOwnMusicList mOwnMusicList;
    private CustomerServerMusicList mServerMusicList;
    private SectionsPagerAdapter mSectionsPagerAdapter;
    private ViewPager mViewPager;

    @RequiresApi(api = Build.VERSION_CODES.LOLLIPOP)
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        getSupportFragmentManager();
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_customer);

        String nameInput = getIntent().getStringExtra("nom");
        if (nameInput.length() > 0) {
            CustomerIdentificationService customerIdentificationService = new CustomerIdentificationService(getApplicationContext());
            customerIdentificationService.loginService(nameInput);
        }

        Window window = this.getWindow();
        window.clearFlags(WindowManager.LayoutParams.FLAG_TRANSLUCENT_STATUS);
        window.addFlags(WindowManager.LayoutParams.FLAG_DRAWS_SYSTEM_BAR_BACKGROUNDS);
        window.setStatusBarColor(this.getResources().getColor(R.color.colorPrimaryDark));

        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);
        getSupportActionBar().setDisplayHomeAsUpEnabled(true);
        AppBarLayout.LayoutParams params = (AppBarLayout.LayoutParams) toolbar.getLayoutParams();
        params.setScrollFlags(0);

        mSectionsPagerAdapter = new SectionsPagerAdapter(getSupportFragmentManager());

        mViewPager = (ViewPager) findViewById(R.id.container);
        mViewPager.setAdapter(mSectionsPagerAdapter);

        TabLayout tabLayout = (TabLayout) findViewById(R.id.tabs);

        mViewPager.addOnPageChangeListener(new TabLayout.TabLayoutOnPageChangeListener(tabLayout));
        tabLayout.addOnTabSelectedListener(new TabLayout.ViewPagerOnTabSelectedListener(mViewPager));
        tabLayout.addOnTabSelectedListener(new TabLayout.OnTabSelectedListener() {
            @Override
            public void onTabSelected(TabLayout.Tab tab) {
                switch (tab.getPosition()) {
                    case 0:
                        break;
                    case 1:
                        mServerMusicList.getSongsList();
                        break;
                    default:
                        break;
                }
            }

            @Override
            public void onTabUnselected(TabLayout.Tab tab) { }

            @Override
            public void onTabReselected(TabLayout.Tab tab) { }
        });
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.menu_customer, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        int id = item.getItemId();

        if (id == R.id.action_settings) {
            login();
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    public static String getToken() {
        return mToken;
    }

    private void login() {
        EditText nameInput = new EditText(this);
        nameInput.setInputType(InputType.TYPE_CLASS_TEXT);

        AlertDialog ad = new AlertDialog.Builder(this)
                .setTitle("Please enter your name!")
                .setView(nameInput)
                .setPositiveButton("Confirm Name", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        String nameImput = nameInput.getText().toString();
                        if (!nameImput.equals("")){
                            CustomerIdentificationService customerIdentificationService = new CustomerIdentificationService(getApplicationContext());
                            customerIdentificationService.loginService(nameImput);
                        }
                    }
                })
                .setNegativeButton("Cancel", null)
                .show();
    }

    public class SectionsPagerAdapter extends FragmentPagerAdapter {

        public SectionsPagerAdapter(FragmentManager fm) {
            super(fm);
        }

        @Override
        public Fragment getItem(int position) {
            switch (position) {
                case 0:
                    mOwnMusicList = CustomerOwnMusicList.newInstance(mToken);
                    return mOwnMusicList;
                case 1:
                    mServerMusicList = CustomerServerMusicList.newInstance(mToken);
                    return mServerMusicList;
                default:
                    return null;
            }
        }

        @Override
        public int getCount() {
            return 2;
        }
    }
}
