package com.inf3995.PracticeApp.Fragment;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.util.Base64;
import android.util.SparseBooleanArray;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AbsListView;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.ProgressBar;

import com.inf3995.PracticeApp.Activities.CustomerActivity;
import com.inf3995.PracticeApp.R;
import com.inf3995.PracticeApp.Service.CustomerUploadSongService;
import com.inf3995.PracticeApp.Utils.Storage;

import org.apache.commons.io.FileUtils;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class CustomerOwnMusicList extends Fragment {
    private ListView mListView;
    private ArrayAdapter mAdapter;
    public static Button mUploadBtn;
    public static ProgressBar mSpinner;
    private ArrayList<String> pathSongs = new ArrayList<>();
    private static String mToken;

    public static CustomerOwnMusicList newInstance(String token) {
        mToken = token;

        return new CustomerOwnMusicList();
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        View rootView = inflater.inflate(R.layout.layout_customer_own_music_list, container, false);

        return rootView;
    }

    @Override
    public void onViewCreated(View view, Bundle savedInstance){
        mListView = (ListView) view.findViewById(R.id.fragment_musicList_list);
        mUploadBtn = (Button) view.findViewById(R.id.fragment_upload_btn);
        mSpinner = (ProgressBar) view.findViewById(R.id.fragment_progress_bar_upload);

        getLocalSongs();

        mListView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                mListView.setItemChecked(position, mListView.isItemChecked(position));
            }
        });

        mUploadBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                SparseBooleanArray sparseBooleanArray = mListView.getCheckedItemPositions();
                mToken = ((CustomerActivity) getActivity()).getToken();
                List<String> listRequest = new ArrayList<>();
                for(int i = 0; i < mListView.getAdapter().getCount(); i++){
                    if(sparseBooleanArray.get(i)) {
                        String path = pathSongs.get(i);
                        File file = new File(path);
                        String base64String = encode_base64(file);
                        base64String.replaceAll("\n", "");
                        listRequest.add(base64String);
                    }
                }

                CustomerUploadSongService service = new CustomerUploadSongService(v.getContext(), listRequest);
                service.uploadSong(mToken);
            }
        });
    }

    /**
     * This function gets an the list of the mp3 files in the device and stock the result in an array
     */
    private void getLocalSongs() {
        pathSongs = Storage.getMusicFiles();
        ArrayList<String> localMusic = Storage.getMusicFiles();

        for(int i = 0; i < localMusic.size(); i++){
            localMusic.set(i, localMusic.get(i).substring(localMusic.get(i).lastIndexOf("/") + 1));
        }

        mAdapter = new ArrayAdapter(this.getContext(), android.R.layout.simple_list_item_multiple_choice, localMusic);
        mListView.setVisibility(View.VISIBLE);
        mListView.setEnabled(true);
        mListView.setChoiceMode(AbsListView.CHOICE_MODE_MULTIPLE);
        mListView.setAdapter(mAdapter);
    }

    /**
     * This function will encode the song to base64
     * @param file
     * @return
     */
    private String encode_base64(File file) {
        String encoded = "";
        try {
            byte[] bytes = FileUtils.readFileToByteArray(file);
            encoded = Base64.encodeToString(bytes, Base64.NO_PADDING | Base64.NO_WRAP);
        } catch (IOException e) {
            e.printStackTrace();
        }

        return encoded;
    }
}
