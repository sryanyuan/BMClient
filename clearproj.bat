@pushd %~dp0
@set WDIR=%~dp0
@rd /s /q %WDIR%\debug
@rd /s /q %WDIR%\release
@del /q %WDIR%\backmir08.ncb
@pause
@popd