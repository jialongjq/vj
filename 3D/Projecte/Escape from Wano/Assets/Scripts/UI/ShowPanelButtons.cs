using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ShowPanelButtons : MonoBehaviour
{
    public string PanelId;
    public PanelShowBehaviour Behaviour;
    private PanelManager _panelManager;

    private void Start() {
        _panelManager = PanelManager.Instance;
    }

    public void QuitGame() {
        Application.Quit();
    }    
    public void DoShowPanel() {
        Debug.Log("fgbhg");
        // _panelManager.ShowPanel(PanelId, Behaviour);
        _panelManager.changeGameStatus(PanelId);
    }
}
