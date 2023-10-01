using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;

public class Score : MonoBehaviour
{
    public PlayerController playerController;
    private TMP_Text ScoreText;
    private static int score = 0;
    private int maxScore = 0;
    // Start is called before the first frame update
    void Start()
    {
        ScoreText = GetComponent<TMP_Text>();
        maxScore = PlayerPrefs.GetInt("HighScore");    
        Debug.Log(maxScore);
        score = 0;
        PlayerPrefs.SetInt("Score", score);    
    }

    public void AddScore(int newScore) {
        score += newScore;
    }

    // Update is called once per frame
    void UpdateScore()
    {   
        if (playerController.GetCounter() > score) {
            score = playerController.GetCounter();
            PlayerPrefs.SetInt("Score", score);    
        }
        if (score > maxScore)
        {
            maxScore = score;
            PlayerPrefs.SetInt("HighScore", maxScore);    
        }
        
        ScoreText.text = "Score: " + score + "";

    }
    void Update() {
        UpdateScore();
    }

    public static void AddPoints(int pointsToAdd)
    {
        score += pointsToAdd;
    }
}
