using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SawController : MonoBehaviour
{
    public char direction;
    public float maxDistance;

    public CharacterController controller;

    public float speed = 1;
    private float gravity = -9.81f;
    public float gravityMultiplier = 1.5f;
    private float velocity = 0;
    private Vector3 directionVec = new Vector3(0, 0, 0);
    private float movedDistance = 0;
    private float time = 0;

    private Vector3 iniPos;
    private bool atIni = false;
    private Vector3 finPos;
    private bool active = true;

    // Start is called before the first frame update
    void Start()
    {
        iniPos = new Vector3(transform.position.x, 0, transform.position.z);
        if (direction == 'Z')
        {
            finPos = new Vector3(transform.position.x, 0, transform.position.z + maxDistance);
            directionVec = new Vector3(0, 0, 1);
        }
        else
        {
            finPos = new Vector3(transform.position.x + maxDistance, 0, transform.position.z);
            directionVec = new Vector3(1, 0, 0);
        }
    }

    // Update is called once per frame
    void Update()
    {
        ApplyGravity();
        Move();
        time += Time.deltaTime;
        if (time > 0.1f)
        {
            time = 0;
            Rotate();
        }
        if (Input.GetKeyDown(KeyCode.G))
        {
            active = !active;
            gameObject.SetActive(active);
        }
    }

    void ApplyGravity()
    {
        if (controller.isGrounded && velocity < 0)
        {
            velocity = -1.0f;
        }
        velocity += gravity * gravityMultiplier * Time.deltaTime;
        directionVec.y = velocity;
    }

    void Move()
    {
        float distance = speed * Time.deltaTime;
        movedDistance += distance;
        if (movedDistance > maxDistance)
        {
            movedDistance = 0;
            directionVec.x *= -1;
            directionVec.z *= -1;

            if (atIni)
            {
                atIni = false;
                transform.position = iniPos + new Vector3(0, transform.position.y, 0);
            }
            else
            {
                atIni = true;
                transform.position = finPos + new Vector3(0, transform.position.y, 0);
            } 
        }
        else
        {
            controller.Move(directionVec * distance);
        }
    }

    void Rotate()
    {
        if (direction == 'Z') transform.Rotate(90, 0, 0);
        else transform.Rotate(0, 0, 90);
    }
}
