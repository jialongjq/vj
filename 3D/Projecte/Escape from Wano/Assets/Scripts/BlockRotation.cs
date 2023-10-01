using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BlockRotation : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        int randomValue = Random.Range(0, 4);
        // 1 faces to Z
        // 2 faces to X
        // add more probability, since having too much consequently is not good looking
        if (randomValue == 1 || randomValue == 2)
        {
            float prob = Random.value;
            if (prob < 0.5)
            {
                if (prob < 0.25) randomValue = 0;
                else randomValue = 3;
            }
        }
        transform.Rotate(0f, 90 * randomValue, 0f);
    }
}
