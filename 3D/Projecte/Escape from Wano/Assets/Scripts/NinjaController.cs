using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class NinjaController : MonoBehaviour
{
    public CharacterController controller;
    public GameObject shuriken;
    public GameObject objectToAnimate;
    private bool triggered = false;
    private float gravity = -9.81f;
    private float gravityMultiplier = 1.5f;
    private float velocity = 0;
    private float jumpPower = 2.5f;
    private Vector3 direction = new Vector3(0, 1, 0);
    public float speed = 5;
    private bool jumped = false;
    private bool done = false;
    private Vector3 iniPos;

    // Start is called before the first frame update
    void Start()
    {
        iniPos = transform.position;
        shuriken.SetActive(false);
    }

    // Update is called once per frame
    void Update()
    {
        if (triggered && !done)
        {
            ApplyGravity();
            if (controller.isGrounded && !jumped)
            {
                Jump();
            }
            else if (transform.position.y > (iniPos.y + 0.1f) && !jumped)
            {
                jumped = true;
            }
            else if (controller.isGrounded && jumped)
            {
                transform.parent.transform.parent.GetComponent<SoundController>().PlaySound();
                objectToAnimate.GetComponent<Animator>().Play("Throw");
                done = true;
                shuriken.SetActive(true);
                shuriken.GetComponent<ShurikenController>().Trigger();
            }
            controller.Move(direction * speed * Time.deltaTime);
        }
    }

    void Jump()
    {
        velocity = -1.0f;
        velocity += jumpPower;
    }

    void ApplyGravity()
    {
        if (controller.isGrounded && velocity < 0)
        {
            velocity = -1.0f;
        }
        velocity += gravity * gravityMultiplier * Time.deltaTime;
        direction.y = velocity;
    }


    void OnTriggerEnter(UnityEngine.Collider collider)
    {
        if (collider.gameObject.tag == "Player" && !triggered)
        {
            triggered = true;
        }
    }
}
