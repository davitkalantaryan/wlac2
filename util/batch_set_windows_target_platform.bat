::
:: Created on:	Jan. 03 2018
:: Autor:	Davit Kalantaryan
::
:: Purpose:	This script sets the correct platfor for window compilation
::
:: Argumet list:This script expects 1 argumets, that shows which server to start
::

@ECHO off

SETLOCAL enableextensions
::setlocal

:: calculating current directory
SET  scriptDirectory=%~dp0
set  currentDirectory=%cd%

set  pltSetterExe=%scriptDirectory%set_windows_target_platform.exe
set  prjDir=%scriptDirectory%..\prj

:: Example :: %cur_dir%set_windows_target_platform.exe %cur_dir%..\prj\server\watchdog_server_vs15\watchdog_server.vcxproj %1
%pltSetterExe% %prjDir%\rfc2\remote_function_call_false_without_lib2_vs\remote_function_call_false_without_lib2.vcxproj %1
%pltSetterExe% %prjDir%\rfc2\rfc2_vs\rfc2.vcxproj %1
%pltSetterExe% %prjDir%\rfc2\rfc3_tests_for_next_vs\rfc3_tests_for_next.vcxproj %1
%pltSetterExe% %prjDir%\wlac2\wlac2_vs\wlac2.vcxproj %1
%pltSetterExe% %prjDir%\wlac2\win_signaling2_vs\win_signaling2.vcxproj %1

%pltSetterExe% %prjDir%\tools\grep_vs\grep.vcxproj %1
%pltSetterExe% %prjDir%\tools\ldd_vs\ldd.vcxproj %1
%pltSetterExe% %prjDir%\rpc_bind\rpc_bind_vs\rpc_bind.vcxproj %1
%pltSetterExe% %prjDir%\tools\start_windows_app_vs\start_windows_app.vcxproj %1
%pltSetterExe% %prjDir%\tools\ufind_vs\ufind.vcxproj %1
%pltSetterExe% %prjDir%\tools\ukill_vs\ukill.vcxproj %1
%pltSetterExe% %prjDir%\tools\set_environment_vs\set_environment.vcxproj %1

%pltSetterExe% %prjDir%\xdrrpc\mini_xdr_rpc_vs\mini_xdr_rpc.vcxproj %1
%pltSetterExe% %prjDir%\winldap\newwinldap_vs\newwinldap.vcxproj %1
%pltSetterExe% %prjDir%\usergroupid\usergroupid_vs\usergroupid.vcxproj %1


%pltSetterExe% %prjDir%\util\make_functions_exportable_vs\make_functions_exportable.vcxproj %1
%pltSetterExe% %prjDir%\util\make_symbols_exportable_vs\make_symbols_exportable.vcxproj %1
%pltSetterExe% %prjDir%\util\set_windows_target_platform_vs\set_windows_target_platform.vcxproj %1

%pltSetterExe% %prjDir%\test\victim_test_vs\victim_test.vcxproj %1
%pltSetterExe% %prjDir%\test\wlac_test_vs\wlac_test.vcxproj %1
%pltSetterExe% %prjDir%\test\agressor_exe_vs\agressor_exe.vcxproj %1

%pltSetterExe% %prjDir%\install\wlac_install_update_uninstall_vs\wlac_install_update_uninstall.vcxproj %1


ENDLOCAL
