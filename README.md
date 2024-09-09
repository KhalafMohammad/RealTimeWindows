# Real-Time Windows Driver

The Driver Follows the instructions of Soft-real time windows 10 IoT [URL](https://learn.microsoft.com/en-us/windows/iot/iot-enterprise/soft-real-time/soft-real-time).
Follow the Device Configuration, the driver requires:

Edition	Windows 10 IoT Enterprise LTSC
Version	21H2
OS build	19044.4780

Make sure to update windows to update 21h2 or later is avalible, because MDM_WindowsIoT_SoftRealTimeProperties01 isn't avalible before 21h2 as it was intereduced in 21h2.
DONOT run the Driver without doing the Device configuration and reserving realtime cores as it's very harmful for your PC.