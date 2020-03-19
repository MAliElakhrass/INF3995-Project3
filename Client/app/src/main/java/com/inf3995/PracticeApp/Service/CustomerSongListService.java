package com.inf3995.PracticeApp.Service;

import android.content.Context;
import android.widget.Toast;

import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;
import com.inf3995.PracticeApp.Fragment.CustomerServerMusicList;
import com.inf3995.PracticeApp.POJO.ListSongPOJO;
import com.inf3995.PracticeApp.POJO.SongPOJO;
import com.inf3995.PracticeApp.Utils.ErrorInfo;

import org.jsoup.Jsoup;
import org.jsoup.nodes.Document;
import org.jsoup.nodes.Element;
import org.jsoup.safety.Whitelist;

import java.text.Normalizer;
import java.util.Locale;

import io.reactivex.SingleObserver;
import io.reactivex.android.schedulers.AndroidSchedulers;
import io.reactivex.disposables.Disposable;
import io.reactivex.schedulers.Schedulers;

public class CustomerSongListService {

    private Context context;
    private String token;
    private String username;
    private CustomerServerMusicList fragmentServerList;
    private static RequestQueue queue;

    private static final String LYRICS_URL = "https://www.lyricsmania.com/%s_lyrics_%s.html";

    public CustomerSongListService(Context context, String token, String username, CustomerServerMusicList fragmentServerList) {
        this.context = context;
        this.token = token;
        this.username = username;
        this.fragmentServerList = fragmentServerList;
    }

    public void getSongList() {
        ServerAPI serverAPI = new ServerService(false, context).getServerAPI();
        serverAPI.getUserMusicList(token).subscribeOn(Schedulers.io()).observeOn(AndroidSchedulers.mainThread())
                .subscribe(new SingleObserver<ListSongPOJO>() {
                    @Override
                    public void onSubscribe(Disposable d) { }

                    @Override
                    public void onSuccess(ListSongPOJO listSongs) {
                        if (listSongs.getCurrentSong() != null)
                            fragmentServerList.initCurrentSongView(listSongs.getCurrentSong());
                        fragmentServerList.initListSongRecyclerView(listSongs.getAsList(), username);
                    }

                    @Override
                    public void onError(Throwable e) {
                        Toast.makeText(context, ErrorInfo.convertErrorMessage(e.getMessage()), Toast.LENGTH_SHORT).show();
                    }
                });
    }

    public void getLyrics(SongPOJO song) {
        queue = Volley.newRequestQueue(context);
        StringRequest stringRequest = new StringRequest(Request.Method.GET, getURL(song.getArtiste(), song.getTitre()),
                new com.android.volley.Response.Listener<String>() {
                    @Override
                    public void onResponse(String response) {
                        try {
                            Document document = Jsoup.parse(response);
                            Element lyricsBody = document.getElementsByClass("lyrics-body").get(0);
                            lyricsBody.select("div").last().remove();

                            String text = Jsoup.clean(lyricsBody.html(), "", Whitelist.basic().addTags("div"));
                            text = text.substring(text.indexOf("</strong>") + 1, text.lastIndexOf("</div>"));
                            text = text.replaceAll("<br>|<div>|</div>", "").replaceAll("(?m)^[ \\t]*\\r?\\n", "").replaceAll("\\n(\\s*)?", "\n").trim();

                            fragmentServerList.setLyricsView(text);
                        } catch (Exception e){
                            fragmentServerList.setLyricsView("Lyrics not found !!!");
                        }
                    }
                },
                new com.android.volley.Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError e) {
                        Toast.makeText(context, ErrorInfo.convertErrorMessage(e.getMessage()), Toast.LENGTH_SHORT).show();
                    }
                });
        queue.add(stringRequest);
    }

    private static String getURL(String artist, String song) {
        String htmlArtist = Normalizer.normalize(artist.replaceAll("[\\s-]", "_"), Normalizer.Form.NFD)
                .replaceAll("[^\\p{ASCII}]", "").replaceAll("[^A-Za-z0-9_]", "");
        String htmlSong = Normalizer.normalize(song.replaceAll("[\\s-]", "_"), Normalizer.Form.NFD)
                .replaceAll("[^\\p{ASCII}]", "").replaceAll("[^A-Za-z0-9_]", "");

        if (artist.startsWith("The"))
            htmlArtist = htmlArtist.substring(4) + "_the";

        return String.format(LYRICS_URL, htmlSong.toLowerCase(Locale.getDefault()), htmlArtist.toLowerCase(Locale.getDefault()));
    }
}
