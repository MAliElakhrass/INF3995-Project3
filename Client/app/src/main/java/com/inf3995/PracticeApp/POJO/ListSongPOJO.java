package com.inf3995.PracticeApp.POJO;

import com.google.gson.annotations.Expose;
import com.google.gson.annotations.SerializedName;

import java.util.ArrayList;
import java.util.List;

public class ListSongPOJO {

    @SerializedName("songs")
    @Expose
    private SongPOJO[] songs;

    @SerializedName("current song")
    @Expose
    private SongPOJO[] currentSong;

    public SongPOJO[] getSongs() {
        return songs;
    }

    public void setSongs(SongPOJO[] songs) {
        this.songs = songs;
    }

    public ArrayList<String> displaySongs() {
        ArrayList<String> displayArray = new ArrayList<String>();

        if (currentSong != null) {
            displayArray.add("Titre: " + currentSong[0].getTitre() + " Artiste: " + currentSong[0].getArtiste() + " Length: " + currentSong[0].getMinutes() + " minutes "
                    + currentSong[0].getSeconds() + " seconds is playing");
        }
        if (songs != null) {
            for (SongPOJO song: songs) {
                displayArray.add("Titre: " + song.getTitre() + " Artiste: " + song.getArtiste() + " Length: " + song.getMinutes() + " minutes "
                                    + song.getSeconds() + " seconds");
            }
        }

        return  displayArray;
    }

    public ArrayList<Integer> getOwnSong(String nom) {
        ArrayList<Integer> ownSongs = new ArrayList<>();
        int index = 0;
        if (currentSong != null) {
            if (getCurrentSong().getProposeePar().equals(nom)) {
                ownSongs.add(index);
            }
            index++;
        }
        if (songs != null) {
            for (SongPOJO song: songs) {
                if (song.getProposeePar().equals(nom))
                    ownSongs.add(index);

                index++;
            }
        }

        return ownSongs;
    }

    public SongPOJO getCurrentSong() {
        if (currentSong != null)
            return currentSong[0];

        return null;
    }

    public List<SongPOJO> getAsList() {
        List<SongPOJO> tmp = new ArrayList<SongPOJO>();
        if (songs != null) {
            for (SongPOJO song: songs) {
                tmp.add(song);
            }
        }

        return tmp;
    }
}
