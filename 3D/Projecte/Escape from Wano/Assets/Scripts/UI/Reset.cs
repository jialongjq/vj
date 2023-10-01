using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;
using UnityEngine.Audio;

public class Reset : MonoBehaviour
{
    [SerializeField] private AudioMixer mixer;

    const string MIXER_MUSIC = "MusicVolume";
    const string MIXER_SFX = "SFXVolume";
    
    public void resetPlayerPrefs() {
        PlayerPrefs.DeleteAll();
        Debug.Log("RESET!");
        mixer.SetFloat(MIXER_SFX, 1);
        mixer.SetFloat(MIXER_MUSIC, 1);
        SceneManager.LoadScene("GameScene");
    }
}
