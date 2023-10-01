using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ActivateTile : MonoBehaviour
{
    public Material activatedMaterial;

    public void Activate()
    {
        MeshRenderer meshRenderer = GetComponent<MeshRenderer>();
        meshRenderer.material = activatedMaterial;
    }
}
