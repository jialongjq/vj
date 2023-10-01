using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameUIController : MonoBehaviour
{
    public PlayerController playerController;
    public MoveCube rollCubeController;
    public GameObject mainCamera;

    private float previousAnimeSpeed;
    
    void Start() {
        previousAnimeSpeed = playerController.objectToAnimate.GetComponent<Animator>().GetFloat("speedMultiplier");
    }
   void Awake()
    {
        GameStateManager.Instance.OnGameStateChanged += OnGameStateChanged;
    }
    
    void OnDestroy() 
    {
        GameStateManager.Instance.OnGameStateChanged -= OnGameStateChanged; 
    }

    private void OnGameStateChanged(GameState newGameState)
    {
        Debug.Log("ONON");
        bool pauseGame = (newGameState == GameState.Paused || newGameState == GameState.Others);
        playerController.enabled = !pauseGame;
        rollCubeController.enabled = !pauseGame;
        CameraController cont = mainCamera.GetComponent<CameraController>();
        cont.enabled = !pauseGame;
        Animator anim = playerController.objectToAnimate.GetComponent<Animator>();
        if (pauseGame) {
            Debug.Log("ENTRO1");
            previousAnimeSpeed = anim.GetFloat("speedMultiplier");
            playerController.objectToAnimate.GetComponent<Animator>().SetFloat("speedMultiplier", 0f);
            DisableScriptsOfType<SpikeBlockController>();
            DisableScriptsOfType<ShurikenController>();
            DisableScriptsOfType<NinjaController>();
            DisableScriptsOfType<WaterFlow>();
            DisableScriptsOfType<CoinController>();
            DisableScriptsOfType<SawController>(); 
            DisableScriptsOfType<FallingBlockController>();
            DisableScriptsOfType<BoatController>();  

        }
        else {
            anim.SetFloat("speedMultiplier", previousAnimeSpeed);
            Debug.Log("ENTRO");
            Debug.Log(newGameState);
            EnableScriptsOfType<SpikeBlockController>();
            EnableScriptsOfType<ShurikenController>();
            EnableScriptsOfType<NinjaController>();
            EnableScriptsOfType<WaterFlow>();
            EnableScriptsOfType<CoinController>(); 
            EnableScriptsOfType<SawController>(); 
            EnableScriptsOfType<FallingBlockController>();
            EnableScriptsOfType<BoatController>();
        }
        // Animator anim = playerController.objectToAnimate.GetComponent<Animator>();
        // if (!enabled) {
        //     previousAnimeSpeed = anim.GetFloat("speedMultiplier");
        //     anim.SetFloat("speedMultiplier", 0f);
        // }
        // else {
        //     anim.SetFloat("speedMultiplier", previousAnimeSpeed);
        // }
    }

      public void EnableScriptsOfType<T>() where T : MonoBehaviour
    {
        T[] scripts = FindObjectsOfType<T>();

        foreach (T script in scripts)
        {
            script.enabled = true;
        }
    }

    public void DisableScriptsOfType<T>() where T : MonoBehaviour
    {
        T[] scripts = FindObjectsOfType<T>();

        foreach (T script in scripts)
        {
            script.enabled = false;
        }
    }
}
