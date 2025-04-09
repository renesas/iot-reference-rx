## Description
- Author : `(Write the person responsible for submitting the MR)`
- Contents : `(Write what updates are included in the MR)`
- Review-by date : `(Set review deadline [include time of feedback, fix it and code is merged])`

## Checklist:
- [ ] Package comparison with baseline 
- [ ] Make sure all changes in baseline are merged in. 
- [ ] Make sure the changes of this MR does not modify common code (unless justified). 
- [ ] Make sure common code are commonized between different connectivity/application   
      (e.g: config.h, FIT config, main code, etc). 
- [ ] Make sure generic config and memory (stack/heap) config are unchanged (unless justified). 
- [ ] Exported project comparison 
- [ ] No duplicate content in .scfg 
- [ ] Make sure project structure is same. 
- [ ] Make sure project settings (.cproject/.project) does not contain unexpected changes.   
      (Check linker section, include path, linked files, macro, symbol mapping, etc) 
- [ ] Make sure SC components version and config are the same (unless justified) 
- [ ] Make sure compiler version, RTOS version are updated. 
- [ ] Make sure all settings are unchanged before/after generate code, and before/after build 
- [ ] Check HardwareDebug settings is correct, can debug with e2studio 
- [ ] "Application to be merged" can build and run to completion. 