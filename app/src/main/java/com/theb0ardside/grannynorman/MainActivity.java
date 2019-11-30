package com.theb0ardside.grannynorman;


import android.content.res.AssetManager;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.RadioButton;
import android.widget.SeekBar;
import android.widget.Spinner;
import android.widget.Switch;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity {

    private native void startEngine(AssetManager mgr);

    // per Sampler functions - first param is sampler #

    private native void setGrainsPerSecond(int target, float val);

    private native float getGrainsPerSecond(int target);

    private native void setGrainDuration(int target, float val);

    private native float getGrainDuration(int target);

    private native void setGrainSpray(int target, float val);

    private native float getGrainSpray(int target);

    private native void setGrainFudge(int target, float val);

    private native float getGrainFudge(int target);

    private native void setGranularMode(int target, int mode);

    private native void setGrainIndex(int target, int val);

    private native int getGrainIndex(int target);

    private native void toggleGranularMode(int target); // bad name - loop mode?

    private native void toggleOnOffMode(int target);

    private native void setEnvelopeMode(int target, int val);

    private native void randomize(int target);

    private native void reset(int target);

    private native void scramble(int target);

    private native void stutter(int target);

    private native int getActive(int target);

    private native int getLoopMode(int target);

    private native int getEnvelopeMode(int target);

    private AssetManager mgr;

    private boolean active;

    SeekBar seekbar_grain_index; // 0-100 % of length
    private TextView grain_index_value;

    SeekBar seekbar_grains_per_second;
    private int seekbar_grains_per_second_min = 1;
    private int seekbar_grains_per_second_max = 100;
    private TextView grains_per_second_value;

    SeekBar grain_duration_seekbar;
    private int seekbar_grain_duration_min = 10; // ms
    private int seekbar_grain_duration_max = 200; // ms
    private TextView grain_duration_value;

    SeekBar seekbar_spray; // random start offset from playback idx in ms
    private int seekbar_spray_min = 0; // ms
    private int seekbar_spray_max = 500; // ms
    private TextView spray_value;

    SeekBar seekbar_fudge; // random diff added to duration
    private int seekbar_fudge_min = 0;
    private int seekbar_fudge_max = 500;
    private TextView fudge_value;

    private TextView sample_display_num;

    Switch modeSwitch;
    Switch onOffSwitch;

    Button loopButton;
    Button randButton;
    Button stutterButton;
    Button scrambleButton;

    Spinner envelope_spinner;

    private int sample_number = 0; // 0-3 for drums, bass, strings, thingie


    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    protected void updateInterface() {

        onOffSwitch.setChecked(getActive(sample_number) == 1);
        modeSwitch.setChecked(getLoopMode(sample_number) == 1);

        int grain_idx = getGrainIndex(sample_number);
        Log.v("INTz", "Idx:" + grain_idx);
        if (grain_idx < 0 || grain_idx > 100)
            grain_idx = 0;
        grain_index_value.setText(String.valueOf(grain_idx));
        seekbar_grain_index.setProgress(grain_idx);

        float grains_per_sec = getGrainsPerSecond(sample_number);
        grains_per_second_value.setText(String.valueOf(grains_per_sec));
        seekbar_grains_per_second.setProgress((int) Utilz.scale(grains_per_sec, seekbar_grains_per_second_min, seekbar_grains_per_second_max, 0, 100));

        float grain_duration = getGrainDuration(sample_number);
        grain_duration_value.setText(String.valueOf(grain_duration));
        grain_duration_seekbar.setProgress((int) Utilz.scale(grain_duration, seekbar_grain_duration_min, seekbar_grain_duration_max, 0, 100));

        float grain_spray = getGrainSpray(sample_number);
        spray_value.setText(String.valueOf(grain_spray));
        seekbar_spray.setProgress((int) Utilz.scale(grain_spray, seekbar_spray_min, seekbar_spray_max, 0, 100));

        float grain_fudge = getGrainFudge(sample_number);
        fudge_value.setText(String.valueOf(grain_fudge));
        seekbar_fudge.setProgress((int) Utilz.scale(grain_fudge, seekbar_fudge_min, seekbar_fudge_max, 0, 100));

        int envelope_mode = getEnvelopeMode(sample_number);
        envelope_spinner.setSelection(envelope_mode);

    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {

        Log.v("JAVAME", "Starting UP, YO!\n\n");

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mgr = getResources().getAssets();
        active = true;

        seekbar_grain_index
                = (SeekBar) findViewById(R.id.seekBar5);
        grain_index_value = (TextView) findViewById(R.id.textView9);

        sample_display_num = (TextView) findViewById(R.id.SampleNum);

        seekbar_grain_index
                .setOnSeekBarChangeListener(
                        new SeekBar
                                .OnSeekBarChangeListener() {

                            // When the progress value has changed
                            @Override
                            public void onProgressChanged(
                                    SeekBar seekBar,
                                    int val,
                                    boolean fromUser) {

                                setGrainIndex(sample_number, val);
                                grain_index_value.setText(String.valueOf(val));
                            }

                            @Override
                            public void onStartTrackingTouch(SeekBar seekBar) {
                            }

                            @Override
                            public void onStopTrackingTouch(SeekBar seekBar) {
                            }
                        });

        seekbar_grains_per_second
                = (SeekBar) findViewById(R.id.seekBar);
        grains_per_second_value = (TextView) findViewById(R.id.gps_value);

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

                                double scaled_val = Utilz.scale(val, 0, 100, seekbar_grains_per_second_min, seekbar_grains_per_second_max);
                                setGrainsPerSecond(sample_number, (float) scaled_val);
                                grains_per_second_value.setText(String.valueOf(scaled_val));
                            }

                            @Override
                            public void onStartTrackingTouch(SeekBar seekBar) {
                            }

                            @Override
                            public void onStopTrackingTouch(SeekBar seekBar) {
                            }
                        });

        grain_duration_seekbar = (SeekBar) findViewById(R.id.seekBar2);
        grain_duration_value = (TextView) findViewById(R.id.textView4);
        grain_duration_seekbar
                .setOnSeekBarChangeListener(
                        new SeekBar
                                .OnSeekBarChangeListener() {

                            // When the progress value has changed
                            @Override
                            public void onProgressChanged(
                                    SeekBar seekBar,
                                    int val,
                                    boolean fromUser) {

                                double scaled_val = Utilz.scale(val, 0, 100, seekbar_grain_duration_min, seekbar_grain_duration_max);
                                setGrainDuration(sample_number, (float) scaled_val);
                                grain_duration_value.setText(String.valueOf(scaled_val));
                            }

                            @Override
                            public void onStartTrackingTouch(SeekBar seekBar) {
                            }

                            @Override
                            public void onStopTrackingTouch(SeekBar seekBar) {
                            }
                        });

        seekbar_spray = (SeekBar) findViewById(R.id.seekBar3);
        spray_value = (TextView) findViewById(R.id.textView6);
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

                                double scaled_val = Utilz.scale(val, 0, 100, seekbar_spray_min, seekbar_spray_max);
                                setGrainSpray(sample_number, (float) scaled_val);
                                spray_value.setText(String.valueOf(scaled_val));
                            }

                            @Override
                            public void onStartTrackingTouch(SeekBar seekBar) {
                            }

                            @Override
                            public void onStopTrackingTouch(SeekBar seekBar) {
                            }
                        });

        seekbar_fudge = (SeekBar) findViewById(R.id.seekBar4);
        fudge_value = (TextView) findViewById(R.id.textView8);
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

                                double scaled_val = Utilz.scale(val, 0, 100, seekbar_fudge_min, seekbar_fudge_max);
                                setGrainFudge(sample_number, (float) scaled_val);
                                fudge_value.setText(String.valueOf(scaled_val));

                            }

                            @Override
                            public void onStartTrackingTouch(SeekBar seekBar) {
                            }

                            @Override
                            public void onStopTrackingTouch(SeekBar seekBar) {
                            }
                        });

        modeSwitch = findViewById(R.id.button);
        modeSwitch.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                toggleGranularMode(sample_number);
                // Code here executes on main thread after user presses button
            }
        });

        onOffSwitch = findViewById(R.id.button2);
        onOffSwitch.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                toggleOnOffMode(sample_number);
            }
        });

        loopButton = findViewById(R.id.button3);
        loopButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                reset(sample_number);
                updateInterface();
            }
        });

        randButton = findViewById(R.id.button_rand);
        randButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                randomize(sample_number);
                updateInterface();
            }
        });

        scrambleButton = findViewById(R.id.button4);
        scrambleButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                scramble(sample_number);
            }
        });

        stutterButton = findViewById(R.id.button5);
        stutterButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                stutter(sample_number);
            }
        });


        envelope_spinner = (Spinner) findViewById(R.id.spinner);
        ArrayAdapter<CharSequence> adapter = ArrayAdapter.createFromResource(this,
                R.array.envelope_types, android.R.layout.simple_spinner_item);
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        envelope_spinner.setAdapter(adapter);
        envelope_spinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                setEnvelopeMode(sample_number, position);
            }

            @Override
            public void onNothingSelected(AdapterView<?> parent) {

            }
        });

        startEngine(mgr);

        updateInterface();


    }

    public void onRadioButtonClicked(View view) {
        // Is the button now checked?
        boolean checked = ((RadioButton) view).isChecked();

        // Check which radio button was clicked
        switch (view.getId()) {
            case R.id.radio_0:
                if (checked)
                    sample_number = 0;
                break;
            case R.id.radio_1:
                if (checked)
                    sample_number = 1;
                break;
            case R.id.radio_2:
                if (checked)
                    sample_number = 2;
                break;
            case R.id.radio_3:
                if (checked)
                    sample_number = 3;
                break;
        }

        sample_display_num.setText(String.valueOf(sample_number));
        updateInterface();
    }


}
