using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class ScoreManager : MonoBehaviour
{
    public static int score;
    public static int highscore;

    private Text text;

    void Start()
    {
        text = GetComponent<Text>();

        score = 0;

        highscore = PlayerPrefs.GetInt("highscore", highscore);
    }

    void Update()
    {
        if (score > highscore)
        {
            highscore = score;
        }
        
        text.text = "Score: " + score + "\nHighscore: " + highscore;

        PlayerPrefs.SetInt("highscore", highscore);
    }

    public static void AddPoints(int pointsToAdd)
    {
        Debug.Log("ADDD");
        score += pointsToAdd;
    }

    public static void Reset()
    {
        score = 0;
    }
}
