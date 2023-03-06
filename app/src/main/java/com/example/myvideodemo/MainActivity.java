package com.example.myvideodemo;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.TextView;

import com.example.myvideodemo.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'myvideodemo' library on application startup.
    static {
        System.loadLibrary("native-render");
    }

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());
    }

    @Override
    protected void onResume() {
        super.onResume();
        binding.glSurfaceView.onResume();
    }

    @Override
    protected void onPause() {
        super.onPause();
        binding.glSurfaceView.onPause();
    }

}