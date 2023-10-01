using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BlockCounter : MonoBehaviour
{
    private GameObject[] specialBlocks;
    private Vector3 currentBlockPosition;
    private Vector3 nextBlockPosition;
    private Vector3 midBlockPosition;
    private float blocksUntilNext;
    private char currentDirection;

    void Start()
    {
        currentBlockPosition = new Vector3(0, 0, 0);
        currentDirection = 'Z';
        if (specialBlocks == null) {
            specialBlocks = GameObject.FindGameObjectsWithTag("SpecialBlock");
        }
        foreach (GameObject specialBlock in specialBlocks) {
            if (specialBlock.transform.position.x == 0) {
                nextBlockPosition = specialBlock.transform.position;
                blocksUntilNext = nextBlockPosition.z;
                midBlockPosition = currentBlockPosition + new Vector3(0, 0, blocksUntilNext / 2);
                CameraController script = FindObjectOfType<CameraController>();
                script.UpdatePosition(currentBlockPosition, midBlockPosition);
                break;
            }
        }
    }

    void OnTriggerExit(UnityEngine.Collider collider) {
        if (collider.gameObject.tag == "Tile") {
            if (currentDirection == 'Z') currentDirection = 'X';
            else currentDirection = 'Z';
            currentBlockPosition = collider.gameObject.transform.parent.position;
            foreach (GameObject specialBlock in specialBlocks) {
                Vector3 blockPosition = specialBlock.transform.position;
                if (currentDirection == 'Z') {
                    if (blockPosition.x == currentBlockPosition.x && blockPosition.z > currentBlockPosition.z) {
                        nextBlockPosition = blockPosition;
                        blocksUntilNext = nextBlockPosition.z - currentBlockPosition.z;
                        midBlockPosition = new Vector3(currentBlockPosition.x, nextBlockPosition.y, currentBlockPosition.z + blocksUntilNext / 2);
                        break;
                    }
                }
                else {
                    if (blockPosition.z == currentBlockPosition.z && blockPosition.x > currentBlockPosition.x) {
                        nextBlockPosition = blockPosition;
                        blocksUntilNext = nextBlockPosition.x - currentBlockPosition.x;
                        midBlockPosition = new Vector3(currentBlockPosition.x + blocksUntilNext / 2, nextBlockPosition.y, currentBlockPosition.z);
                        break;
                    }
                }
            }
            CameraController script = FindObjectOfType<CameraController>();
            script.UpdatePosition(currentBlockPosition, midBlockPosition);
        }
    }
}
