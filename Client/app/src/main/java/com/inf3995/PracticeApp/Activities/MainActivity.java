package com.inf3995.PracticeApp.Activities;

import android.Manifest;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.graphics.Color;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

import com.inf3995.PracticeApp.R;
import com.inf3995.PracticeApp.Service.SupervisorIdentificationService;

public class MainActivity extends AppCompatActivity {

    private EditText mEditName;
    private EditText mEditPassword;

    private LayoutInflater inflater;
    private View view;

    public static AlertDialog ad;

    private String userName;
    private String password;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        setTitleColor(Color.BLACK);
        inflater = this.getLayoutInflater();

        askPermission();

        Button mUserBtn = (Button) findViewById(R.id.activity_main_user_btn);
        mUserBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                launchCustomerActivity(v);
            }
        });

        Button mSupervisorBtn = (Button) findViewById(R.id.activity_main_supervisor_btn);
        mSupervisorBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                loginDialog(v);
            }
        });
    }

    private void askPermission() {
        final int MY_PERMISSIONS_REQUEST_READ_STORAGE = 0;
        if (ContextCompat.checkSelfPermission(this, Manifest.permission.READ_EXTERNAL_STORAGE)
                != PackageManager.PERMISSION_GRANTED) {

            ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.READ_EXTERNAL_STORAGE}, MY_PERMISSIONS_REQUEST_READ_STORAGE);
        }
    }

    private void launchCustomerActivity(View v){
        EditText editText = (EditText) findViewById(R.id.textView_customer_identification);
        String nom = editText.getText().toString();

        Intent intent = new Intent(this, CustomerActivity.class);
        intent.putExtra("nom", nom);
        startActivity(intent);
    }

    public void launchSupervisorActivity(View v){
        Intent intent = new Intent(this, SupervisorActivity.class);
        intent.putExtra("UserName", userName);
        intent.putExtra("Password", password);
        startActivity(intent);
    }

    private void loginDialog(View v) {
        this.view = v;
        v = inflater.inflate(R.layout.login_dialog, null);

        ad = new AlertDialog.Builder(v.getContext())
                .setTitle("Login to supervisor app!")
                .setView(v)
                .show();

        mEditName = (EditText) v.findViewById(R.id.text_name);
        mEditPassword = (EditText) v.findViewById(R.id.text_password);
        Button mLoginButton = (Button) v.findViewById(R.id.button_login);
        Button mCancelButton = (Button) v.findViewById(R.id.button_cancel);

        mLoginButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                userName = mEditName.getText().toString();
                password = mEditPassword.getText().toString();

                if (!userName.equals("") && !password.equals("")) {
                    MainActivity mainActivity = (MainActivity)  view.getContext();
                    SupervisorIdentificationService supIdService = new SupervisorIdentificationService(getApplicationContext(), mainActivity);
                    supIdService.login(v, userName, password);
                }
            }
        });

        mCancelButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                ad.dismiss();
            }
        });
    }

}
