package com.inf3995.PracticeApp.Utils;

import android.os.Environment;

import java.io.File;
import java.util.ArrayList;

public class Storage {

   private static ArrayList<String> musicFiles = new ArrayList<>();

    public Storage(){ }

    public static ArrayList<String> getMusicFiles() {
        musicFiles = new ArrayList<>();
        File Storage = Environment.getExternalStorageDirectory();

        if(Storage != null){
            File[] files = Storage.listFiles();

            find(Storage);
        } else {
            System.out.println("Problem with Storage");
        }

        return musicFiles;
    }

    private static void find(File Directory) {
        File[] files = Directory.listFiles();
        for (File child : files) {
            if (child.isDirectory()){
                find(child);
            } else if (child.isFile()){
                if (isMusic(child)){
                    musicFiles.add(child.toString());
                }
            }
        }
    }

    private static boolean isMusic(File file) {
        return getFileExtension(file).equals("mp3");
    }

    private static String getFileExtension(File file) {
        try {
            if (file != null && file.exists()) {
                String name = file.getName();

                return name.substring(name.lastIndexOf(".") + 1);
            }
        } catch (Exception e ) {
            System.out.println("Error when getting File extension");
        }
        return "null";
    }


}
