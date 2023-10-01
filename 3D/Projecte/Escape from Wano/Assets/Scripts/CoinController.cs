using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CoinController : MonoBehaviour
{
    public AudioSource coinSound;
    private float rotationSpeed = 100;
    private float elevationSpeed = 10;
    private bool collected = false;
    private float totalElevation = 0;
    [SerializeField] private CoinsCollected coinsCollected;

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        float rotationAngle = Time.deltaTime * rotationSpeed;
        transform.Rotate(0, 0, -rotationAngle);

        if (collected)
        {
            transform.parent.position = transform.parent.position + new Vector3(0, elevationSpeed * Time.deltaTime, 0);
            totalElevation += elevationSpeed * Time.deltaTime;
            if (totalElevation > 1)
            {
                coinsCollected.AddCoins();
                Destroy(transform.parent.gameObject);
            }
        }
    }

    void OnTriggerEnter(UnityEngine.Collider collider)
    {
        if (collider.gameObject.tag == "Player" && !collected)
        {
            transform.parent.transform.parent.GetComponent<SoundController>().PlaySound();
            collected = true;
        }
    }
}
