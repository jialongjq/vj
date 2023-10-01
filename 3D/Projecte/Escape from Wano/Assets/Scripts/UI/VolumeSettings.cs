using UnityEngine;
using UnityEngine.Audio;
using UnityEngine.UI;

public class VolumeSettings : MonoBehaviour
{
    [SerializeField] private AudioMixer mixer;
    // [SerializeField] private AudioSource ostSound;
    [SerializeField] private Slider musicSlider;
    [SerializeField] private Slider SFXSlider;

    const string MIXER_MUSIC = "MusicVolume";
    const string MIXER_SFX = "SFXVolume";

    void Start() {
        musicSlider.value = 1;
        SFXSlider.value = 1;
        if (PlayerPrefs.HasKey(MIXER_MUSIC)) {
            musicSlider.value = PlayerPrefs.GetFloat(MIXER_MUSIC);
        }
        if (PlayerPrefs.HasKey(MIXER_SFX)) {
            SFXSlider.value = PlayerPrefs.GetFloat(MIXER_SFX);
        }
    }
    void Awake() {
        musicSlider.onValueChanged.AddListener(SetMusicVolume);
        SFXSlider.onValueChanged.AddListener(SetSFXVolume);
    }

    public void SetMusicVolume(float value) {
        mixer.SetFloat(MIXER_MUSIC, Mathf.Log(value)*20);
        PlayerPrefs.SetFloat(MIXER_MUSIC, value);
    }

    public void SetSFXVolume(float value) {
        mixer.SetFloat(MIXER_SFX, Mathf.Log(value)*20);
        PlayerPrefs.SetFloat(MIXER_SFX, value);
    }
}
