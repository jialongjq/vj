using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;

public class CoinsCollected : MonoBehaviour
{
    private TMP_Text coinsText;
    private int coinsCollected;

    void Start()
    {
        coinsCollected = 0;
        coinsText = GetComponent<TMP_Text>();
        coinsCollected = PlayerPrefs.GetInt("CoinsCollected");
        coinsText.text = "" +coinsCollected;
        
    }

    public void AddCoins() {
        coinsCollected += 1;
        print(coinsCollected);
        PlayerPrefs.SetInt("CoinsCollected", coinsCollected);
    }
   
    void Update() {
        coinsText.text = ""+coinsCollected;
    }
}
