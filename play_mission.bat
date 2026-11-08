@echo off
rem Launch / switch the original Nec.exe to a campaign mission.
rem   play_mission.bat 1        mission 1 (REVOLT)
rem   play_mission.bat 16       final mission (NECTOR)
rem   play_mission.bat 3 --restart
rem   play_mission.bat --list
python "%~dp0tools\orig_play_mission.py" %*
if errorlevel 1 pause
