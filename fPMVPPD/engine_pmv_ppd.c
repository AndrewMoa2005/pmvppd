// convert from : https://github.com/RaynerMauricio/pmvppd/blob/master/js/engine_pmv_ppd.js

#include "engine_pmv_ppd.h"
#include <math.h>
#include <float.h>
#define MIN(i, j) (((i) < (j)) ? (i) : (j))
#define MAX(i, j) (((i) > (j)) ? (i) : (j))
#ifdef DEBUG
#include <stdio.h>
#endif

#define EPS 1e-6
#define CYCLE_LIMITS 10000

// function PMV low air speed model, ISO 7730:2005, GB/T 18049-2017
int PMVlow(double CLO /*clothing insulation [clo], input value*/
	, double MET /*metabolic rate [met], input value*/
	, double WME /*mechanical work [met], input value*/
	, double TA /*air temperature [°C], input value*/
	, double TR /*mean radiant temperature [°C], input value*/
	, double VEL /*air speed [m/s], input value*/
	, double RH /*relative humidity [%], input value*/
	, double PA /*water vapour pressure [Pa], input default = 0*/
	, double* PMV /*predicted mean vote [dimensionless], input ptr and return*/
	, double* PPD /*predicted percentage dissatisfied [%], input ptr and return*/)
{
	//---Initial Calculation
	double FNPST = exp(16.6536 - 4030.183 / (TA + 235)); //saturated vapor pressure KPa

	double mPA;
	if (fabs(PA - 0) <= 1e-4) {
		mPA = RH * 10 * FNPST; //water vapor pressure, Pa
	}
	else {
		mPA = PA;
	}
	double ICL = 0.155 * CLO; //thermal insulation of the clothing in m2K/W
	double M = MET * 58.15; //metabolic rate in W/m2
	double W = WME * 58.15; //external work in W/m2
	double MW = (M - W); //internal heat production in the human body

	double FCL;
	if (ICL <= 0.078) {
		FCL = 1 + 1.29 * ICL;
	}
	else {
		FCL = 1.05 + 0.645 * ICL;
	}

	//---heat transf. coeff. by forced convection
	double hcf = 12.1 * sqrt(VEL);
	double taa = TA + 273;
	double tra = TR + 273;
	double tcla = taa + (35.5 - TA) / (3.5 * ICL + 0.1);
	double p1 = ICL * FCL;
	double p2 = p1 * 3.96;
	double p3 = p1 * 100;
	double p4 = p1 * taa;
	double p5 = (308.7 - 0.028 * MW) + (p2 * pow((tra / 100), 4));
	double xn = tcla / 100;
	double xf = tcla / 50;
	int cycle_limits = 10000;

	int n = 0;
	double hcn, hc;
	while (fabs(xn - xf) > EPS) {
		if (n > CYCLE_LIMITS) {
#ifdef DEBUG
			printf("Maximum number of iterations exceeded in PMV low air speed model n=%d...\n", n - 1);
#endif
			* PMV = 999999;
			*PPD = 100;
			return 0;
		}
		xf = (xf + xn) / 2;
		hcn = 2.38 * pow(fabs(100 * xf - taa), 0.25);
		if (hcf > hcn) {
			hc = hcf;
		}
		else {
			hc = hcn;
		}
		xn = (p5 + p4 * hc - p2 * pow(xf, 4)) / (100 + p3 * hc);
		n += 1;
	}

	double tcl = 100 * xn - 273;
	//heat loss diff. through skin
	double hl1 = 3.05 * 0.001 * (5733 - (6.99 * MW) - mPA);
	//heat loss by sweating
	double hl2;
	if (MW > 58.15) {
		hl2 = 0.42 * (MW - 58.15);
	}
	else {
		hl2 = 0;
	}
	//latent respiration heat loss
	double hl3 = 1.7 * 0.00001 * M * (5867 - mPA);
	//dry respiration heat loss
	double hl4 = 0.0014 * M * (34 - TA);
	//heat loss by radiation
	double hl5 = 3.96 * FCL * (pow(xn, 4) - pow((tra / 100), 4));
	//heat loss by convection
	double hl6 = FCL * hc * (tcl - TA);

	double ts = 0.303 * exp(-0.036 * M) + 0.028;

	//---Predicted Mean Vote - PMV
	*PMV = ts * (MW - hl1 - hl2 - hl3 - hl4 - hl5 - hl6);

	//------Predicted Percentage Dissatisfied - PPD
	*PPD = 100 - 95 * exp(-0.03353 * pow(*PMV, 4) - 0.2179 * pow(*PMV, 2));
	return 1;
}

// function PMV elevated air speed model
int PMVelevated(double CLO /*clothing insulation [clo], input value*/
	, double MET /*metabolic rate [met], input value*/
	, double WME /*mechanical work [met], input value*/
	, double TA /*air temperature [°C], input value*/
	, double TR /*mean radiant temperature [°C], input value*/
	, double VEL /*air speed [m/s], input value*/
	, double RH /*relative humidity [%], input value*/
	, double PA /*water vapour pressure [Pa], input default = 0*/
	, double* PMV /*predicted mean vote [dimensionless], input ptr and return*/
	, double* PPD /*predicted percentage dissatisfied [%], input ptr and return*/)
{

	//---Secant method of iteration
	double a1 = 0;
	double b1 = 100;
	int count1 = 1;
	double F1 = SETashrae(CLO, MET, WME, TA - a1, TR - a1, 0.10, RH) - SETashrae(CLO, MET, WME, TA, TR, VEL, RH);
	double ce;
	if (fabs(F1) < 0.001)
		ce = a1;
	double F2 = SETashrae(CLO, MET, WME, TA - b1, TR - b1, 0.10, RH) - SETashrae(CLO, MET, WME, TA, TR, VEL, RH);
	if (fabs(F2) < 0.001)
		ce = b1;

	double slope, c1;
	int b_ce = 1;
	while (count1 <= CYCLE_LIMITS) {
		slope = (F2 - F1) / (b1 - a1);
		c1 = b1 - F2 / slope;

		if (c1 > 999999 || c1 == DBL_MAX || c1 == DBL_MIN) {
			b_ce = 0;
			break;
		}

		double F3 = SETashrae(CLO, MET, WME, TA - c1, TR - c1, 0.10, RH) - SETashrae(CLO, MET, WME, TA, TR, VEL, RH);

		if (fabs(F3) < EPS) {
			ce = c1;
			break;
		}
		a1 = b1;
		b1 = c1;
		F1 = F2;
		F2 = F3;

		count1 += 1;

		if (count1 > CYCLE_LIMITS) {
			b_ce = 0;
#ifdef DEBUG
			print("Maximum number of iteration exceeded in secant method of PMV elevated air speed model count1=%d...\n", count1 - 1);
#endif
			break;
		}
	}

	//---Bisection method of iteration
	double a2 = -85;
	double b2 = +94;
	double DIFF = 0.5;
	int count2 = 1;
	double c2, Eq1, Eq2, Eq3;;
	if (b_ce == 0) {
		while (fabs(DIFF) > EPS) {
			c2 = (a2 + b2) / 2;
			Eq1 = SETashrae(CLO, MET, WME, TA - a2, TR - a2, 0.10, RH) - SETashrae(CLO, MET, WME, TA, TR, VEL, RH);
			Eq2 = SETashrae(CLO, MET, WME, TA - b2, TR - b2, 0.10, RH) - SETashrae(CLO, MET, WME, TA, TR, VEL, RH);
			Eq3 = SETashrae(CLO, MET, WME, TA - c2, TR - c2, 0.10, RH) - SETashrae(CLO, MET, WME, TA, TR, VEL, RH);

			if (Eq1 * Eq3 < 0) {
				b2 = c2;
			}
			if (Eq1 * Eq3 > 0) {
				a2 = c2;
			}
			if (fabs(Eq3) < EPS) {
				break;
			}

			DIFF = fabs(b2 - a2);
			ce = c2;
			count2 += 1;

			if (count2 > CYCLE_LIMITS) {
#ifdef DEBUG
				printf("Maximum number of iteration exceeded in bisection method of PMV elevated air speed model count2=%d...\n", count2 - 1);
#endif
				* PMV = 999999;
				*PPD = 100;
				return 0;
			}

		}
	}
	else {
		ce = c1;
	}

	//PMV and PPD
	if (PMVlow(CLO, MET, WME, TA - ce, TR - ce, 0.10, RH, PA, PMV, PPD) <= 0) {
		*PMV = 999999;
		*PPD = 100;
		return 0;
	}
	return 1;
}

//---vapor pressure function---
double FindSVP(double Temperature /*[°C]*/)
{
	return exp(18.6686 - 4030.183 / (Temperature + 235));
}

// function SET : standard effective temperature
double SETashrae(double CLO /*clothing insulation [clo], input value*/
	, double MET /*metabolic rate [met], input value*/
	, double WME /*mechanical work [met], input value*/
	, double TA /*air temperature [°C], input value*/
	, double TR /*mean radiant temperature [°C], input value*/
	, double VEL /*air speed [m/s], input value*/
	, double RH /*relative humidity [%], input value*/)
{

	double PA = 101.325;   //---Default of 1 atm equal to 101.325 kPa

	//---Constants
	double BodyWeight = 69.9;
	double BodySurfaceArea = 1.8258;
	double MetFactor = 58.2;
	double StefanB = 5.6697 * pow(10, -8); //---constante de Stefan Boltzmann [W/m2K4]
	double TTCR = 36.49; //---neutral core body temperature
	double TTSK = 33.7; //---neutral skin body temperature
	double TTBM = 36.49; //---neutral body temperature
	double SKBFN = 6.3; //---skin blood flow neutral
	double PressureInAtmospheres = PA * 0.0098692; //---presure from kPa to atm

	//---Initial values
	double VaporPressure = RH * FindSVP(TA) / 100;
	double AirVelocity = MAX(VEL, 0.1); //---0.1 m/s at least
	double M = MET * MetFactor;
	double RM = MET * MetFactor;
	double RCL = 0.155 * CLO; //---m2K/W
	double FACL = 1 + 0.15 * CLO;

	double LR = 2.2 / PressureInAtmospheres;
	double alpha = 0.1; //---actual skin to total body mass ratio
	double MSHIV = 0;
	double ESK = 0.1 * MET;

	//---Simulation initial values
	double TSK = TTSK; //---Temperature Skin
	double TCR = TTCR; //---Temperature Core
	double SKBF = SKBFN; //---Skin Blood Flow

	//---Physiological temperatura regulation controls
	double CDIL = 120; //----liters/(m2.h.K)
	double CSTR = 0.5; //----dimensionless
	double CSW = 170; //----g/(m2.h)
	double SKBFL = 90; //----liter/(m2h) max SKBF
	double REGSWL = 500;

	double CHCt = 3 * pow(PressureInAtmospheres, 0.53);
	double CHCV = 8.600001 * pow(AirVelocity * PressureInAtmospheres, 0.53);
	double CHC = MAX(CHCt, CHCV);
	double CHR = 4.7;
	double CTC = CHR + CHC;
	double RA = 1 / (FACL * CTC);
	double TOP = (CHR * TR + CHC * TA) / CTC;
	double TCL = TOP + (TSK - TOP) / (CTC * (RA + RCL));

	double WCRIT, ICL;

	if (CLO <= 0) {
		WCRIT = 0.38 * pow(AirVelocity, -0.29);
		ICL = 1;
	}
	else {
		WCRIT = 0.59 * pow(AirVelocity, -0.08);
		ICL = 0.45;
	}

	////////////////////////////////////////////////////---Begin 60 minutes simulation---////////////////////////////////////////////////////
	int LTIME = 60; //---Timestep equal to 60
	int TIM = 1;
	double TCLold = TCL;
	int flag = 1;
	double DRY, PWET;
	while (TIM <= LTIME) {
		//---Dry heat balance - Solve TCL and CHR
		double DIFF1 = 0.5;
		if (flag > 0) {
			int count1 = 0;
			while (DIFF1 > EPS) {
				TCLold = TCL;
				CHR = 4 * 0.72 * StefanB * pow((TCL + TR) / 2 + 273.15, 3);
				CTC = CHR + CHC;
				RA = 1 / (FACL * CTC);
				TOP = (CHR * TR + CHC * TA) / CTC;
				DIFF1 = fabs(TCLold - TCL);
				TCL = (RA * TSK + RCL * TOP) / (RA + RCL);
				count1 += 1;

				//---Iteration from bisection method
				if (DIFF1 > 99999) { //---an alternative control from non-convergente (rarely used)
					double DIFF5 = 0.5;
					double o1 = -300;
					double o2 = +350;
					int count2 = 0;
					double o3, funo1, funo2, funo3;
					while (fabs(DIFF5) > EPS) {
						o3 = (o1 + o2) / 2;
						TCL = o1;
						CHR = 4 * 0.72 * StefanB * pow((TCL + TR) / 2 + 273.15, 3);
						CTC = CHR + CHC;
						RA = 1 / (FACL * CTC);
						TOP = (CHR * TR + CHC * TA) / CTC;
						funo1 = (RA * TSK + RCL * TOP) / (RA + RCL) - TCL;

						TCL = o2;
						CHR = 4 * 0.72 * StefanB * pow((TCL + TR) / 2 + 273.15, 3);
						CTC = CHR + CHC;
						RA = 1 / (FACL * CTC);
						TOP = (CHR * TR + CHC * TA) / CTC;
						funo2 = (RA * TSK + RCL * TOP) / (RA + RCL) - TCL;

						TCL = o3;
						CHR = 4 * 0.72 * StefanB * pow((TCL + TR) / 2 + 273.15, 3);
						CTC = CHR + CHC;
						RA = 1 / (FACL * CTC);
						TOP = (CHR * TR + CHC * TA) / CTC;
						funo3 = (RA * TSK + RCL * TOP) / (RA + RCL) - TCL;

						if (funo1 * funo3 < 0) {
							o2 = o3;
						}
						if (funo1 * funo3 > 0) {
							o1 = o3;
						}
						if (fabs(funo3) < EPS)
							break;

						DIFF5 = fabs(o2 - o1);
						TCL = o3;
						count2 += 1;

						if (count2 > CYCLE_LIMITS) {
#ifdef DEBUG
							printf("Maximum number of iteration exceeded in TCL inside SET from bisection method count2=%d...\n", count2 - 1);
#endif
							break;
						}
					}
				}//---End of control of non-cnovergence
			}
		}
		flag = 0;
		//---Heat flow from clothing surface to environment (FACL=1 if CLOE used)
		DRY = (TSK - TOP) / (RA + RCL);
		double HFCS = (TCR - TSK) * (5.28 + 1.163 * SKBF);

		//---Dry and latent respiratory heat losses
		double ERES = 0.0023 * M * (44 - VaporPressure);
		double CRES = 0.0014 * M * (34 - TA);

		double SCR = M - HFCS - ERES - CRES - WME;
		double SSK = HFCS - DRY - ESK;

		//---Thermal capacities
		double TCSK = 0.97 * alpha * BodyWeight;
		double TCCR = 0.97 * (1 - alpha) * BodyWeight;

		//---Temperature changes in 1 minute
		double DTSK = (SSK * BodySurfaceArea) / (TCSK * 60);
		double DTCR = (SCR * BodySurfaceArea) / (TCCR * 60);
		TSK = TSK + DTSK;
		TCR = TCR + DTCR;
		double TB = alpha * TSK + (1 - alpha) * TCR;

		//---Definition of vascular control signals
		double WARMS, COLDS;
		if (TSK > TTSK) {
			WARMS = TSK - TTSK;
			COLDS = 0;
		}
		else {
			COLDS = TTSK - TSK;
			WARMS = 0;
		}
		double WARMC, COLDC;
		if (TCR > TTCR) {
			WARMC = TCR - TTCR;
			COLDC = 0;
		}
		else {
			COLDC = TTCR - TCR;
			WARMC = 0;
		}
		double WARMB, COLDB;
		if (TB > TTBM) {
			WARMB = TB - TTBM;
			COLDB = 0;
		}
		else {
			COLDB = TTBM - TB;
			WARMS = 0;
		}

		//---Control skin blood flow
		double DILAT = CDIL * WARMC;
		double STRIC = CSTR * COLDS;
		SKBF = (SKBFN + DILAT) / (1 + STRIC);

		//---SKBF is never below 0.5 liter/m2h not above SKBFL
		SKBF = MAX(0.5, MIN(SKBFL, SKBF));

		//---control of regulatory sweting
		double REGSW = CSW * WARMB * exp(WARMS / 10.7);
		REGSW = MIN(REGSW, REGSWL);
		double ERSW = 0.68 * REGSW;

		//---Mass transfer equation between skin and environment
		double REA = 1 / (LR * FACL * CHC);
		double RECL = RCL / (LR * ICL);
		double EMAX = (FindSVP(TSK) - VaporPressure) / (REA + RECL);
		double PRSW = ERSW / EMAX;

		//---PDIF for nonsweating skin
		PWET = 0.06 + 0.94 * PRSW;
		double EDIF = PWET * EMAX - ERSW;
		ESK = ERSW + EDIF;

		//---Beginning of dripping (swet not evaporated on skin surface)
		if (PWET > WCRIT) {
			PWET = WCRIT;
			PRSW = WCRIT / 0.94;
			ERSW = PRSW * EMAX;
			EDIF = 0.06 * (1 - PRSW) * EMAX;
			ESK = ERSW + EDIF;
		}

		//---When EMAX<0 condensation on skin ocurs
		if (EMAX < 0)
		{
			EDIF = 0;
			ERSW = 0;
			PWET = WCRIT;
			PRSW = WCRIT;
			ESK = EMAX;
		}

		//---Adjustment of metabolic heat due to shivering
		ESK = ERSW + EDIF;
		MSHIV = 19.4 * COLDS * COLDC;
		M = RM + MSHIV;

		//---Ratio of skin-core masses change with SKBF
		alpha = 0.0417737 + 0.7451833 / (SKBF + 0.585417);

		TIM += 1; //---Next minute
	}
	////////////////////////////////////////////////////---End 60 minutes simulation---////////////////////////////////////////////////////

	double HSK = DRY + ESK;
	double RN = M - WME;
	double ECOMF = 0.42 * (RN - (1 * MetFactor));
	if (ECOMF < 0) {
		ECOMF = 0;
	}
	double EMAX = EMAX * WCRIT;
	double W = PWET;
	double PSSK = FindSVP(TSK);

	//---Standard environment
	double CHRS = CHR;

	//---CHCS=standard convective heat transfer coefficient (level walking/still air)
	double CHCS;
	if (MET < 0.85) {
		CHCS = 3;
	}
	else {
		double CHCSt = 5.66 * pow((MET - 0.85), 0.39);
		CHCS = MAX(3, CHCSt);
	}

	double KCLO = 0.25;
	double CTCS = CHCS + CHRS;
	double RCLOS = 1.52 / ((MET - WME / MetFactor) + 0.6944) - 0.1835;
	double RCLS = 0.155 * RCLOS;
	double FACLS = 1 + KCLO * RCLOS;
	double FCLS = 1 / (1 + 0.155 * FACLS * CTCS * RCLOS);
	double IMS = 0.45;
	double ICLS = IMS * CHCS / CTCS * (1 - FCLS) / (CHCS / CTCS - FCLS * IMS);
	double RAS = 1 / (FACLS * CTCS);
	double REAS = 1 / (LR * FACLS * CHCS);
	double RECLS = RCLS / (LR * ICLS);
	double HDS = 1 / (RAS + RCLS);
	double HES = 1 / (REAS + RECLS);

	////////////////////////////////////////////////////---Standard Effective Temperature SET---////////////////////////////////////////////////////

	//---Newton method of iteration
	double a3 = TSK - HSK / HDS; //---Initial estimation of SET
	double DIFF3 = 100;
	int count3 = 0;
	double delta = 0.0001;
	double SETOLD = a3;
	double ER1, ER2, SET;
	while (fabs(DIFF3) > EPS) {
		ER1 = HSK - HDS * (TSK - SETOLD) - W * HES * (PSSK - 0.5 * FindSVP(SETOLD));
		ER2 = HSK - HDS * (TSK - SETOLD - delta) - W * HES * (PSSK - 0.5 * FindSVP(SETOLD + delta));
		SET = SETOLD - delta * ER1 / (ER2 - ER1);
		DIFF3 = SET - SETOLD;
		SETOLD = SET;
		count3 += 1;

		if (count3 > CYCLE_LIMITS) {
#ifdef DEBUG
			printf("Maximum number of iteration exceeded in SET iteration for Newton method count3=%d...\n", count3 - 1);
#endif
			break;
		}

		if (fabs(DIFF3) > 999999) {
			//SET=NULL;
			//SETOLD=NULL;
			double DIFF2 = 0.5;
			int count4 = 0;
			double a2 = -301;
			double b2 = 486;
			double c2, ERR1, ERR2, ERR3;
			while (DIFF2 > EPS) {
				c2 = (b2 + a2) / 2;
				SETOLD = a2;
				ERR1 = HSK - HDS * (TSK - SETOLD) - W * HES * (PSSK - 0.5 * FindSVP(SETOLD));
				SETOLD = b2;
				ERR2 = HSK - HDS * (TSK - SETOLD) - W * HES * (PSSK - 0.5 * FindSVP(SETOLD));
				SETOLD = c2;
				ERR3 = HSK - HDS * (TSK - SETOLD) - W * HES * (PSSK - 0.5 * FindSVP(SETOLD));
				if (ERR1 * ERR3 < 0) {
					b2 = c2;
				}
				if (ERR1 * ERR3 > 0) {
					a2 = c2;
				}
				if (fabs(ERR3) < EPS)
					break;
				DIFF2 = fabs(b2 - a2);
				SETOLD = c2;
				count4 += 1;

				if (count4 > CYCLE_LIMITS)
				{
#ifdef DEBUG
					printf("Maximum number of iteration exceeded in SET iteration for bisection method count4=%d...\n", count4 - 1);
#endif
					break;
				}
				SET = SETOLD; //---iterative calculation
			}
		}
	}
	return SET;
}

// function PMV both
int PMVboth(double CLO /*clothing insulation [clo], input value*/
	, double MET /*metabolic rate [met], input value*/
	, double WME /*mechanical work [met], input value*/
	, double TA /*air temperature [°C], input value*/
	, double TR /*mean radiant temperature [°C], input value*/
	, double VEL /*air speed [m/s], input value*/
	, double RH /*relative humidity [%], input value*/
	, double PA /*water vapour pressure [Pa], input default = 0*/
	, double* PMV /*predicted mean vote [dimensionless], input ptr and return*/
	, double* PPD /*predicted percentage dissatisfied [%], input ptr and return*/
	, double* SET /*standard effective temperature [°C], input ptr and return*/)
{
	if (VEL <= 0.10) {
		if (PMVlow(CLO, MET, WME, TA, TR, VEL, RH, PA, PMV, PPD) < 1)
			return 0;
	}
	if (VEL > 0.10) {
		if (PMVelevated(CLO, MET, WME, TA, TR, VEL, RH, PA, PMV, PPD) < 1)
			return 0;
	}
	*SET = SETashrae(CLO, MET, WME, TA, TR, VEL, RH);
	return 1;
}
