#| file: r3balum-C3.llFri May 16 10:49:57 MET 1986modele original: ../FMOD/balum-C3.llreduction aux temps :(0. 3.)offset: 10. dB.pente: 20. dB. par octave42 partiels retenus sur 235|##| no  freq		ampl		band1  14.93143	-69.65804	.026179942  40.05544	-72.66772	.20088063  47.87826	-77.77977	.1538334  63.32995	-58.38895	1.5481275  167.1616	-38.45867	7.3624756  177.5058	-30.30522	10.83537  260.0743	-16.81293	.22081948  518.5092	-24.96359	.11814969  780.7788	-15.37406	2.17827310  1045.69	-25.13823	.283812611  1311.126	-33.41514	.20107212  1588.701	-37.312	.508590713  1850.47	-24.7345	.717441614  1857.926	-26.75679	.37228715  2127.85	-31.52211	1.01262216  2412.13	-23.02267	3.34756717  3307.771	-41.73302	1.19469918  3314.749	-40.39085	1.48728119  3584.746	-34.39268	1.14061820  3589.47	-35.48717	.993963121  3605.28	-30.48743	2.80328122  3910.235	-36.78728	1.78528323  4186.188	-44.25226	1.40332624  4270.069	-43.03277	4.73212825  4353.698	-41.59832	12.0934426  4561.798	-46.50241	1.20075127  4600.229	-45.43206	6.33657628  4831.049	-70.54329	.049420829  4901.288	-40.54713	2.3938930  4916.701	-42.71004	1.47521131  4924.305	-36.80637	1.49872332  4934.346	-44.80784	.822423633  5268.153	-33.56201	12.348334  5297.832	-35.72038	4.65666735  5597.927	-39.43294	6.52927836  5650.533	-39.53107	5.87607237  8467.482	-53.28844	3.08451138  9086.313	-51.45078	16.3898439  9910.148	-44.50414	8.8337840  12506.41	-54.93793	18.8306241  12807.17	-59.57372	9.0972442  13005.55	-61.56248	14.1042|#(setq l-freqf '(   14.93143   40.05544   47.87826   63.32995   167.1616   177.5058   260.0743   518.5092   780.7788   1045.69   1311.126   1588.701   1850.47   1857.926   2127.85   2412.13   3307.771   3314.749   3584.746   3589.47   3605.28   3910.235   4186.188   4270.069   4353.698   4561.798   4600.229   4831.049   4901.288   4916.701   4924.305   4934.346   5268.153   5297.832   5597.927   5650.533   8467.482   9086.313   9910.148   12506.41   12807.17   13005.55))(setq l-amplf '(   .0003289257   .000232602   .0001291252   .001203795   .0119417   .03053085   .144329   .05647032   .1703323   .05534633   .02134238   .013627   .05797955   .04593678   .02653959   .07061008   .008191222   .009559989   .01907066   .01681286   .02989707   .01447559   .006128961   .007052796   .008319244   .004730201   .005350529   .0002970539   .009389509   .007319784   .0144438   .005749204   .02098454   .01636744   .01067464   .01055471   .002165599   .002675842   .00595378   .00179103   .001050302   .0008353648))(setq l-bandf '(   .02617994   .2008806   .153833   1.548127   7.362475   10.8353   .2208194   .1181496   2.178273   .2838126   .201072   .5085907   .7174416   .372287   1.012622   3.347567   1.194699   1.487281   1.140618   .9939631   2.803281   1.785283   1.403326   4.732128   12.09344   1.200751   6.336576   .0494208   2.39389   1.475211   1.498723   .8224236   12.3483   4.656667   6.529278   5.876072   3.084511   16.38984   8.83378   18.83062   9.09724   14.1042))