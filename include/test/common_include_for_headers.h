
// common_include_for_headers.h

#ifndef __common_include_for_headers_h__
#define __common_include_for_headers_h__

//#include <common_macroses.h>


#pragma include_alias( <stdio.h>, <redesigned/stdio.h> )
#pragma include_alias( "stdio.h", "redesigned/stdio.h" )

#pragma include_alias( <stdlib.h>, <redesigned/stdlib.h> )
#pragma include_alias( "stdlib.h", "redesigned/stdlib.h" )

#pragma include_alias( <process.h>, <redesigned/process.h> )
#pragma include_alias( "process.h", "redesigned/process.h" )

#pragma include_alias( <fstream>, <redesigned/fstream> )
#pragma include_alias( "fstream", "redesigned/fstream" )


#endif // #ifndef __common_include_for_headers_h__
