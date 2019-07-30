========================================================================
       DYNAMIC LINK LIBRARY : sunrpc
========================================================================
SUN ONC/RPC library. It use extra HOSTS and RPC files from 
%WINNT%\system32\driver\etc directory.
Put sunrpc.dll from Debug or Release into %WINNT%\system32 directory.
Release version contains addition authorization procedure outside 
of source code (don't recompile it).
The all warning and error messages of the client libraries can be found
in 'Administrative tools\Event Viewer' as DOOCS.log.
/////////////////////////////////////////////////////////////////////////////

Changes on 2.0.1 
1. xdr_long(XDR *xdrs,long *lp) -> xdr_long(XDR *xdrs,size_t *lp)
