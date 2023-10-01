using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FallingBlockController : MonoBehaviour
{
    private bool soundPlayed = false;

    public char shakingDirection;
    public GameObject reactiveFallingBlock;
    public bool reactive;
    public float waitTime = 0;
    private float triggeredTimeElapsed = 0;

    private Vector3 directionVector;
    private float shakingSpeed = -2f;
    private float distanceToCenter = 0;
    private float fallingSpeed = 5;
    private bool falling = false;
    private bool shaking = false;

    private float gravity = -9.81f;
    private float gravityMultiplier = 0.5f;
    private float velocity = 0;

    private bool triggered = false;

    private Vector3 iniPos;

    void Start()
    {
        if (shakingDirection == 'Z')
        {
            directionVector = new Vector3(0, 0, 1);
        }
        else if (shakingDirection == 'X')
        {
            directionVector = new Vector3(1, 0, 0);
        }
        else
        {
            directionVector = new Vector3(0, -1, 0);
        }
        iniPos = transform.position;
    }

    // Update is called once per frame
    void Update()
    {
        if (!reactive || (reactive && triggered))
        {
            if (shaking && !falling)
            {
                if (!reactive && !soundPlayed)
                {
                    transform.parent.GetComponent<SoundController>().PlaySound();
                    soundPlayed = true;
                }
                triggeredTimeElapsed += Time.deltaTime;
                if (triggeredTimeElapsed > waitTime)
                {
                    transform.position = transform.position + ((shakingSpeed * Time.deltaTime) * directionVector);
                    distanceToCenter += shakingSpeed * Time.deltaTime;
                    if (distanceToCenter < -0.1f)
                    {
                        shakingSpeed *= -1;
                        transform.position = iniPos + (-0.1f * directionVector);
                        distanceToCenter = -0.1f;
                    }
                    else if (distanceToCenter > 0.1f)
                    {
                        shakingSpeed *= -1;
                        transform.position = iniPos + (0.1f * directionVector);
                        distanceToCenter = 0.1f;
                        falling = true;
                        if (reactiveFallingBlock != null)
                        {
                            reactiveFallingBlock.GetComponent<FallingBlockController>().TriggerReactive();
                        }
                    }
                }
            }
            else if (falling)
            {
                if (shakingDirection != 'Y' && distanceToCenter != 0)
                {
                    transform.position = transform.position + ((shakingSpeed * Time.deltaTime) * directionVector);
                    distanceToCenter += shakingSpeed * Time.deltaTime;
                    if (distanceToCenter < 0)
                    {
                        distanceToCenter = 0;
                        transform.position = iniPos;
                    }
                }
                else
                {
                    velocity += gravity * gravityMultiplier * Time.deltaTime;
                    transform.position = transform.position + new Vector3(0, velocity * fallingSpeed * Time.deltaTime, 0);
                    if (transform.position.y < (iniPos.y - 10))
                    {
                        Destroy(gameObject);
                    }
                }
            }
        }
    }

    void OnTriggerEnter(UnityEngine.Collider collider)
    {
        if (collider.gameObject.tag == "Player" && !shaking)
        {
            shaking = true;
        }
    }

    public void TriggerReactive()
    {
        triggered = true;
        shaking = true;
    }
}
