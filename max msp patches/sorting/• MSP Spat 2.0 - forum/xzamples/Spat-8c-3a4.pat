max v2;#N vpatcher 56 72 513 365;#P newex 328 93 95 1376265 Spat_Circ source1 2.5;#P newex 328 73 62 1376265 Circ source1;#P newex 24 169 139 1376265 Spat~ 1Da 8c 3a 3a4;#P newex 346 17 50 9 about Spat...;#P comment 97 49 50 196620 Reset >;#P message 151 51 69 9 bang \, DISPLAY 0;#P newex 220 114 177 9 Spat_OPer source1;#P newex 137 121 76 9 r Spat_control;#P comment 18 17 317 196620 1 source -> 3-channel B format -> 4 speakers;#P newex 220 150 103 9 Spat_ source1 1 8c;#P comment 305 207 56 196617 print -----> high-level commands;#P toggle 363 206 15 0;#P newex 363 226 34 9 gate;#P newex 363 247 34 9 print;#P toggle 198 206 15 0;#P newex 198 226 32 9 gate;#P newex 198 247 32 9 print;#P newex 24 140 73 9 input~;#P newex 24 211 58 9 dac~ 5 6 7 8;#P toggle 24 73 16 0;#P comment 41 74 26 196618 dsp;#P comment 233 207 59 196617 <-- print low-level control messages;#P message 82 90 127 9 corners 35 -35 -145 145;#P newex 24 95 30 9 max;#P connect 23 0 17 0;#P connect 22 0 23 0;#P connect 21 0 5 0;#P connect 21 1 5 1;#P connect 21 2 5 2;#P connect 21 3 5 3;#P connect 18 0 17 0;#P connect 17 0 14 0;#P connect 17 1 11 1;#P connect 16 0 17 0;#P connect 14 0 21 2;#P connect 14 0 8 1;#P connect 12 0 11 0;#P connect 11 0 10 0;#P connect 9 0 8 0;#P connect 8 0 7 0;#P connect 6 0 21 0;#P connect 4 0 0 0;#P connect 1 0 17 0;#P connect 0 0 4 0;#P pop;