1
4
5
7
8
9
10
13
21
22
23
26
28
29
30
32
34
35
36
38
39
42
43
44
46
48
50
51
52
54
58
59
60
61
62
64
66
71
73
74
76
77
78
81
83
87
89
92
93
94
95
97
98
99
100
101
103
104
107
110
111
112
113
114
115
117
118
120
121
126
127
128
129
131
132
133
134
135
136
138
139
140
144
145
147
148
149
150
152
155
157
158
159
160
161
162
163
164
166
167
168
170
172
173
175
179
181
182
184
185
186
187
188
190
191
192
193
194
195
196
199
202
203
204
206
207
208
210
211
212
213
214
215
216
218
219
220
221
222
225
227
228
229
230
231
233
234
235
237
238
242
243
245
246
249
252
253
254
255
256
257
258
260
261
263
264
265
267
270
271
272
274
275
276
278
280
281
285
286
287
288
289
290
291
294
295
296
300
301
302
304
305
306
308
309
312
313
314
315
316
317
319
322
324
325
326
327
328
329
330
333
334
336
338
339
340
341
342
344
348
350
351
352
353
354
355
357
360
361
363
364
365
367
370
371
375
376
377
378
382
383
384
387
389
390
391
392
394
395
396
397
399
400
403
404
405
409
410
412
413
414
415
416
418
420
421
422
423
425
426
427
429
430
432
433
434
437
438
439
441
442
444
445
447
448
449
451
452
453
454
455
456
457
458
462
463
465
466
467
468
469
470
472
473
477
478
479
484
486
489
490
491
492
495
496
497
498
499
501
502
504
505
507
508
509
511
512
513
515
517
518
519
521
523
525
526
528
529
530
531
532
533
536
541
544
545
546
548
551
552
554
555
556
560
562
563
564
566
567
568
570
571
572
575
576
579
580
581
582
583
585
587
588
589
590
592
593
594
595
596
597
600
601
603
604
605
608
610
611
614
615
616
617
618
620
622
627
628
629
630
631
632
634
635
636
637
638
639
640
641
643
645
646
647
649
650
651
654
655
656
658
659
662
663
666
667
670
673
675
676
678
679
680
681
683
684
685
686
688
691
692
693
694
695
699
701
703
705
707
708
709
710
711
712
713
714
715
718
719
720
721
722
723
724
725
726
728
729
731
733
734
735
737
738
739
740
741
742
744
745
746
747
748
749
750
752
754
755
758
759
761
763
766
769
770
771
773
776
777
778
779
780
782
783
785
787
790
793
794
795
796
799
800
802
804
805
807
809
810
811
812
813
814
815
816
817
821
823
826
827
828
829
830
832
833
836
837
838
839
840
841
844
847
848
849
850
851
852
854
856
857
858
859
861
863
864
865
866
868
869
870
871
873
877
878
880
881
882
884
885
886
887
888
889
890
892
894
895
896
898
899
901
902
906
909
910
912
913
914
915
917
919
920
921
923
924
925
926
928
929
931
934
935
936
937
938
939
941
942
943
945
946
947
948
949
951
952
957
960
962
965
966
969
971
972
973
975
976
978
979
980
982
983
985
986
987
988
989
991
993
995
998
999
1001
1002
1006
1007
1008
1009
1012
1013
1014
1016
1017
1019
1020
1021
1022
1023
1024
1026
1027
1030
1031
1033
1034
1035
1037
1040
1041
1043
1044
1047
1048
1049
1053
1058
1059
1061
1062
1063
1064
1065
1066
1067
1068
1069
1071
1073
1077
1078
1079
1080
1082
1084
1087
1089
1092
1095
1096
1097
1098
1099
1100
1101
1104
1105
1106
1107
1108
1110
1112
1114
1115
1117
1119
1122
1124
1125
1126
1129
1130
1131
1132
1133
1134
1139
1140
1141
1143
1145
1146
1149
1151
1152
1155
1157
1158
1160
1161
1162
1163
1165
1166
1167
1168
1170
1171
1172
1173
1175
1177
1178
1179
1180
1181
1182
1183
1184
1189
1192
1193
1195
1196
1198
1199
1200
1202
1205
1206
1207
1209
1210
1211
1212
1214
1216
1217
1219
1220
1223
1225
1226
1227
1229
1230
1231
1233
1236
1237
1238
1239
1241
1244
1245
1246
1247
1248
1250
1252
1254
1257
1258
1259
1261
1262
1264
1266
1268
1270
1273
1274
1275
1276
1277
1279
1280
1281
1282
1285
1286
1287
1289
1291
1293
1294
1295
1296
1298
1299
1301
1304
1305
1306
1307
1308
1309
1313
1314
1315
1316
1320
1321
1323
1324
1325
1328
1329
1330
1333
1334
1338
1339
1341
1342
1345
1346
1347
1348
1349
1350
1353
1356
1357
1359
1361
1362
1363
1365
1367
1369
1370
1371
1372
1374
1378
1379
1380
1382
1384
1387
1388
1389
1390
1391
1397
1398
1399
1402
1403
1404
1405
1407
1408
1410
1411
1412
1416
1418
1419
1420
1421
1422
1423
1424
1426
1427
1429
1431
1433
1434
1435
1436
1437
1438
1439
1441
1442
1444
1445
1446
1448
1449
1452
1455
1456
1458
1459
1461
1464
1467
1469
1470
1471
1472
1474
1475
1479
1480
1481
1482
1483
1484
1486
1487
1489
1491
1492
1493
1494
1495
1496
1498
1503
1504
1508
1509
1511
1514
1516
1518
1519
1520
1521
1522
1524
1526
1527
1528
1530
1531
1533
1534
1535
1536
1537
1538
1539
1540
1547
1550
1551
1552
1553
1555
1556
1558
1559
1560
1561
1562
1564
1566
1567
1568
1571
1572
1573
1574
1575
1577
1581
1582
1584
1586
1587
1589
1591
1592
1593
1594
1595
1597
1598
1600
1602
1603
1607
1612
1613
1614
1616
1618
1620
1621
1622
1625
1626
1629
1631
1632
1633
1634
1635
1636
1638
1640
1641
1642
1644
1647
1648
1649
1652
1653
1654
1656
1657
1660
1661
1662
1664
1666
1668
1670
1671
1672
1673
1674
1676
1678
1679
1680
1683
1684
1686
1691
1692
1694
1696
1697
1699
1701
1703
1705
1706
1707
1709
1710
1711
1712
1713
1715
1716
1718
1721
1723
1726
1727
1728
1729
1731
1732
1734
1735
1736
1740
1741
1742
1745
1746
1747
1748
1749
1750
1752
1753
1755
1756
1759
1760
1761
1762
1763
1767
1771
1772
1773
1776
1777
1778
1780
1781
1784
1785
1787
1788
1789
1790
1791
1792
1795
1796
1797
1798
1799
1800
1801
1802
1805
1806
1809
1812
1815
1816
1818
1819
1821
1823
1825
1826
1827
1829
1832
1833
1836
1839
1842
1843
1844
1846
1847
1848
1850
1852
1854
1857
1858
1860
1861
1863
1864
1865
1866
1868
1869
1870
1871
1875
1876
1878
1880
1881
1882
1884
1886
1888
1889
1890
1892
1893
1897
1898
1899
1901
1903
1904
1905
1906
1907
1908
1909
1911
1912
1915
1916
1918
1922
1924
1925
1927
1929
1932
1935
1936
1938
1941
1942
1943
1944
1946
1948
1949
1950
1951
1954
1955
1958
1962
1963
1964
1965
1969
1970
1971
1972
1975
1976
1977
1978
1979
1983
1984
1986
1988
1990
1991
1992
1993
1995
1996
1999
2000
2002
2003
2005
2006
2007
2009
2011
2012
2013
2016
2017
2018
2020
2021
2022
2023
2026
2027
2029
2031
2033
2034
2035
2037
2038
2039
2041
2043
2045
2047
2048
2051
2054
2055
2056
2057
2062
2065
2067
2068
2069
2070
2071
2072
2074
2078
2082
2083
2084
2086
2087
2089
2090
2091
2092
2095
2096
2097
2098
2100
2101
2102
2103
2104
2105
2108
2111
2112
2114
2115
2117
2119
2121
2122
2124
2125
2126
2128
2131
2133
2134
2136
2137
2138
2139
2140
2141
2144
2146
2147
2148
2149
2150
2151
2152
2153
2154
2157
2158
2159
2160
2161
2163
2167
2168
2169
2170
2172
2173
2175
2176
2177
2178
2180
2181
2182
2183
2184
2185
2186
2189
2190
2191
2193
2194
2195
2196
2198
2201
2202
2203
2204
2205
2206
2208
2209
2210
2211
2212
2213
2215
2216
2218
2220
2221
2224
2225
2226
2229
2230
2231
2232
2233
2235
2236
2239
2241
2243
2244
2245
2247
2248
2250
2251
2252
2253
2254
2255
2258
2259
2260
2262
2263
2266
2268
2269
2270
2273
2274
2276
2277
2279
2281
2284
2285
2286
2288
2289
2291
2292
2294
2295
2296
2298
2300
2301
2303
2304
2305
2306
2307
2308
2309
2311
2314
2317
2319
2323
2324
2326
2327
2328
2330
2331
2332
2333
2335
2336
2337
2338
2339
2341
2342
2344
2345
2346
2349
2350
2351
2353
2354
2355
2356
2357
2358
2360
2364
2368
2370
2371
2373
2374
2375
2376
2382
2386
2387
2390
2391
2392
2396
2397
2399
2401
2403
2405
2408
2409
2410
2411
2412
2413
2414
2416
2418
2424
2426
2428
2431
2432
2433
2435
2436
2437
2439
2440
2441
2443
2444
2445
2446
2447
2448
2449
2450
2451
2455
2456
2457
2458
2459
2461
2462
2464
2465
2468
2469
2470
2471
2472
2476
2477
2478
2479
2480
2482
2483
2485
2486
2487
2491
2492
2493
2494
2495
2496
2498
2499
2503
2504
2505
2506
2508
2512
2513
2514
2515
2516
2517
2520
2521
2522
2523
2531
2532
2534
2535
2537
2539
2540
2541
2542
2543
2547
2548
2549
2550
2551
2552
2554
2555
2556
2557
2559
2560
2561
2562
2563
2565
2566
2567
2568
2569
2570
2571
2573
2574
2577
2578
2580
2582
2583
2585
2587
2590
2593
2594
2595
2597
2599
2601
2602
2605
2607
2608
2611
2612
2614
2616
2617
2618
2620
2621
2624
2625
2629
2630
2632
2635
2636
2639
2642
2644
2645
2648
2649
2651
2652
2653
2654
2655
2656
2657
2658
2662
2663
2665
2667
2669
2670
2671
2672
2674
2675
2676
2678
2680
2681
2682
2683
2686
2687
2690
2691
2692
2693
2694
2695
2696
2701
2702
2703
2705
2706
2709
2711
2712
2716
2718
2720
2723
2724
2725
2726
2729
2731
2735
2736
2737
2738
2739
2741
2743
2745
2748
2749
2750
2752
2753
2754
2755
2756
2761
2762
2763
2764
2765
2766
2767
2770
2771
2772
2775
2776
2778
2779
2780
2781
2782
2784
2786
2788
2789
2791
2792
2794
2795
2796
2798
2799
2801
2802
2804
2806
2807
2809
2810
2811
2812
2813
2814
2815
2817
2819
2820
2822
2823
2828
2830
2831
2832
2833
2834
2835
2836
2837
2838
2843
2844
2846
2847
2850
2852
2855
2856
2857
2859
2862
2863
2865
2866
2869
2870
2871
2872
2873
2874
2877
2878
2879
2881
2883
2885
2889
2890
2891
2893
2895
2897
2898
2899
2900
2902
2903
2905
2906
2907
2908
2910
2911
2912
2914
2915
2916
2917
2918
2919
2921
2924
2925
2926
2928
2929
2930
2931
2932
2934
2935
2937
2938
2939
2940
2941
2946
2948
2949
2953
2954
2956
2957
2960
2963
2965
2966
2968
2969
2971
2973
2974
2975
2976
2977
2978
2980
2982
2983
2984
2985
2987
2989
2992
2995
2996
2997
3000
3001
3002
3004
3005
3006
3008
3010
3016
3017
3018
3021
3023
3024
3025
3026
3027
3028
3029
3031
3033
3034
3036
3037
3040
3041
3042
3044
3046
3051
3052
3053
3054
3056
3058
3059
3060
3061
3062
3063
3068
3069
3071
3072
3074
3075
3076
3078
3079
3080
3081
3083
3085
3086
3089
3090
3091
3092
3093
3094
3095
3097
3098
3099
3101
3102
3103
3105
3106
3107
3109
3111
3115
3117
3118
3122
3123
3125
3126
3127
3130
3131
3135
3136
3139
3141
3142
3145
3146
3147
3149
3152
3155
3158
3160
3162
3163
3164
3165
3167
3170
3171
3172
3173
3174
3175
3176
3178
3180
3182
3184
3187
3189
3190
3191
3192
3193
3196
3198
3199
3200
3201
3202
3205
3206
3207
3208
3210
3213
3214
3215
3217
3218
3219
3220
3222
3223
3225
3226
3227
3229
3234
3235
3237
3238
3239
3240
3241
3244
3246
3248
3249
3250
3252
3253
3254
3256
3258
3260
3262
3266
3267
3268
3269
3272
3275
3277
3278
3282
3283
3284
3288
3290
3291
3293
3294
3295
3297
3299
3300
3302
3303
3305
3306
3307
3312
3313
3314
3315
3316
3317
3321
3323
3324
3326
3327
3329
3330
3331
3333
3334
3337
3339
3341
3342
3343
3346
3347
3352
3354
3355
3356
3358
3360
3361
3362
3364
3366
3367
3369
3371
3373
3374
3380
3381
3382
3383
3384
3385
3389
3391
3393
3396
3397
3398
3399
3403
3404
3407
3408
3410
3411
3414
3415
3416
3419
3420
3422
3425
3426
3428
3429
3430
3431
3433
3436
3440
3444
3445
3447
3448
3455
3456
3457
3458
3462
3463
3464
3465
3466
3468
3469
3472
3473
3474
3476
3478
3479
3482
3483
3484
3486
3489
3491
3492
3493
3494
3495
3498
3499
3501
3502
3504
3505
3506
3507
3508
3510
3512
3513
3514
3516
3517
3518
3519
3520
3524
3526
3527
3528
3529
3531
3533
3535
3536
3538
3541
3542
3543
3544
3546
3548
3550
3552
3554
3555
3556
3558
3559
3561
3564
3566
3567
3569
3570
3572
3574
3576
3578
3580
3583
3585
3586
3587
3588
3589
3593
3596
3597
3599
3601
3602
3604
3605
3606
3607
3608
3609
3611
3612
3614
3615
3616
3617
3619
3620
3622
3623
3627
3628
3629
3630
3631
3633
3636
3640
3641
3642
3643
3649
3650
3651
3652
3654
3655
3656
3657
3659
3660
3662
3663
3664
3665
3666
3669
3670
3671
3672
3673
3675
3676
3678
3680
3682
3683
3685
3686
3687
3688
3689
3691
3693
3696
3697
3698
3699
3700
3701
3702
3706
3709
3710
3711
3716
3717
3718
3720
3725
3726
3727
3729
3730
3731
3732
3734
3735
3738
3740
3741
3742
3743
3744
3745
3748
3749
3750
3751
3753
3758
3759
3763
3764
3765
3766
3769
3772
3773
3775
3776
3779
3780
3783
3784
3785
3786
3787
3788
3789
3790
3793
3794
3799
3800
3801
3803
3805
3806
3807
3809
3815
3817
3818
3819
3821
3827
3828
3829
3830
3832
3833
3834
3835
3838
3840
3842
3844
3845
3848
3849
3852
3854
3856
3857
3858
3859
3861
3862
3863
3864
3867
3868
3873
3875
3876
3877
3879
3880
3882
3884
3886
3887
3888
3890
3891
3892
3893
3896
3898
3899
3900
3902
3905
3906
3907
3909
3911
3920
3921
3922
3923
3924
3926
3931
3933
3934
3936
3938
3944
3945
3947
3951
3952
3953
3957
3958
3960
3961
3962
3963
3966
3967
3968
3970
3971
3972
3973
3975
3977
3978
3979
3980
3981
3984
3987
3989
3990
3992
3993
3994
3998
3999
4000
4002
4005
4009
4011
4014
4015
4019
4023
4026
4027
4028
4033
4034
4035
4036
4038
4042
4043
4044
4046
4048
4049
4050
4051
4055
4057
4058
4060
4063
4064
4065
4066
4067
4070
4073
4075
4076
4077
4080
4081
4083
4084
4085
4086
4087
4089
4093
4094
4095
4096
