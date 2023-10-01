using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class WaterFlow : MonoBehaviour
{
    // Start is called before the first frame update
    public Material water1;
    public Material water2;
    public Material water3;

    private Material[] water;
    private float time = 0;
    private int waterIndex = 0;
    private MeshRenderer meshRenderer;

    void Start()
    {
        water = new Material[3];
        water[0] = water1;
        water[1] = water2;
        water[2] = water3;
        meshRenderer = GetComponent<MeshRenderer>();
        meshRenderer.material = water1;
    }

    // Update is called once per frame
    void Update()
    {
        time += Time.deltaTime;
        if (time > (1/3.0f))
        {
            time = 0;
            waterIndex += 1;
            if (waterIndex > 2)
            {
                waterIndex = 0;
            }
            meshRenderer.material = water[waterIndex];
        }
    }
}
