 using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class PlayerController : MonoBehaviour
{
    public AudioSource jumpSound;
    public AudioSource secondJumpSound;
    public AudioSource activateTileSound;
    public AudioSource hitSound;
    public AudioSource groundSound;
    public AudioSource enterWaterSound;
    public AudioSource shurikenHitSound;
    public AudioSource sawHitSound;

    public AudioSource laughSound;
    public AudioSource laugh2Sound;
    public AudioSource dying1Sound;
    public AudioSource dying2Sound;
    public AudioSource dying3Sound;
    public AudioSource dying4Sound;
    public AudioSource dying5Sound;

    public AudioSource ostSound;

    private AudioSource[] dyingSounds;

    private bool canPlayWaterSound = true;

    public CharacterController controller;

    private int counter = 0;

    public float speed = 5;
    private float originalSpeed = 0;

    private float gravity = -9.81f;
    public float gravityMultiplier = 1.5f;
    private float velocity = 0;
    public float jumpPower = 3.5f;
    private bool secondJump = false;

    private Vector3 zDirection = new Vector3(0f, 0f, 1f);
    private Vector3 xDirection = new Vector3(1f, 0f, 0f);
    private Vector3 direction = new Vector3(0f, 0f, 1f);

    private bool onSpecialBlock = false;
    private Vector3 currentSpecialBlock = new Vector3(0, 0, 0);
    private char currentDirection = 'Z';
    private bool changedDirection = false;

    private float desiredPos = 0;
    private float offsetSpeed = 1.5f;
    private float remainingOffsetX = 0;
    private float remainingOffsetZ = 0;

    private float rotationSpeed = 1000;
    private float remainingAngleClockwise = 0;
    private float remainingAngleCounterclockwise = 0;

    private bool start = false;
    private bool dead = false;

    private GameObject tile = null;

    public GameObject objectToAnimate;

    private bool grounded = true;
    private bool jumped = false;
    private bool jumpStarted = false;

    private bool goal = false;

    private GameObject boat;

    private bool godMode = false;
    private bool onBoat = false;

    private bool canRestart = false;

    void Start()
    {
        GameObject[] boats = GameObject.FindGameObjectsWithTag("Boat");
        boat = boats[0];
        dyingSounds = new AudioSource[5];
        dyingSounds[0] = dying1Sound; dyingSounds[1] = dying2Sound; dyingSounds[2] = dying3Sound; dyingSounds[3] = dying4Sound; dyingSounds[4] = dying5Sound;
        originalSpeed = speed;
        Animator anim = objectToAnimate.GetComponent<Animator>();
        anim.SetFloat("speedMultiplier", 1f);
    }

    public int GetCounter() {
        return counter;
    }

    public bool CanRestart() {
        return canRestart;
    }
    public void SetGrounded(bool grounded)
    {
        this.grounded = grounded;
    }

    public bool IsGrounded()
    {
        return grounded;
    }

    public bool IsOnBoat()
    {
        return onBoat;
    }

    void Update()
    {
        if (canRestart && Input.GetKeyDown(KeyCode.Space))
        {
            SceneManager.LoadScene("GameScene");
        }

        if (start && !goal) {

            if (jumpStarted && !IsGrounded())
            {
                jumped = true;
            }

            if (IsGrounded())
            {
                if (jumped)
                {
                    groundSound.Play();
                    jumped = false;
                    objectToAnimate.GetComponent<Animator>().Play("Running");
                }
                if (secondJump)
                {
                    secondJump = false;
                }
            }
            Move();
            ApplyGravity();
            if (Input.GetKeyDown(KeyCode.Space)) {
                Tap();
            }
        }
        else if (start && goal)
        {
            if (transform.position.z < boat.transform.position.z)
            {
                Move();
            }
            else
            {
                if (!onBoat)
                {
                    float prob = Random.value;
                    if (prob > 0.5)
                    {
                        laughSound.Play();
                    }
                    else
                    {
                        laugh2Sound.Play();
                    }
                    objectToAnimate.GetComponent<Animator>().Play("Sit");
                    onBoat = true;
                    canRestart = true;
                    transform.parent = boat.transform;
                    BoatController boatController = boat.transform.GetComponent<BoatController>();
                    boatController.StartMoving();
                }
            }
        }
        else if (Input.GetKeyDown(KeyCode.Space)) {
            start = true;
            objectToAnimate.GetComponent<Animator>().Play("Running");
            FindObjectOfType<CameraController>().StartGame();
            ostSound.Play();
        }

        if (transform.position.y < currentSpecialBlock.y - 5.0f && !canRestart)
        {
            dead = true;
            direction.x = 0;
            direction.z = 0;
            canRestart = true;
            hitSound.Play();
            ostSound.Stop();
        }
        
        if (Input.GetKeyDown(KeyCode.G))
        {
            godMode = !godMode;
        }
    }

    void ApplyGravity() {
        if (IsGrounded() && velocity < 0) {
            velocity = -1.0f;
        }
        velocity += gravity * gravityMultiplier * Time.deltaTime;
        direction.y = velocity;
    }

    void Move() {
        controller.Move(direction * speed * Time.deltaTime);

        // From Z to X
        if (remainingAngleClockwise > 0) {
            float rotationAngle = Time.deltaTime * rotationSpeed;
            transform.Rotate(0, rotationAngle, 0);
            remainingAngleClockwise -= rotationAngle;
            if (remainingAngleClockwise < 0)
            {
                remainingAngleClockwise = 0;
                transform.rotation = Quaternion.Euler(new Vector3(0, 90, 0));
            }
        }
        // From X to Z
        if (remainingAngleCounterclockwise > 0) {
            float rotationAngle = Time.deltaTime * rotationSpeed;
            transform.Rotate(0, -rotationAngle, 0);
            remainingAngleCounterclockwise -= rotationAngle;
            if (remainingAngleCounterclockwise < 0)
            {
                remainingAngleCounterclockwise = 0;
                transform.rotation = Quaternion.Euler(new Vector3(0, 0, 0));
            }
        }

        if (remainingOffsetX != 0) {
            if (remainingOffsetX > 0) {
                controller.Move(xDirection * offsetSpeed * Time.deltaTime);
                remainingOffsetX = desiredPos - transform.position.x;
                if (remainingOffsetX < 0)
                {
                    transform.position = new Vector3(desiredPos, transform.position.y, transform.position.z);
                    remainingOffsetX = 0;
                }
            }
            else {
                controller.Move(-xDirection * offsetSpeed * Time.deltaTime);
                remainingOffsetX = desiredPos - transform.position.x;
                if (remainingOffsetX > 0)
                {
                    transform.position = new Vector3(desiredPos, transform.position.y, transform.position.z);
                    remainingOffsetX = 0;
                }
            }
        }

        if (remainingOffsetZ != 0) {
            if (remainingOffsetZ > 0) {
                controller.Move(zDirection * offsetSpeed * Time.deltaTime);
                remainingOffsetZ = desiredPos - transform.position.z;
                if (remainingOffsetZ < 0)
                {
                    transform.position = new Vector3(transform.position.x, transform.position.y, desiredPos);
                    remainingOffsetZ = 0;
                }
            }
            else {
                controller.Move(-zDirection * offsetSpeed * Time.deltaTime);
                remainingOffsetZ = desiredPos - transform.position.z;
                if (remainingOffsetZ > 0)
                {
                    transform.position = new Vector3(transform.position.x, transform.position.y, desiredPos);
                    remainingOffsetZ = 0;
                }
            }
        }
        
    }

    public void Tap() {
        if (!dead)
        {
            Debug.Log(transform.position);
            if (onSpecialBlock && !changedDirection)
            {
                changedDirection = true;
                counter += 1;
                ChangeDirection();
            }
            else if (IsGrounded())
            {
                objectToAnimate.GetComponent<Animator>().Play("FirstJump");
                jumpSound.Play();
                Jump();
            }
            else if (!secondJump)
            {
                secondJumpSound.Play();
                objectToAnimate.GetComponent<Animator>().Play("SecondJump");
                secondJump = true;
                Jump();
            }
            else
            {
                Debug.Log("WTF");
            }
        }
    }

    void Jump() {
        velocity = -1.0f;
        velocity += jumpPower;
        jumpStarted = true;
    }

    void DeathJump() {
        objectToAnimate.GetComponent<Animator>().Play("Death");
        int randomValue = Random.Range(0, 5);
        dyingSounds[randomValue].Play();
        dead = true;
        jumpPower = 4.5f;
        rotationSpeed = 200f;
        Jump();
        ChangeDirection();
    }

    void ChangeDirection() {
        if (currentDirection == 'Z') {
            desiredPos = currentSpecialBlock.z;
            if (!dead) remainingOffsetZ = desiredPos - transform.position.z;
            remainingAngleClockwise += 90;
            direction.z = 0.0f;
            direction.x = 1.0f;
            currentDirection = 'X';
        }
        else {
            desiredPos = currentSpecialBlock.x;
            if (!dead) remainingOffsetX = desiredPos - transform.position.x;
            remainingAngleCounterclockwise += 90;
            direction.x = 0.0f;
            direction.z = 1.0f;
            currentDirection = 'Z';
        }
        if (!dead)
        {
            activateTileSound.Play();
            ActivateTile activateTile = tile.GetComponent<ActivateTile>();
            activateTile.Activate();
        }
    }

    void OnTriggerEnter(UnityEngine.Collider collider) {
        if (collider.gameObject.tag == "Tile")
        {
            changedDirection = false;
            onSpecialBlock = true;
            currentSpecialBlock = collider.transform.position;
            tile = collider.gameObject;
        }
        if (collider.gameObject.tag == "Spike" && !dead) {
            DeathJump();
        }
        if (collider.gameObject.tag == "Saw" && !dead)
        {
            sawHitSound.Play();
            DeathJump();
        }
        if (collider.gameObject.tag == "Shuriken" && !dead)
        {
            shurikenHitSound.Play();
            DeathJump();
        }
        if (collider.gameObject.tag == "Muelle")
        {
            goal = true;
        }
    }

    void OnTriggerStay(UnityEngine.Collider collider)
    {
        if (collider.gameObject.tag == "WaterBlock")
        {
            Animator anim = objectToAnimate.GetComponent<Animator>();
            anim.SetFloat("speedMultiplier", 2.0f);
            speed = 1f;
            jumpPower = 3f;
            if (canPlayWaterSound)
            {
                enterWaterSound.Play();
                canPlayWaterSound = false;
            }
        }
    }


    void OnTriggerExit(UnityEngine.Collider collider) {
        if (collider.gameObject.tag == "Tile") {
            onSpecialBlock = false;
        }
        if (collider.gameObject.tag == "WaterBlock")
        {
            Animator anim = objectToAnimate.GetComponent<Animator>();
            anim.SetFloat("speedMultiplier", 1f);
            speed = originalSpeed;
            jumpPower = 3.5f;
            canPlayWaterSound = true;
        }
    }
}
