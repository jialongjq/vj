using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using System.Linq;
using TMPro;

public class PanelManager : Singleton<PanelManager>
{
    public PlayerController playerController;
    public AudioSource click;
    public AudioSource close;

    private List<PanelInstanceModel> _listInstances = new List<PanelInstanceModel>();

    private ObjectPool _objectPool;
    private bool startAfterTimer = false;
    private bool gameOver = false;
   
   IEnumerator EnableEndPanel()
    {
        yield return new WaitForSeconds(1f);
        if (!gameOver) {
            gameOver = true;
            changeGameStatus("EndPanel");
        }
    }

    private void Start()
    {
        _objectPool = ObjectPool.Instance;
        enableScore(false);
        
        // changeGameStatus("MainMenuPanel");

        ShowPanel("MainMenuPanel", PanelShowBehaviour.HIDE_PREVIOUS);
        GameStateManager.Instance.SetState(GameState.Main);
    }

    void Update()
    {
        if (startAfterTimer && !gameOver) {
            if (disableTimer()) {
                startAfterTimer = false;
                GameStateManager.Instance.SetState(GameState.Gameplay);
            }
        }
        else {
            GameState currentGameState = GameStateManager.Instance.CurrentGameState;
            if (currentGameState == GameState.Gameplay && playerController.CanRestart()) {
                StartCoroutine(EnableEndPanel());
            }
            else if (Input.GetKeyDown(KeyCode.Space)) {
                if (currentGameState == GameState.End) {
                    changeGameStatus("MainMenuPanel");
                }
                else if (currentGameState == GameState.Main){ 
                    print("MAIM");
                    enableScore(true);
                    ShowPanel("GamePanel", PanelShowBehaviour.HIDE_PREVIOUS);
                    GameStateManager.Instance.SetState(GameState.Gameplay);
                }
            }
            else if(Input.GetKeyDown(KeyCode.Escape))
            {
                if (currentGameState == GameState.Gameplay) {
                    Debug.Log("PAUSE");
                    changeGameStatus("PausePanel");
                }
                else if (currentGameState == GameState.Paused || currentGameState == GameState.End){
                    changeGameStatus("MainMenuPanel");
                }
                else {
                    QuitGame();
                }
            }
        }
    }


    public void ShowPanel(string panelId, PanelShowBehaviour behaviour = PanelShowBehaviour.KEEP_PREVIOUS) {
        GameObject panelInstance = _objectPool.GetObjectFromPool(panelId);        
        Debug.Log(panelId);
        if (panelInstance != null) 
        {
            if (behaviour == PanelShowBehaviour.HIDE_PREVIOUS && GetAmountPanelsInQueue() > 0) {
                var lastPanel = GetLastPanel();
                if (lastPanel != null) {
                    lastPanel.PanelInstance.SetActive(false);
                }
            }
            _listInstances.Add(new PanelInstanceModel {
                PanelId = panelId,
                PanelInstance = panelInstance
            });
        }
        else {
            Debug.Log("ERROR");
        }
    }

    public void HideLastPanel() {
        // Make sure we do have a panel showing
        if (AnyPanelShowing())
        {
            // Get the last panel showing
            var lastPanel = GetLastPanel();

            // Remove it from the list of instances
            _listInstances.Remove(lastPanel);
            
            // Pool the object
            _objectPool.PoolObject(lastPanel.PanelInstance);

            // If we have more panels in the queue
           if (GetAmountPanelsInQueue() > 0)
           {
               lastPanel = GetLastPanel();
               if (lastPanel != null && !lastPanel.PanelInstance.activeInHierarchy)
               {
                   lastPanel.PanelInstance.SetActive(true);
               }
           }
        //    Debug.Log(lastPanel.PanelInstance);
        }
    }

    PanelInstanceModel GetLastPanel() {
        return  _listInstances[_listInstances.Count-1];
    }
    
    public int GetAmountPanelsInQueue() {
        return _listInstances.Count;
    }
    
    public bool AnyPanelShowing() {
        return GetAmountPanelsInQueue() > 0;
    }

    public void QuitGame() {
        Debug.Log("QUIT GAMSE");
        Application.Quit();
    }

    public void changeGameStatus(string panelId) {
            GameState currentGameState = GameStateManager.Instance.CurrentGameState;
            if (panelId == "GamePanel") {
                enableScore(true);
                initializeTimer();
                ShowPanel("GamePanel", PanelShowBehaviour.HIDE_PREVIOUS);
            }
            else if (panelId == "PausePanel") {
                if (!gameOver) {
                    click.Play();   
                    GameStateManager.Instance.SetState(GameState.Paused);
                    ShowPanel("PausePanel", PanelShowBehaviour.HIDE_PREVIOUS);
                }
            }
            else if (panelId == "MainMenuPanel") {
                enableScore(false);
                gameOver = false;
                ShowPanel("MainMenuPanel", PanelShowBehaviour.HIDE_PREVIOUS);
                if (currentGameState != GameState.Others) {
                    click.Play();
                    SceneManager.LoadScene("GameScene");
                }
                else {
                    close.Play();
                }
                GameStateManager.Instance.SetState(GameState.Main);
            }
            else if (panelId == "EndPanel") {
                if (currentGameState == GameState.Paused) click.Play();
                enableScore(false);
                ShowPanel("EndPanel", PanelShowBehaviour.HIDE_PREVIOUS);
                GameStateManager.Instance.SetState(GameState.End);
            }          
            else {
                click.Play();
                ShowPanel(panelId, PanelShowBehaviour.HIDE_PREVIOUS);
                GameStateManager.Instance.SetState(GameState.Others);
            }
            print(GameStateManager.Instance.CurrentGameState);
    }

    private void enableScore(bool enable) {
        GameObject canvasObject = GameObject.Find("UI/Canvas/Panel");
        if (canvasObject != null) {
                {   
                    print("ASDF");
                    Transform childTransform = canvasObject.transform.Find("Score");
                    if (childTransform != null) {
                        GameObject Score = childTransform.gameObject;
                        Score.SetActive(enable);
                        print("·FOUND");
                    } 
                    
                }
        }
    }

    public void initializeTimer() {
        Debug.Log("FIFIFI");
        GameStateManager.Instance.SetState(GameState.Paused);
        GameObject canvasObject = GameObject.Find("UI/Canvas");
        if (canvasObject != null)
        {
            Transform childTransform = canvasObject.transform.Find("CountDownTimer");
            if (childTransform != null) {
                GameObject timer = childTransform.gameObject;
                timer.SetActive(true);
                startAfterTimer = true;
            } 
        }
    }

    public bool disableTimer() {
        GameObject canvasObject = GameObject.Find("UI/Canvas");
        if (canvasObject != null)
        {
            Transform childTransform = canvasObject.transform.Find("CountDownTimer");
            if (childTransform != null) {
                GameObject timer = childTransform.gameObject;
                CountDownTimer countDownTimerScript = timer.GetComponent<CountDownTimer>();
                if (countDownTimerScript.countDownDisplay.text == "GO")
                {
                    timer.SetActive(false);
                    GameStateManager.Instance.SetState(GameState.Gameplay); 
                    return true;
                }
            }
        }
        return false;
    }    
}


