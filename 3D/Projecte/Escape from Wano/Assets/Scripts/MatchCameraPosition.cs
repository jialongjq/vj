using UnityEngine;

public class MatchCameraPosition : MonoBehaviour
{
    private Camera mainCamera;

    private void LateUpdate()
    {
        if (mainCamera == null)
        {
            mainCamera = FindMainCamera();
            if (mainCamera == null)
            {
                Debug.LogWarning("Main camera not found in the scene.");
                return;
            }
        }

        transform.position = mainCamera.transform.position;
    }

    private Camera FindMainCamera()
    {
        Camera[] cameras = Camera.allCameras;
        print(cameras);
        foreach (Camera camera in cameras)
        {
            if (camera.CompareTag("MainCamera"))
            {
                return camera;
            }
        }
        return null;
    }
}
