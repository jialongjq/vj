using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using TMPro;

public class CountDownTimer : MonoBehaviour
{
    public AudioSource clock;
    public int countDownTimer;
    public TMP_Text countDownDisplay;

    private Coroutine countDownCoroutine;

    void OnEnable()
    {
        ResetCountdown();
    }

    void Start()
    {
        ResetCountdown();
    }

    void ResetCountdown()
    {
        countDownTimer = 3;
        if (countDownCoroutine != null)
            StopCoroutine(countDownCoroutine);

        countDownCoroutine = StartCoroutine(CountDown());
    }

    IEnumerator CountDown()
    {
        while (countDownTimer > 0)
        {
            print("psd");
            clock.Play();
            countDownDisplay.text = countDownTimer.ToString();
            yield return new WaitForSeconds(1f);
            countDownTimer--;
        }

        countDownDisplay.text = "GO!";
        clock.Play();       
        yield return new WaitForSeconds(0.3f);
        countDownDisplay.text = "GO";
        // countDownDisplay.gameObject.SetActive(false);
        // GameStateManager.Instance.SetState(GameState.Gameplay);
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}