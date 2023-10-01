using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Linq;

public class ObjectPool : Singleton<ObjectPool>
{
    public List<GameObject> PrefabsForPool;

    private List<GameObject> _pooledObjects = new List<GameObject>();

    public GameObject GetObjectFromPool(string objectName) {
        var instance = _pooledObjects.FirstOrDefault(obj => obj.name == objectName);

        if (instance != null) {
            _pooledObjects.Remove(instance);
            instance.SetActive(true);
            return instance;
        }

        var prefab = PrefabsForPool.FirstOrDefault(obj => obj.name == objectName);
        if (prefab != null) {

            var newInstance = Instantiate(prefab, transform);
            newInstance.name = objectName;
            return newInstance;
        }
        Debug.LogWarning("Obj pool");
        return null;
    }

    public void PoolObject(GameObject obj) {
        obj.SetActive(false);
        _pooledObjects.Add(obj);
    }

}
