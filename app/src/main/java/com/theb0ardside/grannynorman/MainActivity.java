package com.theb0ardside.grannynorman;

import android.content.res.AssetManager;
import android.os.Bundle;
import android.widget.SeekBar;

import androidx.appcompat.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity {

    private native void startEngine(AssetManager mgr);

    private native void setGrainsPerSecond(float frequency);

    private native void setGrainDuration(float frequency);

    private native void setGrainSpray(float frequency);

    private native void setGrainFudge(float frequency);

    private AssetManager mgr;

    private boolean active;

    SeekBar seekbar_grains_per_second; // 10 - 200?
    SeekBar seekbar_grain_duration; // ms. 10ms - 200?
    SeekBar seekbar_spray; // random start offset from playback idx in ms
    SeekBar seekbar_fudge; // random diff added to duration

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mgr = getResources().getAssets();
        active = true;

        seekbar_grains_per_second
                = (SeekBar) findViewById(R.id.seekBar);

        seekbar_grains_per_second
                .setOnSeekBarChangeListener(
                        new SeekBar
                                .OnSeekBarChangeListener() {

                            // When the progress value has changed
                            @Override
                            public void onProgressChanged(
                                    SeekBar seekBar,
                                    int val,
                                    boolean fromUser) {

                                setGrainsPerSecond(val);
                            }

                            @Override
                            public void onStartTrackingTouch(SeekBar seekBar) {
                            }

                            @Override
                            public void onStopTrackingTouch(SeekBar seekBar) {
                            }
                        });

        seekbar_grain_duration = (SeekBar) findViewById(R.id.seekBar2);
        seekbar_grain_duration
                .setOnSeekBarChangeListener(
                        new SeekBar
                                .OnSeekBarChangeListener() {

                            // When the progress value has changed
                            @Override
                            public void onProgressChanged(
                                    SeekBar seekBar,
                                    int val,
                                    boolean fromUser) {

                                setGrainDuration(val);
                            }

                            @Override
                            public void onStartTrackingTouch(SeekBar seekBar) {
                            }

                            @Override
                            public void onStopTrackingTouch(SeekBar seekBar) {
                            }
                        });

        seekbar_spray = (SeekBar) findViewById(R.id.seekBar3);
        seekbar_spray
                .setOnSeekBarChangeListener(
                        new SeekBar
                                .OnSeekBarChangeListener() {

                            // When the progress value has changed
                            @Override
                            public void onProgressChanged(
                                    SeekBar seekBar,
                                    int val,
                                    boolean fromUser) {

                                setGrainSpray(val);
                            }

                            @Override
                            public void onStartTrackingTouch(SeekBar seekBar) {
                            }

                            @Override
                            public void onStopTrackingTouch(SeekBar seekBar) {
                            }
                        });

        seekbar_fudge = (SeekBar) findViewById(R.id.seekBar4);
        seekbar_fudge
                .setOnSeekBarChangeListener(
                        new SeekBar
                                .OnSeekBarChangeListener() {

                            // When the progress value has changed
                            @Override
                            public void onProgressChanged(
                                    SeekBar seekBar,
                                    int val,
                                    boolean fromUser) {

                                setGrainFudge(val);
                            }

                            @Override
                            public void onStartTrackingTouch(SeekBar seekBar) {
                            }

                            @Override
                            public void onStopTrackingTouch(SeekBar seekBar) {
                            }
                        });


        startEngine(mgr);


    }


}
