using UnityEngine;

public class TransparentFallingObject : MonoBehaviour
{
    private float minY;  // Minimum Y position to start fading
    private float maxY;   // Maximum Y position to finish fading
    public float fadeSpeed;  // Speed at which the object fades

    private MeshRenderer objectRenderer;
    private Material[] originalMaterials;

    private Transform fallingObject; 

    private void Start()
    {
        fallingObject = transform.parent.transform.parent.transform;
        maxY = fallingObject.position.y - 3;
        minY = fallingObject.position.y - 4f;
        // Get the MeshRenderer component of the object
        objectRenderer = GetComponent<MeshRenderer>();

        // Store the original materials and their alpha values
        originalMaterials = objectRenderer.materials;
    }

    private void Update()
    {
        if (fallingObject.position.y < maxY && fallingObject.position.y > minY)
        {
            float transparency = Mathf.InverseLerp(minY, maxY, fallingObject.position.y);

            Material[] modifiedMaterials = new Material[originalMaterials.Length];

            for (int i = 0; i < originalMaterials.Length; i++)
            {
                Color materialColor = originalMaterials[i].color;
                if (transparency < 0.1) transparency = 0;
                materialColor.a = transparency;

                Material modifiedMaterial = new Material(originalMaterials[i]);
                modifiedMaterial.color = materialColor;

                modifiedMaterials[i] = modifiedMaterial;
            }

            objectRenderer.materials = modifiedMaterials;
        }
    }
}