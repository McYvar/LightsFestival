using System;
using System.Collections;
using System.IO.Ports;
using System.Threading;
using UnityEngine;
using UnityEngine.Events;

public class SerialHandler : MonoBehaviour
{
    public static float rotX, rotY, rotZ;
    public static float pitch, roll, yaw;
    public static float accX, accY, accZ;

    float currentAccY;
    float currentRotY;

    SerialPort OutputStream = null;
    SerialPort InputStream = null;

    [SerializeField] UnityEvent myEvent;

    void Start()
    {
        DontDestroyOnLoad(this);
        StartThread();
    }

    public void Update()
    {
        if (Input.GetKeyDown(KeyCode.Space)) ReInit();
        if (Input.GetKeyDown(KeyCode.Return)) myEvent.Invoke();

        string[] command = SplitOutput();
        if (command == null) return;

        // command
        if (command.Length == 1) WriteToOutput(command[0]);

        // axis
        if (command.Length == 9)
        {

            rotX = float.Parse(command[0]);
            rotY = float.Parse(command[1]);
            rotZ = float.Parse(command[2]);

            pitch = float.Parse(command[3]);
            roll = float.Parse(command[4]);
            yaw = float.Parse(command[5]);

            accX = float.Parse(command[6]);
            accY = float.Parse(command[7]);
            accZ = float.Parse(command[8]);
        }


        if (Mathf.Abs(rotY - currentRotY) > 135)
        {
            WriteToOutput("Spread");
            currentRotY = rotY;
        }
        else if (accY - currentAccY > 2)
        {
            WriteToOutput("Fill");
            currentRotY = rotY;
        }
        else
        {
            WriteToOutput(rotZ.ToString());
        }
        Debug.Log("Y:" + (accY - currentAccY));
        Debug.Log("Z:" + (rotY - currentRotY));

        currentAccY = accY;
    }

    string[] SplitOutput()
    {
        string[] output = ReadFromInput()?.Split(char.Parse(","));

        if (output == null) return null;
        return output;
    }

    public void ReInit()
    {
        SendToOutput("reInit");
    }

    Thread thread;

    Queue outputQueue;
    Queue inputQueue;

    public void StartThread()
    {
        outputQueue = Queue.Synchronized(new Queue());
        inputQueue = Queue.Synchronized(new Queue());

        thread = new Thread(ThreadLoop);
        thread.Start();
    }

    public void ThreadLoop()
    {
        OutputStream = new SerialPort("COM5", 115200);
        OutputStream.ReadTimeout = 3000;
        OutputStream.Open();

        InputStream = new SerialPort("COM15", 115200);
        InputStream.ReadTimeout = 3000;
        InputStream.Open();

        // Looping
        while (IsLooping())
        {
            // Send to
            if (outputQueue.Count != 0)
            {
                string command = (string)outputQueue.Dequeue();
                WriteToOutput(command);
            }

            // Read from
            string result = ReadFromInput(3000);
            if (result != null)
            {
                inputQueue.Enqueue(result);
            }
        }

        OutputStream.Close();
        InputStream.Close();
    }

    public void SendToOutput(string command)
    {
        outputQueue.Enqueue(command);
    }

    public void WriteToOutput(string message)
    {
        OutputStream.WriteLine(message);
        OutputStream.BaseStream.Flush();
    }

    public string ReadFromInput()
    {
        if (inputQueue.Count == 0) return null;
        return (string)inputQueue.Dequeue();
    }

    public string ReadFromInput(int timeout = 0)
    {
        InputStream.ReadTimeout = timeout;
        try
        {
            string input = InputStream.ReadLine();
            InputStream.BaseStream.Flush();
            return input;
        }
        catch (TimeoutException e)
        {
            Debug.LogError(e);
            return null;
        }
    }

    private bool looping = true;
    public void StopThread()
    {
        lock (this)
        {
            looping = false;
        }

    }

    public bool IsLooping()
    {
        lock (this)
        {
            return looping;
        }
    }

    private void OnApplicationQuit()
    {
        CloseStreams();
    }

    public void CloseStreams()
    {
        OutputStream.Close();
        InputStream.Close();
    }

    public void SingleToFLoat(float single)
    {
        WriteToOutput(single.ToString());
    }
}