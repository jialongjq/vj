// using System.Collections;
// using System.Collections.Generic;
// using UnityEngine;
// using UnityEngine.UI;

// public class PauseMenu : MonoBehaviour
// {
//     public static bool GameIsPaused = false;
//     public GameObject PauseMenuUI;
    
//     void update () {
//         print("ENTRO");
//         if (Input.GetKeyDown(KeyCode.Escape)) {
//             print("holaa");
//             if (GameIsPaused) {
//                 Resume();
//             }
//             else {
//                 Pause();
//             }
//         }
//     }

//     void Resume () {
//         PauseMenuUI.SetActive(false);
//         // Time.timeScale = 1f;
//         GameIsPaused = false;
//     }

//     void Pause () {
//         PauseMenuUI.SetActive(true);
//         // Time.timeScale = 0f;
//         GameIsPaused = true;
//     }
// }
