package com.polycom.polycom.utils;

import android.media.AudioManager;

import com.socks.library.KLog;

/**
 * Created by zsn on 2016/8/25.
 */
public class VolumeUtils {

    public static void setMyVolume(AudioManager mAudioManager) {

        //参数三AudioManager.FLAG_PLAY_SOUND | AudioManager.FLAG_SHOW_UI表示在调整媒体音量的时候会发出声音，并且弹出音量调整对话框
        //如果不想要这些,可以设置为0
        mAudioManager.adjustStreamVolume(AudioManager.STREAM_RING ,
                AudioManager.ADJUST_RAISE, AudioManager.FLAG_PLAY_SOUND
                        | AudioManager.FLAG_SHOW_UI);
        int currentVolume = mAudioManager.getStreamVolume(AudioManager.STREAM_RING);
        KLog.i("zsn", "currentVolume=" + currentVolume);
        //获取最大铃音音量值
        //int ringMax = mAudioManager.getStreamMaxVolume( AudioManager.STREAM_RING );
        // mAudioManager.setStreamVolume(AudioManager.STREAM_RING, ringMax, 0);

        //获取最大媒体音量值
        // int videoMax = mAudioManager.getStreamMaxVolume(AudioManager.STREAM_MUSIC);
        // mAudioManager.setStreamVolume(AudioManager.STREAM_MUSIC, videoMax, 0);
    }
}
