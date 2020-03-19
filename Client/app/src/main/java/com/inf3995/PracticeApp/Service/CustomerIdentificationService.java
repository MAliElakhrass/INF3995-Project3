package com.inf3995.PracticeApp.Service;

import android.content.Context;
import android.widget.Toast;

import com.inf3995.PracticeApp.Activities.CustomerActivity;
import com.inf3995.PracticeApp.POJO.IdentificationPOJO;
import com.inf3995.PracticeApp.POJO.IdentificationParamsPOJO;
import com.inf3995.PracticeApp.Utils.DeviceInfo;
import com.inf3995.PracticeApp.Utils.ErrorInfo;

import io.reactivex.SingleObserver;
import io.reactivex.android.schedulers.AndroidSchedulers;
import io.reactivex.disposables.Disposable;
import io.reactivex.schedulers.Schedulers;

public class CustomerIdentificationService {
    private Context context;

    public CustomerIdentificationService(Context context) {
        this.context = context;
    }

    public void loginService(String name) {
        DeviceInfo deviceInfo = new DeviceInfo(context);
        String ip = deviceInfo.getIpAddr();
        String mac = deviceInfo.getMACAddr();

        ServerAPI serverAPI = new ServerService(false, context).getServerAPI();
        IdentificationParamsPOJO identificationParamsPOJO = new IdentificationParamsPOJO(ip, mac, name);
        serverAPI.getToken(identificationParamsPOJO)
                .subscribeOn(Schedulers.io())
                .observeOn(AndroidSchedulers.mainThread())
                .subscribe(new SingleObserver<IdentificationPOJO>() {
                    @Override
                    public void onSubscribe(Disposable d) { }

                    @Override
                    public void onSuccess(IdentificationPOJO identificationPOJO) {
                        CustomerActivity.mUser = name;
                        CustomerActivity.mToken = identificationPOJO.getIdentificateur();
                        Toast.makeText(context, identificationPOJO.getMessage(), Toast.LENGTH_SHORT).show();
                    }

                    @Override
                    public void onError(Throwable e) {
                        Toast.makeText(context, ErrorInfo.convertErrorMessage(e.getMessage()), Toast.LENGTH_SHORT).show();
                    }
                });
    }
}
