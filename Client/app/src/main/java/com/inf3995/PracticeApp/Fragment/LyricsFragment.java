package com.inf3995.PracticeApp.Fragment;

import android.content.Context;
import android.os.Bundle;
import android.support.v4.app.DialogFragment;
import android.text.method.ScrollingMovementMethod;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import com.inf3995.PracticeApp.R;

public class LyricsFragment extends DialogFragment {

    private static String lyrics;
    private TextView textViewLyrics;

    public LyricsFragment() { }

    public static LyricsFragment newInstance(String lyricsInput) {
        lyrics = lyricsInput;
        LyricsFragment lyricsFragment = new LyricsFragment();
        return lyricsFragment;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        return inflater.inflate(R.layout.layout_lyrics, container, false);
    }

    @Override
    public void onViewCreated(View view, Bundle savedInstance){
        textViewLyrics = (TextView) view.findViewById(R.id.textView_lyrics);
        textViewLyrics.setText(lyrics);
        textViewLyrics.setMovementMethod(new ScrollingMovementMethod());
    }

    @Override
    public void onAttach(Context context) {
        super.onAttach(context);

    }

    @Override
    public void onDetach() {
        super.onDetach();

    }
}
