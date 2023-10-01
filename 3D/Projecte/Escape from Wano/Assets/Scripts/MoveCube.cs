using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MoveCube : MonoBehaviour
{
    //public CharacterController controller;
    private float accumulateAngle, accumulateTime, deltaAngle, steps;
    private bool start, finish, isMoving, onSpecialBlock, collideWithPlayer, collideWithStair, collideWithStair2, collideWithSaws;
    private Vector3 stairHeight, moveHeight, collideBlockPosition;
    private int stayIn, status, rollsInAir, numStairs;
    private char currentDirection, nextDirection;
    private string stairName = "";
    private Vector3 direction, previousDirection, previousPosition, rotatePosition, moveTo, anchor, axis;
    private Quaternion adjustRotation;

    ////START
    void Start()
    {
        isMoving = false;
        onSpecialBlock = false;
        currentDirection = 'X';
        currentDirection = 'Z';
        if (currentDirection == 'X') {
            direction = Vector3.right;
            nextDirection = 'Z';
        }
        else {
            direction = Vector3.forward;
            nextDirection = 'X';
        }
        previousDirection = direction;
        previousPosition = transform.position;
        Physics.gravity = new Vector3(0, 0, 0);
        accumulateAngle = -5;
        accumulateTime = 0;
        deltaAngle = 5;
        steps = 90/deltaAngle;
        stairHeight = new Vector3 (0, 1/16f*5, 0);
        moveHeight = new Vector3 (0, 1/steps, 0);
        collideWithStair = false;
        collideWithStair2 = false;
        collideWithSaws = false;
        collideWithPlayer = false;
        rollsInAir = 0;
        numStairs = 0;
        stayIn = 1;
        finish = false;
        start = true;
    }

    ////DETECT GAME STATE
    void Awake()
    {
        GameStateManager.Instance.OnGameStateChanged += OnGameStateChanged;
    }
    
    void OnDestroy() 
    {
        GameStateManager.Instance.OnGameStateChanged -= OnGameStateChanged; 
    }

    private void OnGameStateChanged(GameState newGameState)
    {
        enabled = newGameState == GameState.Gameplay;
        // print(enabled);
    }
    

    private void rotateDeltaAngle(){
        transform.position = rotatePosition;
        transform.RotateAround(anchor, axis, deltaAngle);
        rotatePosition = transform.position;
        accumulateAngle += deltaAngle;
    }

    private void moveRollCube(Vector3 vec) {
        moveTo += vec;
        transform.position += moveTo;
    }

    ////UPADATE CUBE POSITION
    private void fixInRollingCube(float height, float width) {
        // print("Rolling...");
        transform.position = previousPosition + new Vector3(0, height, 0) + width*direction;
        // Debug.Log(transform.position.ToString("F4"));
        adjustRotation = adjustRotation*Quaternion.Euler(axis * 90f);
        transform.rotation = adjustRotation;
        rotatePosition = transform.position;
        anchor = transform.position + (Vector3.down + direction) * 0.5f;
        axis = Vector3.Cross(Vector3.up, direction);
        moveTo = new Vector3(0, 0, 0);
        accumulateAngle += deltaAngle;
    }
    ////FIX CUBE POSITION EVERY 90 GRADES
    private void fixEndInRollingCube(int nStairs, float width) {
        if (!finish) {
            transform.position = previousPosition - nStairs*stairHeight + width*direction;
            previousPosition = transform.position;
            // Debug.Log(transform.position.ToString("F4"));
            transform.rotation = adjustRotation;
            isMoving = false;
                        //         print("collide");
                        // print(stairName);
                        // print(rollsInAir);
                        // print(numStairs);
            accumulateAngle = 0;
            rollsInAir = 0;
            numStairs = 0;
        }
        else if (collideWithPlayer) {
            
        }
        else {
            start = false;
            Physics.gravity = new Vector3(0, -9.81f, 0);
        } 
    }


    ////FRAME UPDATE
    void Update()
    {
        if (start) {
            if (!isMoving)
            {
                // Debug.Log(onSpecialBlock);
                // Debug.Log(collideWithStair);
                if (onSpecialBlock) {
                    previousDirection = direction;
                    if (nextDirection == 'X')
                    {
                        direction = Vector3.right;
                        nextDirection = 'Z';
                    }
                    else
                    {
                        direction = Vector3.forward;
                        nextDirection = 'X';
                    }
                    currentDirection = nextDirection;
                    onSpecialBlock = false;
                }
                // if (collideWithStair)
                status = stayIn;
                // Debug.Log(transform.position);
                rotatePosition = transform.position;
                moveTo = new Vector3(0, 0, 0);

                anchor = transform.position + (Vector3.down + direction) * 0.5f;
                axis = Vector3.Cross(Vector3.up, direction);
                adjustRotation = Quaternion.Euler(axis * 90f);
                status = 1;
                if (stayIn > 1) {
                    status = 2;
                    if (stayIn < 4) {
                        rollsInAir = stayIn-1;
                        numStairs = rollsInAir;
                        if (collideWithStair) collideWithStair = false;
                        if (collideWithStair2) collideWithStair2 = false;
                    }
                    else {
                        rollsInAir = stayIn-3;
                        numStairs = 0;
                        if (collideWithSaws) collideWithSaws = false;
                    }
                }
                if (accumulateAngle < 0) {
                    ++accumulateAngle;
                    
                }
                else {
                    // print("LETS MOVE");
                    // print(stayIn);
                    stayIn = 1;
                    isMoving = true;
                }

            }
            else 
            {
                accumulateTime += Time.deltaTime;
                // print(Time.deltaTime);
            //    if (accumulateTime > 0.5f) {  
                // accumulateTime -= 0.5f;
                //STATUS: 1 - 0
                if (status == 1)
                {
                    
                    if (accumulateAngle < 90)
                    {
                        rotateDeltaAngle();
                    }
                    else
                    {
                        fixEndInRollingCube(0, 1);
                        // stayIn = -1;
                    }
                }
                //DOWN 1
                //STATUS: 2 - 1
                else if (status == 2)
                {
                    // print(rollsInAir);
                    if (collideWithStair) {
                        collideWithStair = false;
                        ++rollsInAir;
                        ++numStairs;
                        // print(stairName);
                    }
                    if (collideWithStair2) {
                        collideWithStair2 = false;
                        rollsInAir += 2;
                        numStairs += 2;
                    }
                    if (collideWithSaws) {
                        collideWithSaws = false;
                        ++rollsInAir;
                    }
                    if (accumulateAngle < 90)
                    {
                        rotateDeltaAngle();
                        moveRollCube(moveHeight);
                    }
                    else if (accumulateAngle <= rollsInAir*90) {
                        if (accumulateAngle%90 == 0) {
                            fixInRollingCube(1, (int)(accumulateAngle/90));
                        }
                        else {
                            rotateDeltaAngle();
                        }
                    }
                    else if (accumulateAngle < (rollsInAir+1)*90) {
                        rotateDeltaAngle();
                        moveRollCube(-(numStairs*(stairHeight/steps)+moveHeight));
                    }
                    else
                    {
                        fixEndInRollingCube(numStairs, (rollsInAir+1));
                        stayIn = 1;
                        rollsInAir = 0;
                        numStairs = 0;
                        // print("ENDDD");
                    }
                }
                else
                {
                    transform.position = previousPosition;
                    transform.rotation = adjustRotation;

                }
            // }
            }
        }
        else if (Input.GetKeyDown(KeyCode.Space)) {
            start = true;
        }
    }


    void OnTriggerEnter(UnityEngine.Collider collider)
    {
        Debug.Log(collider.gameObject.name);
        if (collider.gameObject.tag == "Player") {
            collideWithPlayer = true;
            collider.gameObject.GetComponent<Rigidbody>().detectCollisions = false;
            Destroy(collider.gameObject);

        }
        if (collider.gameObject.tag =="Stair")
        {
            if (stairName != collider.gameObject.name) {
                collideWithStair = true;
                stayIn = 2;
                // print("p-name");
                // print(stairName);
                // print("a-name");
                // print(collider.gameObject.name);
                stairName = collider.gameObject.name;                
            }
        }
        else if (collider.gameObject.tag =="Stair2")
        {
            if (stairName != collider.gameObject.name) {
                collideWithStair2 = true;
                stayIn = 3;
                // print("DETECT NEW STAIR");
                stairName = collider.gameObject.name;
            }
        }
        else if (collider.gameObject.tag =="SawBlock")
        {
            // print("asdfg");
            collideWithSaws = true;
            stayIn = 4;
        }

    }

    void OnTriggerStay(UnityEngine.Collider collider) 
    {

        if (collider.gameObject.tag =="nextEmptyBlock")
        {
                stayIn = 2;
        }
        else if (collider.gameObject.tag =="nextEmptyBlock2")
        {
                stayIn = 4;
        }
        else if (collider.gameObject.tag =="nextEmptyBlock3")
        {
            stayIn = 5;
        }
        else if (collider.gameObject.tag =="nextEmptyBlock4")
        {
            stayIn = 3;
        }
       if (collider.gameObject.tag == "Tile")
        {
            onSpecialBlock = true;
        }
    }

    void OnTriggerExit(UnityEngine.Collider collider)
    {
        if (collider.gameObject.tag == "Tile")
        {
            onSpecialBlock = false;
        }
        if (collider.gameObject.tag == "Finish") {
            finish = true;
//            isMoving = false;
            // start = false;
        }
    }
}