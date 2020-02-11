#include <math.h>

double rh2pa(double rh,double ta)
{
    return rh * 10 * exp(16.6536 - 4030.183 / (ta + 235));
}

double pa2rh(double pa,double ta)
{
    return pa/(10 * exp(16.6536 - 4030.183 / (ta + 235)));
}

int pmvppd(double *pmv,
           double *ppd,
           double CLO,
           double MET,
           double WME,
           double TA,
           double TR,
           double VEL,
           double RH,
           double PA)
{
    //---Initial Calculation
    if (PA < 1e-3)
        PA = rh2pa(RH,TA);
//       PA = RH * 10 * exp(16.6536 - 4030.183 / (TA + 235));          //water vapor pressure, Pa
    double ICL = 0.155*CLO;                       //thermal insulation of the clothing in m2K/W
    double M = MET * 58.15;                         //metabolic rate in W/m2
    double W = WME * 58.15;                           //external work in W/m2
    double MW = (M - W);                             //internal heat production in the human body

    double FCL;
    if (ICL <= 0.078)
        FCL = 1 + 1.29*ICL;
    else
        FCL = 1.05 + 0.645*ICL;

    //---heat transf. coeff. by forced convection
    double hcf = 12.1*sqrt(VEL);
    double taa = TA + 273;
    double tra = TR + 273;
    double tcla = taa + (35.5 - TA) / (3.5*ICL + 0.1);
    double p1 = ICL * FCL;
    double p2 = p1 * 3.96;
    double p3 = p1 * 100;
    double p4 = p1 * taa;
    double p5 = (308.7 - 0.028*MW) + (p2*pow((tra / 100), 4));
    double xn = tcla / 100;
    double xf = tcla / 50;
    double eps = 0.00015;

    double hcn, hc=hcf;

    double n = 0;
    while (fabs(xn - xf) > eps)
    {
        xf = (xf + xn) / 2;
        hcn = 2.38*pow(fabs(100 * xf - taa), 0.25);
        if (hcf > hcn)
            hc = hcf;
        else
            hc = hcn;

        xn = (p5 + p4 * hc - p2 * pow(xf, 4)) / (100 + p3 * hc);

        n = n + 1;

        if (n > 150)
        {
            *pmv = -999999;
            *ppd = -999999;

            return -1;

        }
    }

    double tcl = 100 * xn - 273;

    //heat loss diff. through skin
    double hl1 = 3.05*0.001*(5733 - (6.99*MW) - PA);
    //heat loss by sweating
    double hl2;
    if (MW > 58.15)
        hl2 = 0.42*(MW - 58.15);
    else
        hl2 = 0;
    //latent respiration heat loss
    double hl3 = 1.7*0.00001*M*(5867 - PA);
    //dry respiration heat loss
    double hl4 = 0.0014*M*(34 - TA);
    //heat loss by radiation
    double hl5 = 3.96*FCL*(pow(xn, 4) - pow(tra / 100, 4));
    //heat loss by convection
    double hl6 = FCL * hc*(tcl - TA);

    double ts = 0.303*exp(-0.036*M) + 0.028;


    //---Predicted Mean Vote - PMV
    double _pmv = ts * (MW - hl1 - hl2 - hl3 - hl4 - hl5 - hl6);

    *pmv = _pmv;

    //------Predicted Percentage Dissatisfied - PPD
    double _ppd = 100 - 95 * exp(-0.03353*pow(_pmv, 4) - 0.2179*pow(_pmv, 2));
    *ppd = _ppd;

    return 1;
}
