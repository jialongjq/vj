using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ShurikenController : MonoBehaviour
{
    public char direction;
    private Vector3 directionVector;
    private float rotationSpeed = 500;
    private float speed = 15;
    private bool triggered = false;

    void Start()
    {
        if (direction == 'Z')
        {
            directionVector = new Vector3(0, 0, 1);
        }
        else if (direction == 'X')
        {
            directionVector = new Vector3(1, 0, 0);
        }
    }
    void Update()
    {
        if (triggered)
        {
            float rotationAngle = Time.deltaTime * rotationSpeed;
            transform.Rotate(0, 0, rotationAngle);
            transform.position = transform.position - (speed * Time.deltaTime) * directionVector;
        }
    }

    public void Trigger()
    {
        triggered = true;
    }
}
