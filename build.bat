rem c:/MinGW/bin/g++ -Wall -Bstatic -static-libgcc -static-libstdc++ -std=c++0x -Ilib -Ltemp/boost_1_52_0/bin.v2/libs/program_options/build/gcc-mingw-4.7.2/debug/link-static/threading-multi -Itemp/boost_1_52_0 -Itemp/boost_1_52_0/libs -Ltemp/boost_1_52_0/libs -Ltemp/boost_1_52_0/stage/lib -Itemp/boost_1_52_0/stage/lib -lboost_program_options-mgw47-mt-d-1_52 lib/pugixml/pugixml.cpp temp/boost_1_52_0/stage/lib/libboost_program_options-mgw47-mt-d-1_52.dll song.cpp playlist.cpp main.cpp -o savant.exe
rem temp/boost_1_52_0/bin.v2/libs/program_options/build/gcc-mingw-4.7.2/debug/link-static/threading-multi/libboost_program_options-mgw47-mt-d-1_52.a
rem -lboost_program_options-mgw47-mt-1_52.a

rem -Itemp/boost_1_52_0/stage/lib -Lboost_program_options-mgw47-mt-d-1_52
g++ -Wall -static-libgcc -static-libstdc++ -std=c++0x -Ilib lib/boost/lib/libboost_program_options-mgw47-mt-d-1_52.dll lib/pugixml/pugixml.cpp song.cpp playlist.cpp main.cpp -o savant.exe
