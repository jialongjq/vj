using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SoundController : MonoBehaviour
{
    public AudioSource coinSound;

    public void PlaySound()
    {
        coinSound.Play();
    }
}
