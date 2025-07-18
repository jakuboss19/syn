package com.yourname.synplayer;

import android.Manifest;
import android.content.pm.PackageManager;
import android.media.MediaPlayer;
import android.os.Bundle;
import android.os.Environment;
import android.view.View;
import android.widget.*;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import java.io.File;
import java.util.ArrayList;
import android.os.Handler;

public class MainActivity extends AppCompatActivity {
    private ListView listView;
    private ArrayList<String> songList = new ArrayList<>();
    private ArrayList<String> songNameList = new ArrayList<>(); // Store just the song names
    private MediaPlayer mediaPlayer = new MediaPlayer();
    private String currentSongName = null;
    private TextView currentSongTextView; // For showing currently playing song
    private Button playButton, pauseButton, stopButton; // Playback controls
    private SeekBar seekBar;
    private Handler handler = new Handler();
    private Runnable updateSeekBar;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        listView = findViewById(R.id.songListView);
        currentSongTextView = findViewById(R.id.currentSongTextView); // Add this to your layout
        playButton = findViewById(R.id.playButton); // Add to layout
        pauseButton = findViewById(R.id.pauseButton); // Add to layout
        stopButton = findViewById(R.id.stopButton); // Add to layout
        seekBar = findViewById(R.id.seekBar);

        // Request storage permission
        ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.READ_EXTERNAL_STORAGE}, 1);

        loadSongs();

        ArrayAdapter<String> adapter = new ArrayAdapter<>(this, android.R.layout.simple_list_item_1, songNameList);
        listView.setAdapter(adapter);

        listView.setOnItemClickListener((parent, view, position, id) -> playSong(songList.get(position)));

        // Button listeners will be set up after layout is available
        playButton.setOnClickListener(v -> {
            if (!mediaPlayer.isPlaying() && mediaPlayer.getCurrentPosition() > 0) {
                mediaPlayer.start();
                if (currentSongName != null && currentSongTextView != null) {
                    currentSongTextView.setText("Playing: " + currentSongName);
                }
            }
        });

        pauseButton.setOnClickListener(v -> {
            if (mediaPlayer.isPlaying()) {
                mediaPlayer.pause();
                if (currentSongName != null && currentSongTextView != null) {
                    currentSongTextView.setText("Paused: " + currentSongName);
                }
            }
        });

        stopButton.setOnClickListener(v -> {
            if (mediaPlayer.isPlaying() || mediaPlayer.getCurrentPosition() > 0) {
                mediaPlayer.stop();
                try {
                    mediaPlayer.prepare(); // Prepare for next play
                } catch (Exception e) {
                    // Ignore for now
                }
                if (currentSongName != null && currentSongTextView != null) {
                    currentSongTextView.setText("Stopped: " + currentSongName);
                }
            }
        });

        // SeekBar change listener
        seekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                if (fromUser && mediaPlayer.isPlaying()) {
                    mediaPlayer.seekTo(progress);
                }
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {}

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {}
        });

        // Update SeekBar as song plays
        updateSeekBar = new Runnable() {
            @Override
            public void run() {
                if (mediaPlayer != null && (mediaPlayer.isPlaying() || mediaPlayer.getCurrentPosition() > 0)) {
                    seekBar.setMax(mediaPlayer.getDuration());
                    seekBar.setProgress(mediaPlayer.getCurrentPosition());
                    handler.postDelayed(this, 500);
                }
            }
        };
    }

    private void loadSongs() {
        File musicDir = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_MUSIC);
        if (musicDir.exists() && musicDir.isDirectory()) {
            for (File file : musicDir.listFiles()) {
                if (file.getName().endsWith(".mp3")) {
                    songList.add(file.getAbsolutePath());
                    songNameList.add(file.getName());
                }
            }
        }
    }

    private void playSong(String path) {
        try {
            mediaPlayer.reset();
            mediaPlayer.setDataSource(path);
            mediaPlayer.prepare();
            mediaPlayer.start();
            currentSongName = new File(path).getName();
            if (currentSongTextView != null) {
                currentSongTextView.setText("Playing: " + currentSongName);
            }
            Toast.makeText(this, "Playing: " + currentSongName, Toast.LENGTH_SHORT).show();
            // Start updating seek bar
            if (seekBar != null) {
                seekBar.setMax(mediaPlayer.getDuration());
                handler.post(updateSeekBar);
            }
        } catch (Exception e) {
            Toast.makeText(this, "Error playing song", Toast.LENGTH_SHORT).show();
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (mediaPlayer != null) {
            mediaPlayer.release();
            mediaPlayer = null;
        }
        handler.removeCallbacksAndMessages(null);
    }
} 