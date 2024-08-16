#ifndef ENGINE_PMV_PPD_HPP
#define ENGINE_PMV_PPD_HPP

#ifdef __cplusplus
extern "C" {
#endif

	//---vapor pressure function---
	double FindSVP(double Temperature /*[°C]*/);

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
		, double* PPD /*predicted percentage dissatisfied [%], input ptr and return*/);

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
		, double* PPD /*predicted percentage dissatisfied [%], input ptr and return*/);

	// function SET : standard effective temperature
	double SETashrae(double CLO /*clothing insulation [clo], input value*/
		, double MET /*metabolic rate [met], input value*/
		, double WME /*mechanical work [met], input value*/
		, double TA /*air temperature [°C], input value*/
		, double TR /*mean radiant temperature [°C], input value*/
		, double VEL /*air speed [m/s], input value*/
		, double RH /*relative humidity [%], input value*/);

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
		, double* SET /*standard effective temperature [°C], input ptr and return*/);

#ifdef __cplusplus
}
#endif

#endif // ENGINE_PMV_PPD_HPP
