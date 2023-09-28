rd /q /s .\Projects\.metadata > nul

if not exist .\log (
  mkdir log
)

if not exist .\mot (
  mkdir mot
)

for /f %%s in (%PROJECT_LIST%) do (
  rd /q /s .\Projects\%%s\e2studio_ccrx\HardwareDebug > nul
  echo Building "%%s"...
  e2studioc.exe --launcher.suppressErrors -nl en_US -nosplash -debug -consolelog -application org.eclipse.cdt.managedbuilder.core.headlessbuild -data ".\Projects" -import ".\Projects\%%s\e2studio_ccrx" -cleanBuild %%s > %CIC_DIR_LOG%\log_%%s.txt 2>&1
  echo Build finished.
  copy /y .\Projects\%%s\e2studio_ccrx\HardwareDebug\*.mot %CIC_DIR_MOT%\ > nul
)
