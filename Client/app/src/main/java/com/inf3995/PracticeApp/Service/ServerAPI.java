package com.inf3995.PracticeApp.Service;

import com.inf3995.PracticeApp.POJO.BlackListUserPOJO;
import com.inf3995.PracticeApp.POJO.IdentificationPOJO;
import com.inf3995.PracticeApp.POJO.IdentificationParamsPOJO;
import com.inf3995.PracticeApp.POJO.ListSongPOJO;
import com.inf3995.PracticeApp.POJO.PositionChangePOJO;
import com.inf3995.PracticeApp.POJO.SoundPOJO;
import com.inf3995.PracticeApp.POJO.SupervisorIdentificationPOJO;
import okhttp3.ResponseBody;

import retrofit2.Call;
import retrofit2.http.Body;
import retrofit2.http.DELETE;
import retrofit2.http.GET;
import retrofit2.http.POST;
import retrofit2.http.Path;

import io.reactivex.Single;

public interface ServerAPI {
    @POST("usager/identification")
    Single<IdentificationPOJO> getToken(@Body IdentificationParamsPOJO deviceInfo);

    @GET("usager/file/{Id}")
    Single<ListSongPOJO> getUserMusicList(@Path("Id") String Id);

    @POST("usager/chanson/{Id}")
    Call<ResponseBody> uploadSong(@Path("Id") String Id, @Body String musicFile);

    @DELETE("usager/chanson/{Id}/{no}")
    Call<ResponseBody> deleteSongUser(@Path("Id") String Id, @Path("no") Integer no);

    @POST("superviseur/login")
    Call<ResponseBody> loginSupervisor(@Body SupervisorIdentificationPOJO identificationPOJO);

    @POST("superviseur/{usager}/logout")
    Call<ResponseBody> logoutSupervisor(@Path("usager") String usager);

    @POST("superviseur/{usager}/changement_mdp")
    Call<ResponseBody> changeSupervisorPassword(@Path("usager") String usager, @Body SupervisorIdentificationPOJO.NewPasswordPOJO passwordPOJO);

    @GET("superviseur/{usager}/file")
    Single<ListSongPOJO> getMusicListSupervisor(@Path("usager") String usager);

    @POST("superviseur/{usager}/position")
    Call<ResponseBody> changePositionList(@Path("usager") String usager, @Body PositionChangePOJO position);

    @GET("superviseur/{usager}/volume")
    Single<SoundPOJO> getVolume(@Path("usager") String usager);

    @POST("superviseur/{usager}/volume/augmenter/{pc}")
    Call<ResponseBody> increaseVolume(@Path("usager") String usager, @Path("pc") String pc);

    @POST("superviseur/{usager}/volume/diminuer/{pc}")
    Call<ResponseBody> decreaseVolume(@Path("usager") String usager, @Path("pc") String pc);

    @POST("superviseur/{usager}/volume/sourdine/activer")
    Call<ResponseBody> enableMute(@Path("usager") String usager);

    @POST("superviseur/{usager}/volume/sourdine/desactiver")
    Call<ResponseBody> disableMute(@Path("usager") String usager);

    @DELETE("superviseur/{usager}/chanson/{no}")
    Call<ResponseBody> deleteSongSup(@Path("usager") String usager, @Path("no") Integer no);

    @GET("superviseur/{usager}/listenoire")
    Single<BlackListUserPOJO> getBlockedUsers(@Path("usager") String usager);

    @POST("superviseur/{usager}/bloquer")
    Call<ResponseBody> blockUser(@Path("usager") String usager, @Body BlackListUserPOJO.User user);

    @POST("superviseur/{usager}/debloquer")
    Call<ResponseBody> unblockUser(@Path("usager") String usager, @Body BlackListUserPOJO.MAC mac);
}
