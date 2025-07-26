@echo off

REM Set the folder name in a variable
set "Saved=Saved"
set "Binaries=Binaries"
set "Intermediate=Intermediate"
set "DerivedDataCache=DerivedDataCache"


REM Saved
if exist "%Saved%" (
    REM Remove the folder and its contents
    rmdir /s /q "%Saved%"

    REM Print a message indicating the folder has been deleted
    echo Folder "%Saved%" deleted.
) else (
    REM Print a message if the folder doesn't exist
    echo Folder "%Saved%" does not exist.
)

REM Binaries
if exist "%Binaries%" (
    REM Remove the folder and its contents
    rmdir /s /q "%Binaries%"

    REM Print a message indicating the folder has been deleted
    echo Folder "%Binaries%" deleted.
) else (
    REM Print a message if the folder doesn't exist
    echo Folder "%Binaries%" does not exist.
)

REM Intermediate
if exist "%Intermediate%" (
    REM Remove the folder and its contents
    rmdir /s /q "%Intermediate%"

    REM Print a message indicating the folder has been deleted
    echo Folder "%Intermediate%" deleted.
) else (
    REM Print a message if the folder doesn't exist
    echo Folder "%Intermediate%" does not exist.
)


REM DerivedDataCache
if exist "%DerivedDataCache%" (
    REM Remove the folder and its contents
    rmdir /s /q "%DerivedDataCache%"

    REM Print a message indicating the folder has been deleted
    echo Folder "%DerivedDataCache%" deleted.
) else (
    REM Print a message if the folder doesn't exist
    echo Folder "%DerivedDataCache%" does not exist.
)