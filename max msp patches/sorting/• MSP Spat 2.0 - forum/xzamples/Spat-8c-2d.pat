max v2;#N vpatcher 179 282 603 519;#N vpatcher 50 40 389 187;#P newex 31 60 178 9 Spat~ 1Da 8c 2d;#P inlet 31 27 16 0;#P outlet 31 105 15 0;#P outlet 52 105 15 0;#P outlet 232 77 15 0;#P inlet 232 45 15 0;#P connect 5 0 3 0;#P connect 5 1 2 0;#P connect 4 0 5 0;#P connect 0 0 5 2;#P connect 0 0 1 0;#P pop;#P newobj 23 150 78 1376265 patcher Spat~;#P newex 220 71 57 1376265 Spat_Circ source1 2.5;#P newex 220 48 57 1376265 Circ source1;#P newex 347 21 50 9 about Spat...;#P newex 23 180 44 9 dac~ 5 6;#P newex 321 84 76 9 r Spat_control;#P message 156 63 48 9 bang \, DISPLAY 0;#P comment 102 61 50 196620 Reset >;#P comment 18 21 301 196620 1 source -> 2 speakers (stereo AB "ORTF");#P newex 220 109 177 9 Spat_OPer source1;#P newex 220 131 103 9 Spat_ source1 1 8c;#P toggle 365 154 15 0;#P toggle 197 155 15 0;#P newex 197 175 33 9 gate;#P newex 22 127 39 9 input~;#P toggle 24 68 16 0;#P comment 41 69 26 196618 dsp;#P newex 24 90 30 9 max;#P comment 233 155 57 196617 <-- print low-level control messages;#P comment 306 155 58 196617 print --> high-level commands;#P newex 365 194 32 9 print;#P newex 365 173 32 9 gate;#P newex 197 196 33 9 print;#P connect 22 0 18 0;#P connect 22 1 18 1;#P connect 21 0 13 0;#P connect 20 0 21 0;#P connect 17 0 13 0;#P connect 13 0 12 0;#P connect 13 1 1 1;#P connect 7 0 5 0;#P connect 12 0 22 1;#P connect 12 0 9 1;#P connect 8 0 22 0;#P connect 9 0 0 0;#P connect 11 0 1 0;#P connect 10 0 9 0;#P connect 5 0 7 0;#P connect 1 0 2 0;#P connect 16 0 13 0;#P pop;