#| file: r3balum-C2.llFri May 16 10:38:31 MET 1986modele original: ../FMOD/balum-C2.llreduction aux temps :(0. 3.)offset: 10. dB.pente: 20. dB. par octave54 partiels retenus sur 167|##| no  freq		ampl		band1  27.00586	-45.88841	2.7079512  129.8902	-38.16751	.083869833  259.8336	-11.30754	.16010934  390.3459	-33.88422	.015670475  520.5243	-14.83291	.095123046  781.8894	-33.24989	.2968867  914.1698	-31.0981	.31702858  1180.568	-22.85245	.19674359  1314.12	-23.51904	.489196410  1315.895	-20.078	1.41808411  1699.634	-36.40427	.648372812  1710.815	-37.13186	.734291813  1720.88	-36.96644	.579850714  1856.884	-34.12703	.350197915  1995.093	-38.93378	.377594916  2545.087	-31.10304	.615050617  2699.821	-42.56104	.637764118  3133.715	-38.1924	.859506519  3171.752	-41.60268	.628428720  3430.114	-49.53591	1.05963821  3868.694	-53.12363	.674877422  3881.652	-52.9825	.759427323  3888.408	-47.10462	1.27285524  4273.479	-67.17349	.0331932525  4329.08	-51.98584	1.65353626  4336.366	-46.08416	2.23684827  4352.477	-46.67236	1.75535328  4360.979	-47.11545	1.02995829  4512.177	-53.23235	1.05387430  5151.521	-54.36668	5.81411831  5487.483	-59.5667	1.14046432  5669.413	-59.86681	.742212433  6534.642	-67.52332	.862990934  6757.995	-66.1815	9.30771835  7053.019	-62.56796	7.97839136  7244.848	-64.8703	7.59360337  7807.3	-67.92776	3.89290438  7950.906	-64.23344	10.1269639  8399.127	-73.3161	3.06718840  8590.039	-67.08197	5.3463641  9385.926	-70.62158	5.72431442  9616.543	-69.48346	8.26962543  9651.807	-65.13205	11.2648244  9865.57	-71.27386	4.23534945  10273.03	-76.32585	2.85959646  10335.02	-71.81319	6.60716747  11117.58	-68.31352	10.8104948  11401.37	-76.7245	2.61047949  12112.23	-67.54622	5.69011350  12180.71	-73.72675	4.41925351  12351.25	-71.85429	6.99015352  12446.59	-66.71902	10.187153  12897.95	-72.11697	3.10968654  12962.31	-67.25133	12.86815|#(setq l-freqf '(   27.00586   129.8902   259.8336   390.3459   520.5243   781.8894   914.1698   1180.568   1314.12   1315.895   1699.634   1710.815   1720.88   1856.884   1995.093   2545.087   2699.821   3133.715   3171.752   3430.114   3868.694   3881.652   3888.408   4273.479   4329.08   4336.366   4352.477   4360.979   4512.177   5151.521   5487.483   5669.413   6534.642   6757.995   7053.019   7244.848   7807.3   7950.906   8399.127   8590.039   9385.926   9616.543   9651.807   9865.57   10273.03   10335.02   11117.58   11401.37   12112.23   12180.71   12351.25   12446.59   12897.95   12962.31))(setq l-amplf '(   .005076675   .01234878   .2720338   .02022034   .181282   .0217523   .0278673   .07200745   .06668808   .099106   .01512817   .01391255   .01418006   .01966294   .01130605   .02785145   .007446429   .01231346   .008315073   .003335834   .00220708   .002243236   .004413353   .0004378501   .002515984   .004963543   .00463855   .004407856   .002179628   .001912783   .001051151   .001015451   .0004205655   .0004908231   .0007440493   .0005708012   .0004014315   .0006142246   .0002158709   .0004424874   .0002943887   .0003356037   .0005538565   .0002730903   .0001526535   .000256649   .0003839936   .0001458058   .0004194584   .0002059026   .0002554375   .0004613698   .0002478284   .000433944))(setq l-bandf '(   2.707951   .08386983   .1601093   .01567047   .09512304   .296886   .3170285   .1967435   .4891964   1.418084   .6483728   .7342918   .5798507   .3501979   .3775949   .6150506   .6377641   .8595065   .6284287   1.059638   .6748774   .7594273   1.272855   .03319325   1.653536   2.236848   1.755353   1.029958   1.053874   5.814118   1.140464   .7422124   .8629909   9.307718   7.978391   7.593603   3.892904   10.12696   3.067188   5.34636   5.724314   8.269625   11.26482   4.235349   2.859596   6.607167   10.81049   2.610479   5.690113   4.419253   6.990153   10.1871   3.109686   12.86815))