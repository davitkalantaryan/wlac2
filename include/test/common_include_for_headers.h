
// common_include_for_headers.h

#ifndef __common_include_for_headers_h__
#define __common_include_for_headers_h__

//#include <common_macroses.h>


#pragma include_alias( <stdio.h>, <other/redesigned/stdio.h> )
#pragma include_alias( "stdio.h", "other/redesigned/stdio.h" )

#pragma include_alias( <stdlib.h>, <other/redesigned/stdlib.h> )
#pragma include_alias( "stdlib.h", "other/redesigned/stdlib.h" )

#pragma include_alias( <process.h>, <other/redesigned/process.h> )
#pragma include_alias( "process.h", "other/redesigned/process.h" )

#pragma include_alias( <fstream>, <other/redesigned/fstream> )
#pragma include_alias( "fstream", "other/redesigned/fstream" )


#endif // #ifndef __common_include_for_headers_h__
