using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlaySound : MonoBehaviour
{
    public AudioSource soundAudio;


    public void playAudio()
    {
    if (soundAudio != null)
        {
            // Play the audio clip
            Debug.Log("asd");
            soundAudio.Play();
        }
        else
        {
            Debug.LogWarning("No AudioSource component found.");
        }
    }
}

