@echo off
set message =  Hello Build

set e2_studio_exe_path=C:\Renesas\setup_e2_studio_2022-04\eclipse\e2studioc.exe

set source_path=%1
set board_folder_name=%2

IF %3 == ccrx (
  set projects_environment_path=%board_folder_name%\e2studio_ccrx
  set path_project_OTA=%board_folder_name% 
) ELSE IF %3 == gcc (
  set projects_environment_path=%board_folder_name%\e2studio-gcc
   set path_project_OTA=%board_folder_name%"-gcc"
)

set workspace_path=%source_path%\ws\%board_folder_name%-%3
set project_path=%source_path%\Projects\%projects_environment_path%
set project_mot_file_path=%project_path%\HardwareDebug\%board_folder_name%".mot"

rem ********************************************************************************************************************

echo %message%

rmdir /s /q "%source_path%\ws"
rmdir /s /q "%project_path%\HardwareDebug"

"%e2_studio_exe_path%" -nosplash --launcher.suppressErrors -debug -consolelog -application org.eclipse.cdt.managedbuilder.core.headlessbuild -data "%workspace_path%" -import %project_path% -cleanBuild all


GOTO END

:END