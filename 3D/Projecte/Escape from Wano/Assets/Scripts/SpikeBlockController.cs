using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SpikeBlockController : MonoBehaviour
{
    private Vector3 iniPos;
    private Vector3 finalPos;
    private bool triggered = false;
    private float upwardsSpeed = 5f;
    private float downwardsSpeed = 0.1f;
    private bool goingUpwards = false;
    private bool goingDownwards = false;
    private float holdingTime = 0;

    // Start is called before the first frame update
    void Start()
    {
        iniPos = transform.position;
        finalPos = iniPos + new Vector3(0, 3 * (1.0f / 16.0f), 0);
    }

    // Update is called once per frame
    void Update()
    {
        if (triggered)
        {
            transform.parent.GetComponent<SoundController>().PlaySound();
            triggered = false;
            goingUpwards = true;
        }
        if (goingUpwards)
        {
            transform.position = transform.position + new Vector3(0, upwardsSpeed * Time.deltaTime, 0);
            if (transform.position.y > finalPos.y)
            {
                goingUpwards = false;
                goingDownwards = true;
                transform.position = finalPos;
            }
        }
        if (goingDownwards)
        {
            holdingTime += Time.deltaTime;
            if (holdingTime >= 1)
            {
                transform.position = transform.position + new Vector3(0, -downwardsSpeed * Time.deltaTime, 0);
                if (transform.position.y < iniPos.y)
                {
                    goingDownwards = false;
                    transform.position = iniPos;
                    holdingTime = 0;
                }
            }
        }
    }

    void OnTriggerEnter(UnityEngine.Collider collider) {
        if (collider.gameObject.tag == "Player" && !goingDownwards && !goingUpwards) {
            triggered = true;
        }
    }
}
