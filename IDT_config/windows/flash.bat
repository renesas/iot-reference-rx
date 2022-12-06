@echo off

rem *** User Settings **************************************************************************************************
set renesas_flash_programmer_exe_path=C:\Program Files (x86)\Renesas Electronics\Programming Tools\Renesas Flash Programmer V3.09\RFPV3.exe
rem ********************************************************************************************************************

rem *** The following variables determined by specifications by Amazon and Renesas (Not to need changing) **************
set source_path=%1
set board_folder_name=%2
IF %3 == ccrx (
  set projects_environment_path=%board_folder_name%\e2studio_ccrx
  set vendors_boards_environment_folder_name=%board_folder_name%
) ELSE IF %3 == gcc (
  set projects_environment_path=%board_folder_name%\e2studio-gcc
  set vendors_boards_environment_folder_name=%board_folder_name%-gcc
)

set project_path=%source_path%\Projects\%projects_environment_path%
set mot_file_path=%project_path%\HardwareDebug\%project_path%.mot

set erase_project_path=%source_path%\Projects\%board_folder_name%\flash_project\erase_from_bank1\erase.rpj
set flash_project_path=%source_path%\Projects\%board_folder_name%\flash_project\flash_project.rpj
set log_file_path=%project_path%\HardwareDebug\flash_programmer.log
rem ********************************************************************************************************************



rem Run Renesas Flash Programmer to erase data of bank1
"%renesas_flash_programmer_exe_path%" /silent "%erase_project_path%" /log "%log_file_path%"
rem Run Renesas Flash Programmer to flash data
"%renesas_flash_programmer_exe_path%" /silent "%flash_project_path%" /file "%mot_file_path%" /log "%log_file_path%"
GOTO END

:END