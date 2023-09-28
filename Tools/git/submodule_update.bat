@echo off

call .\Tools\boards\set_project_list.bat
cd %~dp0..\.. & git pull --progress -v --no-rebase "origin"
cd %~dp0..\.. & git submodule sync --recursive
cd %~dp0..\.. & git submodule update --init --recursive
cd %~dp0..\..
