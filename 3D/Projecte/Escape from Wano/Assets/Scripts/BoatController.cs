using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BoatController : MonoBehaviour
{
    private bool canMove = false;
    private float horizontalSpeed = 1.5f;
    public float verticalSpeed = 20;
    private float distance = 0;
    private float y;
    
    void Start()
    {
        y = transform.position.y;
    }

    void Update()
    {
        if (canMove)
        {
            distance += horizontalSpeed * Time.deltaTime;
            if (transform.position.z < 175)
            {
                transform.position = new Vector3(transform.position.x, y + Mathf.Sin(distance * verticalSpeed) / 8, transform.position.z + horizontalSpeed * Time.deltaTime);
            }
            else
            {
                transform.position = new Vector3(transform.position.x, y + Mathf.Sin(distance * verticalSpeed) / 8, transform.position.z);
            }
        }
    }

    public void StartMoving()
    {
        canMove = true;
    }
}
