using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraController : MonoBehaviour
{
    public Transform player;

    private Vector3 target;

    private Vector3 offset;

    private float y = 0;

    public float smoothTime;
    private Vector3 velocity = Vector3.zero;

    private float rotationSpeed = 25;
    private float remainingAngleClockwise = 20;
    private bool start = false;

    public void StartGame()
    {
        start = true;
    }

    void Start()
    {
       
    }

    // Update is called once per frame
    void Update()
    {
        if (start) 
        {
            float rotationAngle = Time.deltaTime * rotationSpeed;
            transform.Rotate(0, rotationAngle, 0);
            remainingAngleClockwise -= rotationAngle;
            if (remainingAngleClockwise < 0)
            {
                remainingAngleClockwise = 0;
                transform.rotation = Quaternion.Euler(new Vector3(0, 0, 0));
            }
            if (player && player.transform.position.z < 175)
            {
                if (player.GetComponent<PlayerController>().IsOnBoat())
                {
                    target = new Vector3(player.transform.position.x, y+1, player.transform.position.z);
                }
                else
                {
                    float c = player.transform.position.x + player.transform.position.z;
                    target = new Vector3(c / 2, y - 1, c / 2) + offset;
                }
                transform.position = Vector3.SmoothDamp(transform.position, target, ref velocity, smoothTime);
            }
        }
    }

    public void UpdatePosition(Vector3 currentBlockPosition, Vector3 midBlockPosition) {
        float c = currentBlockPosition.z + currentBlockPosition.x;
        float d = midBlockPosition.z - midBlockPosition.x;
        Vector3 midPoint = new Vector3((c-d)/2, 0, (c+d)/2);
        offset = midPoint - new Vector3(c/2, 0, c/2);
        y = midBlockPosition.y;
    }
}
