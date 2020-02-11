#ifndef PMVPPD_H
#define PMVPPD_H

int pmvppd(double *pmv,
           double *ppd,
           double clo, /* 服装热阻,clo */
           double met, /* 代谢率,met */
           double wme, /* 外部做功,met */
           double ta, /* 空气温度,C */
           double tr, /* 平均辐射温度,C */
           double vel, /*空气流速,m/s */
           double rh, /*相对湿度,% */
           double pa /*水蒸气分压力,Pa */);

double rh2pa(double rh, /*相对湿度,% */
             double ta /* 空气温度,C */);

double pa2rh(double pa, /*水蒸气分压力,Pa */
             double ta /* 空气温度,C */);

#endif // PMVPPD_H
