max v2;#N vpatcher 156 195 858 693;#P newex 304 211 65 9 s source2_ctl;#P newex 369 486 78 9 s source1_ctl;#P newex 19 147 65 9 s source3_ctl;#P newex 524 129 104 9 Spat_Circ source3 2.5;#P newex 525 249 95 9 Room_ source3 6b-;#P newex 525 174 69 9 OPer-sel_late;#P newex 525 151 65 9 r source3_ctl;#P newex 525 210 91 9 Spat_OPer source3;#P newex 606 276 69 9 OPer-sel_late;#P newex 606 298 65 9 s source1_ctl;#P newex 243 321 118 9 Spat~ 1 6b- 6r 0 rev1;#P newex 243 298 50 1376265 input~;#P number 396 408 35 9 0 0 0 3;#P toggle 367 411 15 0;#P newex 369 460 91 196617 prepend source Az;#P newex 366 433 53 196617 around;#P toggle 3 308 15 0;#P newex 140 314 22 9 +~;#P newex 116 314 22 9 +~;#P newex 444 253 65 9 s source3_ctl;#P newex 444 231 69 9 OPer-sel_late;#P newex 211 67 304 1376265 Circ.3 source1 source2 source3;#P newex 19 340 133 1376265 dac~ 5 6 7 8 9 10;#P newex 104 246 50 1376265 input~;#P newex 19 187 50 1376265 input~;#P newex 366 29 80 9 about Spat...;#P newex 19 208 118 9 Spat~ 1D 6b+ 6r 0 rev1;#P newex 19 314 22 9 +~;#P newex 43 314 22 9 +~;#P newex 67 314 22 9 +~;#P newex 91 314 22 9 +~;#P newex 103 267 118 9 Spat~ 1 6b- 6r 0 rev1;#P newex 21 124 78 9 s source2_ctl;#P newex 21 102 78 9 s source1_ctl;#P button 21 72 15 0;#P message 46 63 113 9 corners 30 -30 -150 150 \, source Az 0;#P newex 146 162 33 9 max;#P toggle 146 139 15 0;#P newex 211 99 104 9 Spat_Circ source1 2.5;#P newex 236 126 74 9 r source1_ctl;#P newex 211 154 103 9 Spat_OPer source1;#P newex 211 189 105 9 Room_ source1 6b+;#P newex 359 209 95 9 Spat_OPer source2;#P newex 359 150 74 9 r source2_ctl;#P newex 304 176 69 9 OPer-sel_late;#P newex 359 248 99 9 Room_ source2 6b-;#P newex 358 128 104 9 Spat_Circ source2 2.5;#P comment 5 13 290 1376270 2 sources sharing the same reverb and panned with an intensity panning on 4 lsp;#P connect 44 0 40 0;#P connect 43 0 37 2;#P connect 42 0 40 0;#P connect 41 0 40 0;#P connect 40 1 39 0;#P connect 40 0 43 0;#P connect 39 0 38 0;#P connect 37 0 25 0;#P connect 37 1 25 1;#P connect 37 2 25 2;#P connect 37 3 25 3;#P connect 37 4 25 4;#P connect 37 5 25 5;#P connect 36 0 37 0;#P connect 34 0 32 0;#P connect 33 0 46 0;#P connect 35 0 32 2;#P connect 32 0 33 0;#P connect 31 0 25 0;#P connect 30 0 25 5;#P connect 29 0 25 4;#P connect 27 0 28 0;#P connect 26 0 9 0;#P connect 26 1 1 0;#P connect 24 0 16 0;#P connect 23 0 21 0;#P connect 21 0 20 0;#P connect 21 1 19 0;#P connect 21 2 18 0;#P connect 21 3 17 0;#P connect 21 4 29 0;#P connect 21 5 30 0;#P connect 20 0 25 0;#P connect 19 0 25 1;#P connect 18 0 25 2;#P connect 17 0 25 3;#P connect 16 0 20 1;#P connect 16 1 19 1;#P connect 16 2 18 1;#P connect 16 3 17 1;#P connect 16 4 29 1;#P connect 16 5 30 1;#P connect 13 0 45 0;#P connect 13 0 14 0;#P connect 13 0 15 0;#P connect 12 0 45 0;#P connect 12 0 14 0;#P connect 12 0 15 0;#P connect 11 0 10 0;#P connect 10 0 11 0;#P connect 9 0 7 0;#P connect 8 0 7 0;#P connect 7 0 6 0;#P connect 7 1 3 0;#P connect 6 0 21 2;#P connect 5 0 2 0;#P connect 5 1 27 0;#P connect 4 0 5 0;#P connect 3 0 47 0;#P connect 2 0 16 2;#P connect 1 0 5 0;#P pop;