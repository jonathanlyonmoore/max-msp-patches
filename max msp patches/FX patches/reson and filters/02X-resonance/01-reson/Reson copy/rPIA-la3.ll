#| file: rPIA30.llThu Nov 20 15:16:22 MET 1986modele original: ../FMOD/PIA-la3.llreduction aux temps :(0.)offset: 20. dB.pente: 34. dB. par octave82 partiels retenus sur 257|##| no  freq		ampl		band1  13.06686	-70.18794	.44570992  31.74706	-71.17479	.95367733  37.63908	-75.22194	.14808184  43.2532	-49.40657	1.9918655  43.26115	-64.08087	1.1701936  49.65102	-73.3614	.29129257  55.96056	-58.78252	1.3704148  62.58501	-50.62232	1.563169  63.91549	-58.60878	.720746410  68.30585	-65.83476	.480731711  76.41039	-53.67017	.904426112  78.0506	-39.80435	1.9858413  104.0218	-33.62988	3.28676114  119.0321	-47.16741	1.53670415  121.5043	-36.30682	3.67863716  219.5119	-28.90664	.404615317  220.1106	-16.10243	.918035418  439.3563	-22.956	.199241319  439.3818	-30.66004	.101236120  439.8582	-15.57204	.561405221  660.4727	-25.09761	1.03317222  741.0542	-43.44012	12.2419923  880.15	-40.69011	.18933724  880.2719	-33.79158	.280184925  881.4718	-27.22475	1.40132426  1101.211	-35.63773	.230526727  1102.375	-22.34324	.869289428  1103.106	-20.68761	1.2361329  1323.45	-38.21761	.398517830  1324.684	-35.40915	1.61843131  1546.667	-28.43104	2.39760132  1768.35	-45.96989	2.5918233  1998.083	-32.68935	.985338834  1999.946	-26.54212	2.46050435  2221.501	-35.90509	1.8095836  2224.906	-36.27336	.707545437  2225.372	-40.34291	.594535638  2226.544	-36.31406	2.94154639  2454.253	-38.44085	.766723340  2456.12	-28.5976	2.33716241  2685.767	-31.48172	2.46352942  2686.953	-46.54326	.49574843  2915.	-26.6639	.805119944  2917.823	-33.49809	.570982145  2918.031	-26.12087	2.6132746  2920.396	-45.09582	.675703547  3154.328	-36.99403	2.24390248  3156.929	-43.42786	.644561849  3392.203	-34.27784	.834220250  3392.615	-23.15065	4.25246251  3875.618	-49.36308	.853504852  4117.033	-36.98114	5.58760553  4119.963	-42.29333	1.72238254  4226.158	-48.66747	7.13629255  4370.155	-41.19727	3.67985156  4371.738	-50.58607	1.05478657  4621.741	-51.52073	.938901358  4621.743	-36.2598	2.8384259  4623.818	-51.33749	.811469260  4875.951	-48.48405	3.19389261  5134.923	-37.21712	3.16328962  5136.418	-43.75024	1.5956863  5356.665	-59.2416	5.70574764  5395.965	-40.84211	3.30999865  5930.533	-55.62175	1.5992266  5930.604	-50.68351	2.44852667  6199.12	-51.93703	4.34198268  6202.494	-65.63202	.36886269  6386.654	-55.65887	5.60345570  6483.247	-55.76931	.289103471  6759.698	-53.66122	3.79221272  6760.776	-51.91684	7.05868473  7047.21	-47.10532	2.39667874  7335.63	-41.89021	3.75387775  7344.176	-61.10366	1.51678576  8222.975	-64.7049	2.77186377  9166.094	-54.6079	6.46734178  9167.701	-62.6811	1.67505479  9505.643	-61.27614	3.34104480  9821.469	-65.7011	2.30080181  10821.9	-68.9613	6.21157282  12611.48	-72.24661	9.468479|#(setq l-freqf '(   13.06686   31.74706   37.63908   43.2532   43.26115   49.65102   55.96056   62.58501   63.91549   68.30585   76.41039   78.0506   104.0218   119.0321   121.5043   219.5119   220.1106   439.3563   439.3818   439.8582   660.4727   741.0542   880.15   880.2719   881.4718   1101.211   1102.375   1103.106   1323.45   1324.684   1546.667   1768.35   1998.083   1999.946   2221.501   2224.906   2225.372   2226.544   2454.253   2456.12   2685.767   2686.953   2915.   2917.823   2918.031   2920.396   3154.328   3156.929   3392.203   3392.615   3875.618   4117.033   4119.963   4226.158   4370.155   4371.738   4621.741   4621.743   4623.818   4875.951   5134.923   5136.418   5356.665   5395.965   5930.533   5930.604   6199.12   6202.494   6386.654   6483.247   6759.698   6760.776   7047.21   7335.63   7344.176   8222.975   9166.094   9167.701   9505.643   9821.469   10821.9   12611.48))(setq l-amplf '(  .0003094588  .0002762233  .0001733414  .003385876  .0006251098  .0002147482  .001150467  .002943634  .00117371  .0005108122  .002072486  .0102278  .0208212  .004381565  .01529884  .03586475  .1566313  .07115412  .02930881  .1664937  .05560574  .0067767   .009236224  .02043718  .04352735  .01652395  .0763551  .09238887  .01227776  .01696456  .03788333  .005029273  .02320239  .04708625  .01602306  .0153579  .009612892  .0152861  .01196624  .03716379  .0266633  .004708002  .04643065  .02113954  .04942612  .005561716  .01413509  .00673918  .01932448  .06957725  .003402871  .01415607  .007679502  .003686603  .008712376    .002955945  .002654382  .01538188  .002710972  .003765283  .01377666  .006493633  .001091239  .009075997  .001655436  .002922971  .002530164  .0005228765  .001648376  .00162755  .002074622  .002536051  .004412998  .008044316  .0008806775  .0005817751  .001860393  .000734421  .0008633619  .0005187341  .0003563975  .0002441569    ))(setq l-bandf '(   .4457099   .9536773   .1480818   1.991865   1.170193   .2912925   1.370414   1.56316   .7207464   .4807317   .9044261   1.98584   3.286761   1.536704   3.678637   .4046153   .9180354   .1992413   .1012361   .5614052   1.033172   12.24199   .189337   .2801849   1.401324   .2305267   .8692894   1.23613   .3985178   1.618431   2.397601   2.59182   .9853388   2.460504   1.80958   .7075454   .5945356   2.941546   .7667233   2.337162   2.463529   .495748   .8051199   .5709821   2.61327   .6757035   2.243902   .6445618   .8342202   4.252462   .8535048   5.587605   1.722382   7.136292   3.679851   1.054786   .9389013   2.83842   .8114692   3.193892   3.163289   1.59568   5.705747   3.309998   1.59922   2.448526   4.341982   .368862   5.603455   .2891034   3.792212   7.058684   2.396678   3.753877   1.516785   2.771863   6.467341   1.675054   3.341044   2.300801   6.211572   9.468479))