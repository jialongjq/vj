using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class IsGroundedController : MonoBehaviour
{
    private PlayerController playerController;
    // Start is called before the first frame update
    void Start()
    {
        playerController = this.transform.parent.GetComponent<PlayerController>();
    }

    void OnTriggerStay(UnityEngine.Collider collider)
    {
        if (collider.gameObject.tag == "Block" || collider.gameObject.tag == "SpecialBlock" || collider.gameObject.tag == "WaterBlock" || collider.gameObject.tag == "Muelle")
        {
            playerController.SetGrounded(true);
        }
    }

    void OnTriggerExit(UnityEngine.Collider collider)
    {
        if (collider.gameObject.tag == "Block" || collider.gameObject.tag == "SpecialBlock" || collider.gameObject.tag == "WaterBlock" || collider.gameObject.tag == "Muelle")
        {
            playerController.SetGrounded(false);
        }
    }
}
