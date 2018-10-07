#------------------------------------------
#生成 exe 就 "EXE"，生成 lib 就 "LIB"
set(MODE "LIB")
set( TARGET_PREFIX "" )
set( FOLDER_NAME "LIBs" )
add_subdirectory (LIBs)

#------------------------------------------
#生成 exe 就 "EXE"，生成 lib 就 "LIB"
set(MODE "EXE")

set( TARGET_PREFIX "01_" )
set( FOLDER_NAME "01_Introduction" )
add_subdirectory (01_Introduction)