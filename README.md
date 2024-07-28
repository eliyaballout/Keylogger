# ***Welcome to Keylogger***



## Introduction

Welcome to Keylogger project, this program is written in C/C++. <br>
Keylogger is a Windows-based keylogging application developed as part of an educational purposes. It captures keystrokes globally across all user sessions and logs them to a file. The project consists of a dynamic link library (DLL) that installs a global keyboard hook and a main application that handles the DLL injection, logging, and managing the captured keystrokess. <br><br>


**Warning**

The techniques demonstrated by this project are powerful and can be misused if applied maliciously. This tool is provided with the intention of advancing knowledge and should only be used in ethical hacking scenarios where explicit permission has been obtained. Misuse of this software can result in significant harm and legal consequences. By using this software, you agree to do so responsibly, ethically, and within legal boundaries.<br><br>


**Important Note: Before you proceed, it's crucial to understand and acknowledge the following:**

1. ***Purpose of the Script:*** This script is developed strictly for educational purposes. It aims to demonstrate the vulnerabilities present in wireless networks and how they can be exploited. By understanding these vulnerabilities, users can take proactive measures to enhance the security of their networks.
   
2. ***Legal Implications:*** Unauthorized access or disruption of computer networks, including wireless networks, is illegal in many jurisdictions. ***This script should only be used on networks that you own or have explicit permission to test***. Do not use this script against networks that you do not own or do not have authorization to test. Any misuse of this script is entirely the responsibility of the user.

3. ***Ethical Considerations:*** It is essential to use this script responsibly and ethically. Always respect the privacy and rights of others. Avoid causing harm or disruption to network users. The primary goal is to learn and understand network security, not to cause harm or engage in malicious activities.

4. ***Disclaimer:*** I do not endorse or encourage any illegal or malicious activities. The script is provided as-is, without any warranties or guarantees. Users are solely responsible for their actions and should use this script responsibly. <br><br>


**By accessing and using this script, you acknowledge and agree to the terms and guidelines mentioned above. Always prioritize ethical considerations, safety, and legal compliance.**
<br><br>




## Key Components

1. **Global Keyboard Hook:** Uses `SetWindowsHookEx` to install a global keyboard hook to capture keystrokes.

2. **DLL Injection:** Injects the DLL containing the hook into all running processes.

3. **Logging:** Logs keystrokes with timestamps and saves them to a buffer. When a USB drive is inserted, the keystrokes are written to a file and saved in an encrypted format on the USB drive.

4. **Command-Line Interface:** Provides commands to start, stop, and manage the keylogger.
<br>
**The keystrokes are saved into a buffer, and only when a USB drive is inserted, the keystrokes are written to a file and saved in an encrypted format using XOR encryption on the USB drive.**
<br><br>




## Features

1. **MiniKeyloggerr:** it has a cpp file called [MiniKeylogger.cpp](https://github.com/eliyaballout/Keylogger/blob/main/MiniKeylogger/MiniKeylogger/MiniKeylogger.cpp), main application for starting and stopping the keylogger.

2. **KeyloggerDll:** it has a cpp file called [dllmain.cpp](https://github.com/eliyaballout/Keylogger/blob/main/KeyloggerDll/KeyloggerDll/dllmain.cpp), this is a DLL file, containing the hook procedure for capturing keystrokes, checking for USB and the encryption method.

3. **Decrypt:** it has a cpp file called [Decrypt.cpp](https://github.com/eliyaballout/Keylogger/blob/main/Decrypt/Decrypt/Decrypt.cpp), this code is a program for decrypting the logged keystrokes.

<br><br>




## Requirements, Installation & Usage

**I will explain here the requirements, installation and the usage of this keylogger:** <br>

**Requirements:**
1. Ensure you have a C++ compiler and Windows SDK installed. <br>

2. You need to modify the dll path to `YOUR_KeyloggerDLL.dll`. in the cpp file [MiniKeylogger.cpp](https://github.com/eliyaballout/Keylogger/blob/main/MiniKeylogger/MiniKeylogger/MiniKeylogger.cpp), in line **52** `hDll = LoadLibraryA("PATH_TO_KeyloggerDLL.dll");`, you need to change the `PATH_TO_KeyloggerDLL.dll` to the path where you actually stores the `KeyloggerDLL.dll` dll file, for example: `"C:\\Users\\user\\source\\repos\\KeyloggerDll\\x64\\Debug\\PATH_TO_KeyloggerDLL.dll"`. Make sure to put the **absolute** path for the DLL. <br>

3. Compile the `.cpp` file to generate the executable. <br><br>


**Installation:**
1. Download and extract the [ZIP file](https://github.com/eliyaballout/Keylogger/archive/refs/heads/main.zip).<br>

2. Navigate to **MiniKeylogger --> x64 --> Debug**, you will find the `MiniKeylogger.exe` executable file, this is the executable that you need to run in order to start/stop the keylogger.<br>

3. Navigate to **Decrypt --> x64 --> Debug**, you will find the `Decrypt.exe` executable file, this is the executable that you need to run in order to decrypt the output file that contains the keystrokes after you have been inserted the USB. <br><br>


**Usage:**

**Make sure you run all the executables in cmd with administrator privileges (Run as Administrator)** <br>

**Running the MiniKelogger:**

```
MiniKelogger.exe start
```
<br>

**Stopping the MiniKelogger:** <br>
**Make sure to run this command in a separated cmd window with administrator privileges.**
```
MiniKelogger.exe stop
```
<br>

**Running the Decryptor:**
```
Decrypt.exe <encrypted_log_file> <decrypted_output_file>
```
where `<encrypted_log_file>` should be the full path of the encrypted file. You will find it in the USB drive that you have been inserted.<br>
And `<decrypted_output_file>` should be the full path where you want to save the decrypted file that contains the keystrokes.

<br><br>




## Ethical Considerations

This tool is intended for educational use only to demonstrate techniques commonly used by rootkits. It should be used in a controlled environment, such as a penetration testing lab, where explicit permission has been granted. Always practice responsible disclosure and use ethical hacking principles.<br><br>




## Technologies Used
<img src="https://github.com/devicons/devicon/blob/master/icons/c/c-original.svg" title="c" alt="c" width="40" height="40"/>&nbsp;
<img src="https://github.com/devicons/devicon/blob/master/icons/cplusplus/cplusplus-original.svg" title="c++" alt="c++" width="40" height="40"/>&nbsp;
<br><br><br>




## Demonstration of the keylogger



<br>
