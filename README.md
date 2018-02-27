# getpassword
Retrieves a password from the Windows Credential Manager store

The goal is to automate the creation of encrypted archives for offline storage without embedding passwords in script files.

The example below creates a backup of the current user's profile directory in an encrypted zip file. 

```dosbatch
@SETLOCAL

@FOR /F "delims=^" %%I IN ('gettimestamp') DO @SET TIMESTAMP=%%I
@FOR /F "delims=^" %%I IN ('getpassword MyPasswordTargetName') DO @SET PASSWORD=%%I

@REM 7za <command> [<switches>...] <archive_name> [<file_names>...]
@SET ZIP="7za.exe"
@SET ZIPOPTS=-r -tzip -mem=AES256 -p"%PASSWORD%"

@%ZIP% a %ZIPOPTS% "%TIMESTAMP%-%USERNAME%.enc.zip" "%USERPROFILE%\*.*"

@ENDLOCAL
```

Notes
* The password must have previously been stored in Windows Credential Manager with the name 'MyPasswordTargetName'.
* The password can not contain any character from the delims list in the FOR statement.
* The 7-Zip command line executable `7za.exe` must be in the PATH.
