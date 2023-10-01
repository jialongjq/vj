using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GodModeController : MonoBehaviour
{
    private PlayerController script;
    private bool godMode = false;
    GameObject[] saws;
    void Start()
    {
        script = this.transform.parent.GetComponent<PlayerController>();
        saws = GameObject.FindGameObjectsWithTag("Saw");
    }

    void Update()
    {
        if (Input.GetKeyDown(KeyCode.G))
        {
            godMode = !godMode;
            if (godMode)
            {
                foreach (GameObject saw in saws)
                {
                    saw.SetActive(false);
                }
            }
            else
            {
                foreach (GameObject saw in saws)
                {
                    saw.SetActive(true);
                }
            }
        }
    }

    void OnTriggerStay(UnityEngine.Collider collider)
    {
        if (collider.gameObject.tag == "Tap" && godMode)
        {
            script.Tap();
            Destroy(collider.gameObject);
        }
        if (collider.gameObject.tag == "SpecialTap" && godMode && script.IsGrounded())
        {
            script.Tap();
            Destroy(collider.gameObject);
        }
    }
}
