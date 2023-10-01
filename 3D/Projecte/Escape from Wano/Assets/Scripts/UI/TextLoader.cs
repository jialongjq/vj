using UnityEngine;
using UnityEngine.UI;
using TMPro;

public class TextLoader : MonoBehaviour
{
    public TMP_Text textField;
    public string playerPrefsKey;

    void Start()
    {
        // Retrieve the value from PlayerPrefs
        string savedText = PlayerPrefs.GetInt(playerPrefsKey)+"";   
        Debug.Log(PlayerPrefs.HasKey(playerPrefsKey));
        // Assign the value to the Text component
        textField.text = savedText;
    }
}
