@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat"

del "..\resources\native\jbugcheck.*"
cl jbugcheck.c /Fe"..\resources\native\jbugcheck.dll" /MT /LD /I "%JAVA_HOME%\include" /I "%JAVA_HOME%\include\win32" /link /noexp /noimplib

pause