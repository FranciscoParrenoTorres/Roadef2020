
#include "Maintenance.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;



bool Maintenance::comp_descending1(int i, int j)
{
	if ((M_Interventions[i].Get_RiskC(M_I_T[i]) - M_Interventions[i].Get_Min_Risk()) > (M_Interventions[j].Get_RiskC(M_I_T[j]) - M_Interventions[j].Get_Min_Risk()))
		return true;
	else
		return (M_Interventions[i].Get_Min_Risk() > M_Interventions[j].Get_Min_Risk());

}
/*
void Maintenance::VND(double &val)
{
//	printf("\n %d", M_Iter);
	M_Busqueda_Local = true;
	bool mejora = false;
	double val_inicial = val;
	double time_vnd = 0;
	bool silent = M_Challenge_Mode;
	M_Min_Improve = __FLT_EPSILON__;
	for (register int i = 0; i < M_T; i++)
	{
		M_T_M1_Suma[i] = 0;
		M_T_M2_Suma[i] = 0;
		M_T_M3_Suma[i] = 0;
	}
//	M_Min_Improve = ((double)get_random(0, 100) / (double)10000);//.001;
	do
	{

		mejora = false;
		val_inicial = val;
		struct timeb t1;
		ftime(&t1);
		random_shuffle(M_Vec_Alea_I.begin(), M_Vec_Alea_I.end());
		time_vnd = ((double)((t1.time - M_Time_Initial.time) * 1000 + t1.millitm - M_Time_Initial.millitm)) / 1000;

//		if (!M_Challenge_Mode)
//			printf("HiloVND1 %d %.3f\n", M_Hilo, time_vnd);
		if (time_vnd > M_Total_Time)
			break;
//		if (val_inicial < (M_Best_fo + 1))
//			M_Min_Improve = 0.0001;
//		M_Busqueda_Local = true;
		M_VMejora1++;
//		printf("Aquí 0 %d", M_Iter);
		val = MejoraQuitar(val_inicial);

		if (val < (val_inicial - __FLT_EPSILON__) && M_VND == true)
		{
			if (!silent) printf("%.5f 2 %d\n", val, M_Iter);
			M_Mejora1++;
			mejora = true;
			continue;


		}
//		printf("Aquí 1 %d", M_Iter);
//		M_Busqueda_Local = false;
		ftime(&t1);
		time_vnd = ((double)((t1.time - M_Time_Initial.time) * 1000 + t1.millitm - M_Time_Initial.millitm)) / 1000;
		if (!M_Challenge_Mode)
			printf("HiloVND2 %d %.3f\n", M_Hilo, time_vnd);
		if (time_vnd > M_Total_Time)
			break;

			if (M_VMejora2 < 10 || val_inicial < (M_Best_fo + 0.5) || (M_VMejora2 * get_random(0, 100) < (M_Mejora2 + 1) * 200))
			{
				M_VMejora2++;
				val = MejoraIntercambiosDosI(val_inicial);

				if (val < (val_inicial - __FLT_EPSILON__) && M_VND == true)
				{
					if (!silent) printf("%.5f 3 %d\n", val, M_Iter);
					M_Mejora2++;
					mejora = true;
					continue;
				}
			}
			ftime(&t1);
			time_vnd = ((double)((t1.time - M_Time_Initial.time) * 1000 + t1.millitm - M_Time_Initial.millitm)) / 1000;
			if (!M_Challenge_Mode)
				printf("HiloVND3 %d %.3f\n", M_Hilo, time_vnd);
			if (time_vnd > M_Total_Time)
				break;
//			printf("Aquí 2 %d", M_Iter);
			if (M_VMejora3 < 10 || val_inicial < (M_Best_fo + 0.5) || (M_VMejora3 * get_random(0, 100) < (M_Mejora3 + 1) * 300))
			{
				M_VMejora3++;
				val = MejoraIntercambiosMejorT();
				if (val < (val_inicial - __FLT_EPSILON__) && M_VND == true)
				{
					if (!silent) printf("%.5f 4 %d\n", val, M_Iter);
					M_Mejora3++;
					mejora = true;
					continue;
				}
			}
			ftime(&t1);
			time_vnd = ((double)((t1.time - M_Time_Initial.time) * 1000 + t1.millitm - M_Time_Initial.millitm)) / 1000;
			if (!M_Challenge_Mode)
				printf("HiloVND4 %d %.3f\n", M_Hilo, time_vnd);
			if (time_vnd > M_Total_Time)
				break;
//			printf("Aquí 3 %d", M_Iter);
			if (M_VMejora4 < 10 || val_inicial < (M_Best_fo + 0.5) || (M_VMejora4 * get_random(0, 100) < (M_Mejora4 + 1) * 400))
			{
				M_VMejora4++;
				if (M_Pool == false)
					val = MejoraIntercambiosEjection(0);
				else
					val = MejoraIntercambiosEjection(get_random(0, 1));
				if (val < (val_inicial - __FLT_EPSILON__) && M_VND == true)
				{
					if (!silent) printf("%.5f 5 %d\n", val, M_Iter);
					M_Mejora4++;
					mejora = true;
					continue;
				}
			}

//			printf("Aquí 4 %d", M_Iter);

//			printf("Aquí 5 %d", M_Iter);

			ftime(&t1);

			time_vnd = ((double)((t1.time - M_Time_Initial.time) * 1000 + t1.millitm - M_Time_Initial.millitm)) / 1000;

			if (!M_Challenge_Mode)
				printf("HiloVND5 %d %.3f\n", M_Hilo, time_vnd);


	} while (mejora == true && M_VND == true && time_vnd < (M_Total_Time));
//	} while (mejora == true && M_VND == true);
}*/

double Maintenance::VND(double& val)
{
	//	printf("\n %d", M_Iter);
	if (!M_Challenge_Mode) printf("Hilo %d Iter %d ValIni %.4f\n", M_Hilo, M_Iter, val);
	M_Busqueda_Local = true;
	bool mejora = false;
	double val_inicial = val;
	double val_ini2 = val;
	double time_vnd = 0;
	bool silent = M_Challenge_Mode;
	bool Best_solution = false;
	M_Min_Improve = __FLT_EPSILON__;
	for (register int i = 0; i < M_T; i++)
	{
		M_T_M1_Suma[i] = 0;
		M_T_M2_Suma[i] = 0;
		M_T_M3_Suma[i] = 0;
	}
	if (M_Iter < 10)
	{
		M_VMejora1 = 0; M_VMejora2 = 0; M_VMejora3 = 0; M_VMejora4 = 0;
	}
	//	M_Min_Improve = ((double)get_random(0, 100) / (double)10000);//.001;
	do
	{
		Best_solution = false;
		do
		{
			PoolGoodSolutions(M_I_T, val);
			int ndistintos = M_distintos.size();
			M_distintos.insert(hash_operator(M_I_T));
			if (ndistintos == M_distintos.size())
				break;
			mejora = false;
			val_inicial = val;
			struct timeb t1;
			ftime(&t1);
			random_shuffle(M_Vec_Alea_I.begin(), M_Vec_Alea_I.end());
			time_vnd = ((double)((t1.time - M_Time_Initial.time) * 1000 + t1.millitm - M_Time_Initial.millitm)) / 1000;

			//		if (!M_Challenge_Mode)
			//			printf("HiloVND1 %d %.3f\n", M_Hilo, time_vnd);
			if (time_vnd > M_Total_Time)
				break;
			//		if (val_inicial < (M_Best_fo + 1))
			//			M_Min_Improve = 0.0001;
			//		M_Busqueda_Local = true;
			M_VMejora1++;
			//		printf("Aquí 0 %d", M_Iter);
			val = MejoraIntercambiosMejorT();
			//		val = MejoraQuitar(val_inicial);

			if (val < (val_inicial - __FLT_EPSILON__) && M_VND == true)
			{
//				if (!silent) printf("%.5f 2 %d\n", val, M_Iter);
				M_Mejora1++;
				mejora = true;
				CalcularSumasT(M_BestT_Suma);
				if (val < (M_Best_fo * (1.001) + __FLT_EPSILON__))
					Best_solution = true;
				continue;


			}
			//		printf("Aquí 1 %d", M_Iter);
			//		M_Busqueda_Local = false;
			ftime(&t1);
			time_vnd = ((double)((t1.time - M_Time_Initial.time) * 1000 + t1.millitm - M_Time_Initial.millitm)) / 1000;
//			if (!silent)
//				printf("HiloVND2 %d %.3f\n", M_Hilo, time_vnd);
			if (time_vnd > M_Total_Time)
				break;

			if (M_VMejora2 < 10 || val_inicial < (1.001 * M_Best_fo) || (M_VMejora2 * get_random(0, 100) < (M_Mejora2 + 1) * 200))
			{
				M_VMejora2++;
				val = MejoraIntercambiosDosI(val_inicial);

				if (val < (val_inicial - __FLT_EPSILON__) && M_VND == true)
				{
//					if (!silent) printf("%.5f 3 %d\n", val, M_Iter);
					M_Mejora2++;
					mejora = true;
					CalcularSumasT(M_BestT_Suma);
					if (val < (M_Best_fo * (1.001) + __FLT_EPSILON__))
						Best_solution = true;
					continue;
				}
			}
			ftime(&t1);
			time_vnd = ((double)((t1.time - M_Time_Initial.time) * 1000 + t1.millitm - M_Time_Initial.millitm)) / 1000;
//			if (!silent)
//				printf("HiloVND4 %d %.3f\n", M_Hilo, time_vnd);
			if (time_vnd > M_Total_Time)
				break;
			//			printf("Aquí 3 %d", M_Iter);
			if (M_VMejora4 < 10 || val_inicial < (1.001 * M_Best_fo) || (M_VMejora4 * get_random(0, 100) < (M_Mejora4 + 1) * 300))
			{
				M_VMejora4++;
				val = MejoraQuitar(val_inicial);
				if (val < (val_inicial - __FLT_EPSILON__) && M_VND == true)
				{
//					if (!silent) printf("%.5f 5 %d\n", val, M_Iter);
					M_Mejora4++;
					CalcularSumasT(M_BestT_Suma);
					mejora = true;
					if (val < (M_Best_fo * (1.001) + __FLT_EPSILON__))
						Best_solution = true;
					continue;
				}
			}
			ftime(&t1);
			time_vnd = ((double)((t1.time - M_Time_Initial.time) * 1000 + t1.millitm - M_Time_Initial.millitm)) / 1000;
//			if (!silent)
//				printf("HiloVND3 %d %.3f\n", M_Hilo, time_vnd);
			if (time_vnd > M_Total_Time)
				break;
			//			printf("Aquí 2 %d", M_Iter);
			if (M_VMejora3 < 10 || val_inicial < (1.001 * M_Best_fo) || (M_VMejora3 * get_random(0, 100) < (M_Mejora3 + 1) * 400))
			{
				M_VMejora3++;
				if (M_Pool == false)
					val = MejoraIntercambiosEjection(0);
				else
					val = MejoraIntercambiosEjection(get_random(0, 1));

				//			val = MejoraQuitar(val_inicial);
					//		val = MejoraIntercambiosMejorT();
				if (val < (val_inicial - __FLT_EPSILON__) && M_VND == true)
				{
//					if (!silent) printf("%.5f 4 %d\n", val, M_Iter);
					M_Mejora3++;
					mejora = true;
					CalcularSumasT(M_BestT_Suma);
					if (val < (M_Best_fo * (1.001) + __FLT_EPSILON__))
						Best_solution = true;
					continue;
				}
			}


			
			ftime(&t1);
			time_vnd = ((double)((t1.time - M_Time_Initial.time) * 1000 + t1.millitm - M_Time_Initial.millitm)) / 1000;
//			if (!silent)
//				printf("HiloVND5 %d %.3f\n", M_Hilo, time_vnd);
			if (time_vnd > M_Total_Time)
				break;
			//			printf("Aquí 3 %d", M_Iter);
/*			if (M_VMejora5 < 10 || val_inicial < (1.001* M_Best_fo ) || (M_VMejora5 * get_random(0, 100) < (M_Mejora5 + 1) * 500))
			{
				M_VMejora5++;
				val = MejoraQuitarT(val_inicial);
				if (val < (val_inicial - __FLT_EPSILON__) && M_VND == true)
				{
					if (!silent) printf("%.5f 5 %d\n", val, M_Iter);
					M_Mejora5++;
					mejora = true;
					if (val < (M_Best_fo * (1.001) + __FLT_EPSILON__))
						Best_solution = true;
					continue;
				}
			}
			*/
			//			printf("Aquí 4 %d", M_Iter);
			/*			if (M_VMejora5 < 10 || val_inicial < (M_Best_fo + 0.5) || (M_VMejora5 * get_random(0, 100) < (M_Mejora5 + 1) * 500))
						{
							M_VMejora5++;
							val = MejoraQuitarCplex(val);
							if (val < (val_inicial - __FLT_EPSILON__) && M_VND == true)
							{
								if (!silent) printf("%.5f 6 %d\n", val, M_Iter);
								M_Mejora5++;
								mejora = true;
								continue;
							}
						}*/
						//			printf("Aquí 5 %d", M_Iter);

			ftime(&t1);

			time_vnd = ((double)((t1.time - M_Time_Initial.time) * 1000 + t1.millitm - M_Time_Initial.millitm)) / 1000;

//			if (!silent)
//				printf("HiloVND5 %d %.3f\n", M_Hilo, time_vnd);


		} while (mejora == true && M_VND == true && time_vnd < (M_Total_Time));
		
		if (M_Iter<1000) Best_solution = false;
		if (Best_solution)
		{
			Best_solution = false;
			val = MejoraQuitar(val_inicial);
			if (val < (val_inicial - __FLT_EPSILON__) && M_VND == true)
			{
				if (!silent) printf("%.5f 5 %d\n", val, M_Iter);
//				M_Mejora4++;
//				mejora = true;
				if (val < (M_Best_fo * (1.001) + __FLT_EPSILON__))
					Best_solution = true;
			}
		}
	}while (Best_solution==true && M_VND == true && time_vnd < (M_Total_Time));

	PoolGoodSolutions(M_I_T, val);
	if (M_Cuanto_Mejora.size() < 200)
		M_Cuanto_Mejora.push_back(val_ini2 - val);
	if (!M_Challenge_Mode) printf("Hilo %d Iter %d ValFin %.4f\n", M_Hilo, M_Iter, val);
	return val;
	//	} while (mejora == true && M_VND == true);
}
double Maintenance::VNS(double val)
{
	int cuenta = 0;
	struct timeb t1;
	double time_vnd = 0;
	double val3 = VND(val);
	do
	{
		cuenta++;

		double val2 = DestruirCplex(val3);
		if (get_random(0, 1) == 1)

			val3 = VND(val2);
		else
			val3 = VND2(val2);
		val=val3;
		ftime(&t1);

		time_vnd = ((double)((t1.time - M_Time_Initial.time) * 1000 + t1.millitm - M_Time_Initial.millitm)) / 1000;

		//			if (!silent)
		//				printf("HiloVND5 %d %.3f\n", M_Hilo, time_vnd);
		if (! M_Challenge_Mode)
							printf("HiloVND %d %d %.3f %.3f\n", M_Hilo, cuenta, val, time_vnd);

	} while (cuenta < 5 && time_vnd < (M_Total_Time));
	return val;
}
double Maintenance::VND2(double& val)
{
	//	printf("\n %d", M_Iter);
	if (!M_Challenge_Mode) printf("Hilo %d Iter %d ValIni %.4f\n", M_Hilo, M_Iter, val);
	M_Busqueda_Local = true;
	bool mejora = false;
	double val_inicial = val;
	double val_ini2 = val;
	double time_vnd = 0;
	bool silent = M_Challenge_Mode;
	bool Best_solution = false;
	M_Min_Improve = __FLT_EPSILON__;
	for (register int i = 0; i < M_T; i++)
	{
		M_T_M1_Suma[i] = 0;
		M_T_M2_Suma[i] = 0;
		M_T_M3_Suma[i] = 0;
	}
	//	M_Min_Improve = ((double)get_random(0, 100) / (double)10000);//.001;
	do
	{
		Best_solution = false;
		do
		{
			PoolGoodSolutions(M_I_T, val);
			int ndistintos = M_distintos.size();
			M_distintos.insert(hash_operator(M_I_T));
			if (ndistintos == M_distintos.size())
				break;
			mejora = false;
			val_inicial = val;
			struct timeb t1;
			ftime(&t1);
			random_shuffle(M_Vec_Alea_I.begin(), M_Vec_Alea_I.end());
			time_vnd = ((double)((t1.time - M_Time_Initial.time) * 1000 + t1.millitm - M_Time_Initial.millitm)) / 1000;

			//		if (!M_Challenge_Mode)
			//			printf("HiloVND1 %d %.3f\n", M_Hilo, time_vnd);
			if (time_vnd > M_Total_Time)
				break;
			//		if (val_inicial < (M_Best_fo + 1))
			//			M_Min_Improve = 0.0001;
			//		M_Busqueda_Local = true;
			M_VMejora1++;
			//		printf("Aquí 0 %d", M_Iter);
			val = MejoraQuitar(val_inicial);
			//		val = MejoraQuitar(val_inicial);

			if (val < (val_inicial - __FLT_EPSILON__) && M_VND == true)
			{
				//				if (!silent) printf("%.5f 2 %d\n", val, M_Iter);
				M_Mejora1++;
				mejora = true;
				CalcularSumasT(M_BestT_Suma);
				if (val < (M_Best_fo * (1.001) + __FLT_EPSILON__))
					Best_solution = true;
				continue;


			}
			//		printf("Aquí 1 %d", M_Iter);
					M_Busqueda_Local = false;
			ftime(&t1);
			time_vnd = ((double)((t1.time - M_Time_Initial.time) * 1000 + t1.millitm - M_Time_Initial.millitm)) / 1000;
			//			if (!silent)
			//				printf("HiloVND2 %d %.3f\n", M_Hilo, time_vnd);
			if (time_vnd > M_Total_Time)
				break;

			if (M_VMejora2 < 10 || val_inicial < (1.001 * M_Best_fo) || (M_VMejora2 * get_random(0, 100) < (M_Mejora2 + 1) * 200))
			{
				M_VMejora2++;
				val = MejoraIntercambiosDosI(val_inicial);

				if (val < (val_inicial - __FLT_EPSILON__) && M_VND == true)
				{
					//					if (!silent) printf("%.5f 3 %d\n", val, M_Iter);
					M_Mejora2++;
					mejora = true;
					CalcularSumasT(M_BestT_Suma);
					if (val < (M_Best_fo * (1.001) + __FLT_EPSILON__))
						Best_solution = true;
					continue;
				}
			}
			ftime(&t1);
			time_vnd = ((double)((t1.time - M_Time_Initial.time) * 1000 + t1.millitm - M_Time_Initial.millitm)) / 1000;
			//			if (!silent)
			//				printf("HiloVND4 %d %.3f\n", M_Hilo, time_vnd);
			if (time_vnd > M_Total_Time)
				break;
			//			printf("Aquí 3 %d", M_Iter);
			if (M_VMejora4 < 10 || val_inicial < (1.001 * M_Best_fo) || (M_VMejora4 * get_random(0, 100) < (M_Mejora4 + 1) * 300))
			{
				M_VMejora4++;
				
				val = MejoraIntercambiosMejorT();
				if (val < (val_inicial - __FLT_EPSILON__) && M_VND == true)
				{
					//					if (!silent) printf("%.5f 5 %d\n", val, M_Iter);
					M_Mejora4++;
					mejora = true;
					CalcularSumasT(M_BestT_Suma);
					if (val < (M_Best_fo * (1.001) + __FLT_EPSILON__))
						Best_solution = true;
					continue;
				}
			}
			ftime(&t1);
			time_vnd = ((double)((t1.time - M_Time_Initial.time) * 1000 + t1.millitm - M_Time_Initial.millitm)) / 1000;
			//			if (!silent)
			//				printf("HiloVND3 %d %.3f\n", M_Hilo, time_vnd);
			if (time_vnd > M_Total_Time)
				break;
			//			printf("Aquí 2 %d", M_Iter);
			if (M_VMejora3 < 10 || val_inicial < (1.001 * M_Best_fo) || (M_VMejora3 * get_random(0, 100) < (M_Mejora3 + 1) * 400))
			{
				M_VMejora3++;
				if (M_Pool == false)
					val = MejoraIntercambiosEjection(0);
				else
					val = MejoraIntercambiosEjection(get_random(0, 1));

				//			val = MejoraQuitar(val_inicial);
					//		val = MejoraIntercambiosMejorT();
				if (val < (val_inicial - __FLT_EPSILON__) && M_VND == true)
				{
					//					if (!silent) printf("%.5f 4 %d\n", val, M_Iter);
					M_Mejora3++;
					mejora = true;
					CalcularSumasT(M_BestT_Suma);
					if (val < (M_Best_fo * (1.001) + __FLT_EPSILON__))
						Best_solution = true;
					continue;
				}
			}



			ftime(&t1);
			time_vnd = ((double)((t1.time - M_Time_Initial.time) * 1000 + t1.millitm - M_Time_Initial.millitm)) / 1000;
			//			if (!silent)
			//				printf("HiloVND5 %d %.3f\n", M_Hilo, time_vnd);
			if (time_vnd > M_Total_Time)
				break;
			//			printf("Aquí 3 %d", M_Iter);
/*			if (M_VMejora5 < 10 || val_inicial < (1.001* M_Best_fo ) || (M_VMejora5 * get_random(0, 100) < (M_Mejora5 + 1) * 500))
			{
				M_VMejora5++;
				val = MejoraQuitarT(val_inicial);
				if (val < (val_inicial - __FLT_EPSILON__) && M_VND == true)
				{
					if (!silent) printf("%.5f 5 %d\n", val, M_Iter);
					M_Mejora5++;
					mejora = true;
					if (val < (M_Best_fo * (1.001) + __FLT_EPSILON__))
						Best_solution = true;
					continue;
				}
			}
			*/
			//			printf("Aquí 4 %d", M_Iter);
			/*			if (M_VMejora5 < 10 || val_inicial < (M_Best_fo + 0.5) || (M_VMejora5 * get_random(0, 100) < (M_Mejora5 + 1) * 500))
						{
							M_VMejora5++;
							val = MejoraQuitarCplex(val);
							if (val < (val_inicial - __FLT_EPSILON__) && M_VND == true)
							{
								if (!silent) printf("%.5f 6 %d\n", val, M_Iter);
								M_Mejora5++;
								mejora = true;
								continue;
							}
						}*/
						//			printf("Aquí 5 %d", M_Iter);

			ftime(&t1);

			time_vnd = ((double)((t1.time - M_Time_Initial.time) * 1000 + t1.millitm - M_Time_Initial.millitm)) / 1000;

			//			if (!silent)
			//				printf("HiloVND5 %d %.3f\n", M_Hilo, time_vnd);


		} while (mejora == true && M_VND == true && time_vnd < (M_Total_Time));

		if (M_Iter < 1000) Best_solution = false;
		if (Best_solution)
		{
			Best_solution = false;
			val = MejoraQuitar(val_inicial);
			if (val < (val_inicial - __FLT_EPSILON__) && M_VND == true)
			{
				if (!silent) printf("%.5f 5 %d\n", val, M_Iter);
				//				M_Mejora4++;
				//				mejora = true;
				if (val < (M_Best_fo * (1.001) + __FLT_EPSILON__))
					Best_solution = true;
			}
		}
	} while (Best_solution == true && M_VND == true && time_vnd < (M_Total_Time));
	if (M_Cuanto_Mejora.size() < 200)
		M_Cuanto_Mejora.push_back(val_ini2 - val);
	PoolGoodSolutions(M_I_T, val);
	if (!M_Challenge_Mode) printf("Hilo %d Iter %d ValFin %.4f\n", M_Hilo, M_Iter, val);
	return val;
	//	} while (mejora == true && M_VND == true);
}
void Maintenance::GRASP()
{
	
	
	vector<double> Valores;

	srand(M_seed);
	M_Silent = true;
	int veces_mejora = 0;

	M_Random_Constructive = true;
	M_Iterations = 100000;
//	bool primero = false;
	double val = 0;
	double tmp_first = 0;
	int no_mejora = 0;
	int no_sol = 0;
	bool Mediana = false;
	M_Busqueda_Local = true;
	for (int i = 0; i < 15; i++)
	{
		M_Veces_Tipo.push_back(0);
		M_Sin_Terminar_Tipo.push_back(0);
		M_FO_Tipo.push_back(0);
	}
	struct timeb t2;
	ftime(&t2);


	tmp_first = ((double)((t2.time - M_Time_Initial.time) * 1000 + t2.millitm - M_Time_Initial.millitm)) / 1000;

	struct timeb t0;
	ftime(&t0);
	for (; M_Iter < M_Iterations && tmp_first<M_Total_Time; M_Iter++)
	{
		M_Porcentaje_Ocupado = (double) get_random(40, 65)/(double)100;
		M_Porcentaje_Ocupado2 = (double)get_random(70, 90) / (double)100;
//		M_Porcentaje_Ocupado = 100;
//		M_Porcentaje_Ocupado2 = 100;
//		M_Porcentaje_Ocupado = 1;
		M_Delta = get_random(70, 95);
//		if (M_Iter == 111)
//			int kk = 9;
//		M_Tipo = get_random(10, 14);
		if (get_random(0, 1) == 0) M_Tipo = 10;
		else M_Tipo = 13;
//		if (get_rano)
//		if (M_Iter > 6000) M_Tipo = 9;
		//cout << endl<< "Iteración " << M_Iter << endl;
		M_Determinista = false;
		bool Sol_finished = false;
//		if (M_Tipo<=9)
//			Sol_finished = ConstructiveR(M_Tipo);
//			Sol_finished = ConstructiveRBias(M_Tipo);
//		else
			 Sol_finished = ConstructiveRA(M_Tipo);




 		M_Veces_Tipo[M_Tipo]++;

		M_Determinista = true;
		/*
		cout << endl;
		for (int i = 0; i < M_N_Interventions; i++)
		{
			cout << M_Interventions[i].Get_T_Temp() << " ";
		}
		cout << endl;
		*/

//		if (no_sol>10 && Sol_finished == false && M_Best_I_T.size()==0)
		if ((no_sol > 3 || M_Best_I_T.size()==0) && Sol_finished == false )
		{
				no_sol = 0;
			M_Sin_Terminar_Tipo[M_Tipo]++;
			if (M_Silent != true) cout << "Sol no terminado " << M_Iter << endl;
			val = 0;
			int reparada=RepararCplex(0);

//			if (M_Best_I_T.size() != 0)
//			{}
			if (reparada > 0)
			{
				Sol_finished = true;
				val = FuncionObjetivoTemp();
				ActualizarMejorSolution(val, 110);
				val = MejoraQuitar(val);
			}
		}


		if (Sol_finished == false)
		{
			no_sol++;
			M_Sin_Terminar_Tipo[M_Tipo]++;
			if (M_Silent != true) cout << "Sol no terminado " << M_Iter << endl;
			val = 0;
			M_No_Sol++;
//			RepararCplex(0);
		}
		else
		{

			val = FuncionObjetivoTemp();
			ActualizarMejorSolution(val, 110);
			//		double cuantil1= M_T_Temp_Risk_S[i][pos];
		//		 sort(M_T_Temp_Risk_S[i].begin(), M_T_Temp_Risk_S[i].end());

/*			if (primero == false)
			{
				M_FC_Umbral = M_Best_fo + 1;
				primero = true;
			}
			M_FO_Tipo[M_Tipo]+=val;
		int kk = 0;*/
			M_FC_Umbral = __FLT_MAX__;
			if (val > 0 && (((val-M_Mediana_Mejora)<M_Best_fo) || M_Best_I_T.size() == 0))
			{

//				printf("sale %", M_Iter);
				//Filtro calidad

/*				if (val < (M_Best_fo * 1.01))
				{
//					RepararSolutionCopy();


					RepararSolutionCopyA(val);
					MejoraIntercambiosFo2();
//					BusquedaLocalIntercambio();
				}*/



	//				if (M_Iter < 100)
		//				continue;

					

				/*
				if (M_Iter > 0 && M_Iter % 50 == 0)
				{
					pos1 = ceil((double)Valores.size() * 0.1) - 1;
					pos2 = ceil((double)Valores.size() * 0.05) - 1;
					nth_element(Valores.begin(), Valores.begin() + pos1, Valores.end());
					Umbral_M1 = Valores[pos1];
					Umbral_M2 = Valores[pos2];
				}
				*/
					no_mejora = 0;
					M_fo_first = val;
//					val = RepararSolutionCopyA(val);


					Valores.push_back(val);

//					if (val<Umbral_M1)


					switch (M_Tipo_Busqueda)
					{
					case 1:
					{
						if (get_random(0, 3) >= 0)
							VND(val);
						else
							VND2(val);
					}
						
					case 2:
						val=MejoraQuitar(val);
						break;
					case 3:
						val = MejoraIntercambiosMejorT();
						break;
					case 4:
						val = MejoraIntercambiosDosI(val);
						break;
					case 5:
						if (M_Pool == false)
							val = MejoraIntercambiosEjection(0);
						else
							val = MejoraIntercambiosEjection(get_random(0, 1));
						break;
					case 6:
						val = MejoraQuitarCplex(val);
						break;
					default:
					{
//						VNS(val);
						if (get_random(0, 3) >= 1)
							VND(val);
						else
							VND2(val);
					}
					}


					veces_mejora++;
					/*
					int cont = 0;
					do
					{
						MejoraIntercambiosFo1();
						PerturbarSolution();
						cont++;
					} while (cont < 10);
					*/
//					if (get_random(0, 1) >= 0 || M_Pool == false)
//					{
						//				MejoraIntercambiosFo1();
//					MejoraIntercambiosFo1Order();


//						MejoraIntercambiosFo1Dos();
						
//					}
//			MejoraIntercambiosFo2();
//					MejoraIntercambiosFo1OrderFast();
//					else
//					MejoraIntercambiosFo1Best();
//					
//					MejoraIntercambiosFo1Dos();
				
				M_FC_Umbral = val;
				}
				else
					no_mejora++;
/*				if (no_mejora > 4)
				{
					no_mejora = 0;
					M_FC_Umbral = M_FC_Umbral * 1.02;
					if (!M_Challenge_Mode) cout << M_FC_Umbral << endl;
				}
	*/				
			}
			

		if (M_Iter % 100000 == 0 )
		{

			if (!M_Challenge_Mode) cout << endl << "Iter " << M_Iter << " FO " << M_Best_fo << endl;
				if (!M_Challenge_Mode) cout << endl << "Iter " << M_Iter << " FO " << M_fo_temp << endl;
				if (val > 0 && M_Iter>0)
				{
					EjecutarValidadorPython();
				}
		}
		if (Mediana==false && M_Cuanto_Mejora.size() == 200)
		{
			Mediana = true;
//			pos = ceil((double)M_Scenarios_number[i] * M_Quantile) - 1;
//			pos = 100;
//			double media = suma / M_Scenarios_number[i];
			nth_element(M_Cuanto_Mejora.begin(), M_Cuanto_Mejora.begin() + (200/2), M_Cuanto_Mejora.end());
			M_Mediana_Mejora = M_Cuanto_Mejora[200 / 2];
			if (M_Challenge_Mode) printf("M_Mediana_Mejora %d Hilo %d\n", M_Mediana_Mejora,M_Hilo);

		}
		
		ftime(&t0);

		tmp_first = ((double)((t0.time - M_Time_Initial.time) * 1000 + t0.millitm - M_Time_Initial.millitm)) / 1000;

		

	}
			int imposibles = 0;
	if (!M_Challenge_Mode)
	{
		cout << endl;
		cout << endl << "Hilo" << M_Hilo<< "Veces Mejora " << veces_mejora << " Distintos " << M_distintos.size() << " Nosol " <<  M_No_Sol << endl;
		cout << endl;
		for (int i = 10; i < 15; i++)
		{
			cout << i << "";
		}
		cout << endl;
		for (int i = 10; i < 15; i++)
		{
			cout << M_Veces_Tipo[i] << " ";

		}
		cout << endl;

		for (int i = 10; i < 15; i++)
		{
			cout << M_Sin_Terminar_Tipo[i] << " ";
			imposibles += M_Sin_Terminar_Tipo[i];
		}
		cout << endl;
		for (int i = 10; i < 15; i++)
		{
			if ((M_Veces_Tipo[i] - M_Sin_Terminar_Tipo[i]) > 0)
				cout << M_FO_Tipo[i] / (M_Veces_Tipo[i] - M_Sin_Terminar_Tipo[i]) << " ";
			else
				cout << 0 << " ";

		}
		cout << endl;
	}
	if (M_Challenge_Mode!=true && M_NThreads < 2)
	{
		ofstream file;
		string name2 = "Resultados.txt";
		file.open(name2, ios::app);
		cout << " ";
		for (int i = 10; i < 15; i++)
		{
			file << M_Veces_Tipo[i] << " ";

		}


		for (int i = 10; i < 15; i++)
		{
			file << M_Sin_Terminar_Tipo[i] << " ";

		}

		for (int i = 10; i < 15; i++)
		{
			if ((M_Veces_Tipo[i] - M_Sin_Terminar_Tipo[i]) > 0)
				file << M_FO_Tipo[i] / (M_Veces_Tipo[i] - M_Sin_Terminar_Tipo[i]) << " ";
			else
				file << 0 << " ";

		}
		file << " Mejoras " << M_Mejora1 << " " << M_Mejora2 << " " << M_Mejora3 << " " << M_Mejora4 << " " << M_Mejora5 << " " << M_VMejora1 << " " << M_VMejora2 << " " << M_VMejora3 << " " << M_VMejora4 << " " << M_VMejora5 << " Sin Terminar " << imposibles << " Veces Mejora " << veces_mejora << " Distintos Alea" << M_distintos.size() << " Distintos M " << M_distintosM.size();


		file.close();
	}
}
void Maintenance::EjecutarValidadorPython(void)

{
	if (M_Challenge_Mode)
		return;
//	return;
	string kk = "kk.txt";
	PrintSolution(kk, M_I_T);
	//			EjecutarComprobador();
	string system_ins = "RTE_ChallengeROADEF2020_checker.py " + M_Name_file + ".json kk.txt";
	cout << endl << system_ins << endl;
	//	int result = system("RTE_ChallengeROADEF2020_checker.py A_set/example1.json solution1.txt");
	 system(system_ins.c_str());
}

void Maintenance::EjecutarValidadorPython(vector<int> &lista)
{
	//	return;
	if (M_Challenge_Mode)
		return;
	string kk = "kk.txt";
	PrintSolution(kk, lista);
	//			EjecutarComprobador();
	string system_ins = "RTE_ChallengeROADEF2020_checker.py " + M_Name_file + ".json kk.txt";
	cout << endl << system_ins << endl;
	//	int result = system("RTE_ChallengeROADEF2020_checker.py A_set/example1.json solution1.txt");
	system(system_ins.c_str());
}

//A partir de una solución la intento rellenar con vari
double Maintenance::RepararSolutionCopy()
{
	//Si copia está vacía volver
	if (M_I_T_Solution_Copy.size() == 0) return 0;
	M_T_Ordenado = false;
	double best_value=__LONG_MAX__;
	bool siguiente_tipo = false;
	for (int type = 0; type < (2*10)-1; type++)
	{
		CopyBack();
		M_Tipo = type % 10;
		M_Tipo = AleatorizarIgualesListInterventions(M_Tipo);

//		CopyListInterventionsTemp();
		siguiente_tipo = false;
		for (int i = 0; i < M_Interventions.size() && siguiente_tipo != true; i++)
		{
			int ielegido = i;
			if (type > 9) 
				ielegido = M_N_Interventions - i - 1;
			int choose = IndiceTipo(M_Tipo, ielegido);
			if (M_Interventions_Colocada[choose] == true) continue;
			bool colocado = ColocarEnTiempoRGreedy(choose);
			if (colocado == true)
			{
				M_Interventions_Colocada[choose] = true;
			}
			else
			{
				if (!M_Silent) cout << "No puede colocar  la intervention " << choose << endl << endl;
				if (M_Tipo!=9) M_Interventions_Out[choose]+=M_T;
				siguiente_tipo = true;
			}
			if (M_NI_Colocadas == M_N_Interventions)
			{
				double val2 = FuncionObjetivo();
				if (val2 < best_value)
					best_value = val2;
				bool mejorado=ActualizarMejorSolution(val2,1);
//				if (mejorado)
//					int kk = 9;
				siguiente_tipo = true;
			}
		}

	}
	return best_value;

}

//A partir de una solución la intento rellenar con vari
double Maintenance::RellenarSolution(int type)
{
	//Si copia está vacía volver
	if (M_I_T_Solution_Copy.size() == 0) return 0;
	M_T_Ordenado = true;
	double best_value = __LONG_MAX__;
	bool siguiente_tipo = false;

	//	CopyBack();
		M_Tipo = type % 10;
		M_Tipo = AleatorizarIgualesListInterventions(M_Tipo);
		bool inverso = false;
		if (get_random(0, 1) == 0)
			inverso = true;
		//		CopyListInterventionsTemp();
		siguiente_tipo = false;
		for (int i = 0; i < M_Interventions.size() && siguiente_tipo != true; i++)
		{
			int ielegido = i;
 
			if (inverso==true)
				ielegido = M_N_Interventions - i - 1;
			int choose = IndiceTipo(M_Tipo, ielegido);
			if (M_Interventions_Colocada[choose] == true) continue;
			bool colocado = ColocarEnTiempoRGreedy(choose);
			if (colocado == true)
			{
				M_Interventions_Colocada[choose] = true;
			}
			else
			{
				if (!M_Silent) cout << "No puede colocar  la intervention " << choose << endl << endl;
				if (M_Tipo != 9) M_Interventions_Out[choose] += M_T;
				siguiente_tipo = true;
			}
			if (M_NI_Colocadas == M_N_Interventions)
			{
				double val2 = FuncionObjetivo();
				if (val2 < best_value)
					best_value = val2;
				bool mejorado = ActualizarMejorSolution(val2,1);
				return val2;
//				if (mejorado)
//					int kk = 9;
				siguiente_tipo = true;
			}
		}

	return best_value;

}
//A partir de una solución la intento rellenar con vari
double Maintenance::RepararSolutionCopyA(double val_ini)
{

	//Si copia está vacía volver
	if (M_I_T_Copy.size() == 0) return 0;

	M_T_Ordenado = true;
	double best_value = val_ini;
	bool siguiente_tipo = false;
	for (int copy = 0; copy < 2; copy++)
	{
		for (int type = 10; type < 15; type++)
		{
			if (copy == 0) CopyBack();
			else CopyBack2();
			/*
				for (list<pair<int, int>> ::iterator it = M_I_T_Solution.begin(); it != M_I_T_Solution.end(); it++)
				{
					ModificarRiskTemp(M_Interventions[(*it).first].Get_T_Temp(), M_Interventions[(*it).first].Get_T_F_Temp());
					//Poner los valores de los scenarios
					for (int t = M_Interventions[(*it).first].Get_T_Temp(); t < M_Interventions[(*it).first].Get_T_F_Temp(); t++)
					{
						for (int si = 0; si < M_Scenarios_number[t]; si++)
						{
							M_T_Temp_Risk_S[t][si] += M_Interventions[(*it).first].Get_Risk()[t][M_Interventions[(*it).first].Get_T_Temp()][si];

						}
					}
				}

			*/
			//Añado la parte de la función objetivo temp
	//Incluimos los escenarios

			M_Tipo = type;

			//		CopyListInterventionsTemp();
			siguiente_tipo = false;
			int choose = -1;
			do
			{


				if (get_random(0, 3) >= 0)
					choose = ElegirMayor(M_Tipo);
				//choose = ElegirMayor(type);
				if (choose == (-1))
					continue;

				//		int choose = ElegirMayor(M_Tipo);
						//Coloca la intervention en la primera T disponible
			//			ColocarIEnTiempo(choose);
				int telegido = ElegirMayorTFuncionObjetivo(choose);
				ColocarIEnTiempo(choose, telegido);
				ModificarRiskTemp(M_Interventions[choose].Get_T_Temp(), M_Interventions[choose].Get_T_F_Temp());



				if (M_NI_Colocadas == M_N_Interventions)
				{
					double val2 = FuncionObjetivoTemp();
					if (val2 < best_value)
					{
						Copy3();
						best_value = val2;
					}
					bool mejorado = ActualizarMejorSolution(val2,1);
//					if (mejorado)
//						int kk = 9;
					siguiente_tipo = true;
				}
			} while (choose != -1 &&siguiente_tipo!=true);

		}


			
	}
	if (val_ini > (best_value + __FLT_EPSILON__))
	{
		CopyBack3();

	}

	return best_value;

}
//A partir de una solución la intento rellenar con vari
double Maintenance::TerminarSolutionPrimeraCopy(double val)
{

	//Si copia está vacía volver
	if (M_I_T_Copy.size() == 0) return 0;

	M_T_Ordenado = true;
	//	M_Tipo = type;
	double val_inicio = val;
	int choose = -1;
	bool siguiente_tipo = false;
	int telegido = -1;
	Copy2(false);
	VerifySolution();
	//	printf("Entra ");
	for (int type = 0; type < 10; type++)
	{
		M_Tipo = type % 5 + 10;
		siguiente_tipo = false;
		do
		{
			//			printf("Entra 0.5");
			//			if (get_random(0, 3) >= 0)
			choose = ElegirMayorAbsoluto(get_random(10, 14));
			//			printf("Entra 0.5");
			//			else
			//				choose = ElegirMenorAbsoluto(get_random(10, 14));

						//choose = ElegirMayor(type);
			if (choose == (-1))
				break;

			//		int choose = ElegirMayor(M_Tipo);
					//Coloca la intervention en la primera T disponible
		//			ColocarIEnTiempo(choose);
//			printf("Entra 2");
			telegido = ElegirMayorTFuncionObjetivoAbsoluto(choose);
			//			printf("Sale 2 ");
			if (telegido < 0)
				break;
			//			printf("Entra 3");
			ColocarIEnTiempo(choose, telegido);
			//			printf("Sale 3 ");
			if ((M_Alpha * (M_fo_temp / (double)M_T) + (1 - M_Alpha) * (M_fo2_temp / (double)M_T)) > (val_inicio - __FLT_EPSILON__))
			{
				telegido = -1;
				break;
			}
			//			printf("Entra 4 ");
			if (M_NI_Colocadas == M_N_Interventions)
			{
				siguiente_tipo = true;
				double val2 = FuncionObjetivoTemp();
							Copy2(false);
					val_inicio = val2;
					bool mejorado = ActualizarMejorSolution(val2, 2);
					return val_inicio;




			}
			//			printf("Sale 4 ");
		} while (choose != -1 && siguiente_tipo != true);
		if (choose == (-1) || telegido < 0)
		{
			//			printf("Entra 55");
			CopyBack2(false);
			//			printf("Sale 55");
		}
	}
	//	printf("Sale ");
	return val_inicio;





}

//A partir de una solución la intento rellenar con vari
double Maintenance::TerminarSolutionCopy(double val)
{

	//Si copia está vacía volver
	if (M_I_T_Copy.size() == 0) return 0;

	M_T_Ordenado = true;
//	M_Tipo = type;
	double val_inicio = val;
	int choose = -1;
	bool siguiente_tipo = false;
	int telegido = -1;
	Copy2(false);
	VerifySolution();
//	printf("Entra ");
	for (int type = 0; type < 10; type++)
	{
		M_Tipo = type%5+10;
		siguiente_tipo = false;
		do
		{
//			printf("Entra 0.5");
//			if (get_random(0, 3) >= 0)
				choose = ElegirMayorAbsoluto(get_random(10,14));
//			printf("Entra 0.5");
//			else
//				choose = ElegirMenorAbsoluto(get_random(10, 14));

			//choose = ElegirMayor(type);
			if (choose == (-1))
				break;

			//		int choose = ElegirMayor(M_Tipo);
					//Coloca la intervention en la primera T disponible
		//			ColocarIEnTiempo(choose);
//			printf("Entra 2");
			telegido = ElegirMayorTFuncionObjetivoAbsoluto(choose);
//			printf("Sale 2 ");
			if (telegido < 0)
				break;
//			printf("Entra 3");
			ColocarIEnTiempo(choose, telegido);
//			printf("Sale 3 ");
			if ((M_Alpha * (M_fo_temp / (double)M_T) + (1 - M_Alpha) * (M_fo2_temp / (double)M_T)) > (val_inicio - __FLT_EPSILON__))
			{
				telegido = -1;
				break;
			}
//			printf("Entra 4 ");
			if (M_NI_Colocadas == M_N_Interventions)
			{
				siguiente_tipo = true;
				double val2 = FuncionObjetivoTemp();
				if (val2 < (val_inicio - __FLT_EPSILON__-M_Min_Improve)  )
				{
//					return val2;
					if (val2 < (val_inicio - __FLT_EPSILON__))
						Copy2(false);
					val_inicio = val2;
					bool mejorado = ActualizarMejorSolution(val2, 2);
				}
				else
					CopyBack2(false);




			}
//			printf("Sale 4 ");
		} while (choose != -1 && siguiente_tipo!= true);
		if (choose == (-1) || telegido < 0)
		{
//			printf("Entra 55");
			CopyBack2(false);
//			printf("Sale 55");
		}
	}
//	printf("Sale ");
	return val_inicio;



	

}

double Maintenance::RellenarSolutionA(int type)
{

	//Si copia está vacía volver
	if (M_I_T_Copy.size() == 0) return 0;
	int Tipo_anterior = M_Tipo;
	M_T_Ordenado = true;
	double best_value = __LONG_MAX__;
	bool siguiente_tipo = false;

	if (Tipo_anterior <= 11)
		{
			for (list<pair<int, int>> ::iterator it = M_I_T_Solution.begin(); it != M_I_T_Solution.end(); it++)
			{
				ModificarRiskTemp(M_Interventions[(*it).first].Get_T_Temp(), M_Interventions[(*it).first].Get_T_F_Temp());
			}
		}
		M_Tipo = type;

		//		CopyListInterventionsTemp();
		siguiente_tipo = false;
		for (int i = 0; i < M_N_Interventions && siguiente_tipo != true; i++)
			
		{

			int ielegido = i;

			int choose = IndiceTipo(M_Tipo, ielegido);
			if (M_Interventions_Colocada[choose] == true) continue;
			if (M_Interventions[choose].Get_T_Ordenados_Risk_Temp().size() == 0) continue;

			//Coloca la intervention en la primera T disponible
			ColocarIEnTiempoValor(choose);
			ModificarRiskTemp(M_Interventions[choose].Get_T_Temp(), M_Interventions[choose].Get_T_F_Temp());



			if (M_NI_Colocadas == M_N_Interventions)
			{
				double val2 = FuncionObjetivo();
				if (val2 < best_value)
					best_value = val2;
				bool mejorado = ActualizarMejorSolution(val2,1);
//				if (mejorado)
//					int kk = 9;
				siguiente_tipo = true;
			}
		}

	return best_value;

}
double Maintenance::RellenarSolutionAv2(int type)
{

	//Si copia está vacía volver
	if (M_I_T_Solution_Copy.size() == 0) return 0;
	int Tipo_anterior = M_Tipo;
	M_T_Ordenado = true;
	double best_value = __LONG_MAX__;
	bool siguiente_tipo = false;

	if (Tipo_anterior <= 11)
	{
		for (list<pair<int, int>> ::iterator it = M_I_T_Solution.begin(); it != M_I_T_Solution.end(); it++)
		{
			ModificarRiskTemp(M_Interventions[(*it).first].Get_T_Temp(), M_Interventions[(*it).first].Get_T_F_Temp());
		}
	}
	M_Tipo = type;

	//		CopyListInterventionsTemp();
	siguiente_tipo = false;
	int choose = (-1);
	do
	{


		if (get_random(0,3)>=1)
		choose = ElegirMayor(get_random(10,14));
		else
			choose = ElegirMenor(get_random(10, 14));
		if (choose == (-1))
			continue;


		//Coloca la intervention en la primera T disponible
		ColocarIEnTiempoValor(choose);
		ModificarRiskTemp(M_Interventions[choose].Get_T_Temp(), M_Interventions[choose].Get_T_F_Temp());



		if (M_NI_Colocadas == M_N_Interventions)
		{
			double val2 = FuncionObjetivo();

			return val2;
		}
	} while (choose != (-1));

	return best_value;

}

int Maintenance::ElegirMayor(int type)
{
	double Max = -1.0;
	int elegido = -1;
	for (int k = 0; k < M_N_Interventions; k++)

	{
		int i = M_Vec_Alea_I[k];
		if (M_Interventions_Colocada[i] == true) 
			continue;
		if (M_Interventions[i].Get_T_Ordenados_Risk_Temp().size() == 0) 
			continue;
		if (type == 10)
		{
			if (M_Interventions[i].Get_Min_Risk_Temp() > Max)
			{
				Max = M_Interventions[i].Get_Min_Risk_Temp();
				elegido = i;
			}
		}
		if (type == 11)
		{
			if (M_Interventions[i].Get_Regret() > Max)
			{
				Max = M_Interventions[i].Get_Regret();
				elegido = i;
			}

		}

		if (type == 12)
		{
			if (M_Interventions[i].Get_WorkloadC_Temp() > Max)
			{
				Max = M_Interventions[i].Get_WorkloadC_Temp();
				elegido = i;
			}
		}
		if (type == 13)
		{
			if (M_Interventions[i].Get_Min_RiskQ_Temp() > Max)
			{
				Max = M_Interventions[i].Get_Min_RiskQ_Temp();
				elegido = i;
			}
		}
		if (type == 14)
		{
			if (M_Interventions[i].Get_RegretQ() > Max)
			{
				Max = M_Interventions[i].Get_RegretQ();
				elegido = i;
			}

		}

	}
	return elegido;

}
int Maintenance::ElegirMayorAbsoluto(int type)
{
	double Max = -1.0;
	int elegido = -1;
	for (int k = 0; k < M_N_Interventions; k++)
	{
		int i = M_Vec_Alea_I[k];

		if (M_I_T[i] >=0)
			continue;
		//Aleatoriedad al llenar
		if (elegido >= 0 && get_random(0, 2) <= 1)
			continue;
		if (type == 10)
		{
			if (M_Interventions[i].Get_Min_Risk() > Max)
			{
				Max = M_Interventions[i].Get_Min_Risk();
				elegido = i;
			}
		}
		if (type == 11)
		{
			if (M_Interventions[i].Get_Max_Dif_Risk()> Max)
			{
				Max = M_Interventions[i].Get_Max_Dif_Risk();
				elegido = i;
			}
		}
		if (type == 12)
		{
			if (M_Interventions[i].Get_Min_Workload() > Max)
			{
				Max = M_Interventions[i].Get_Min_Workload();
				elegido = i;
			}
		}
		if (type == 13)
		{
			if (M_Interventions[i].Get_Min_RiskQ() > Max)
			{
				Max = M_Interventions[i].Get_Min_RiskQ();
				elegido = i;
			}
		}
		if (type == 14)
		{
			if (M_Interventions[i].Get_Exclusions().size() > Max)
			{
				Max = M_Interventions[i].Get_Exclusions().size();
				elegido = i;
			}
		}
	}
	return elegido;

}
int Maintenance::ElegirMenorAbsoluto(int type)
{
	double Min= __DBL_MAX__;
	int elegido = -1;
	for (int k = 0; k < M_N_Interventions; k++)

	{
		int i = M_Vec_Alea_I[k];
		if (M_I_T[i] >= 0)
			continue;
		if (type == 10)
		{
			if (M_Interventions[i].Get_Min_Risk() < Min)
			{
				Min = M_Interventions[i].Get_Min_Risk();
				elegido = i;
			}
		}
		else
		{
			if (M_Interventions[i].Get_Min_RiskQ() < Min)
			{
				Min = M_Interventions[i].Get_Min_RiskQ();
				elegido = i;
			}
		}

	}
	return elegido;

}
int Maintenance::ElegirMenor(int type)
{
	double Min = __LONG_MAX__;
	int elegido = -1;
	for (int k = 0; k < M_N_Interventions; k++)

	{
		int i = M_Vec_Alea_I[k];
		if (M_Interventions_Colocada[i] == true)
			continue;
		if (M_Interventions[i].Get_T_Ordenados_Risk_Temp().size() == 0)
			continue;
		if (type == 10)
		{
			if (M_Interventions[i].Get_Min_Risk_Temp() < Min)
			{
				Min = M_Interventions[i].Get_Min_Risk_Temp();
				elegido = i;
			}
		}
		if (type == 11)
		{
			if (M_Interventions[i].Get_Regret() < Min)
			{
				Min = M_Interventions[i].Get_Regret();
				elegido = i;
			}

		}

		if (type == 12)
		{
			if (M_Interventions[i].Get_WorkloadC_Temp() < Min)
			{
				Min = M_Interventions[i].Get_WorkloadC_Temp();
				elegido = i;
			}
		}
		if (type == 13)
		{
			if (M_Interventions[i].Get_Min_RiskQ_Temp() < Min)
			{
				Min = M_Interventions[i].Get_Min_RiskQ_Temp();
				elegido = i;
			}
		}
		if (type == 14)
		{
			if (M_Interventions[i].Get_RegretQ() < Min)
			{
				Min = M_Interventions[i].Get_RegretQ();
				elegido = i;
			}

		}

	}
	return elegido;

}
int Maintenance::ElegirMenorAleatorizadoSPG(int type)
{
	double Min = __DBL_MAX__;
	int elegido = -1;
	for (int k = 0; k < M_N_Interventions; k++)

	{
		int i = M_Vec_Alea_I[k];
		if (M_Interventions_Colocada[i] == true)
			continue;
		if (M_Interventions[i].Get_T_Ordenados_Risk_Temp().size() == 0)
			continue;
		if (elegido != -1 && get_random(0, 100) < M_Delta)
			continue;

		if (type == 10)
		{
			if (M_Interventions[i].Get_Min_Risk_Temp() < (Min))
			{
				Min = M_Interventions[i].Get_Min_Risk_Temp();
				elegido = i;
			}
		}
		if (type == 11)
		{
			if (M_Interventions[i].Get_Regret() < (Min))
			{
				Min = M_Interventions[i].Get_Regret();
				elegido = i;
			}
		}
		if (type == 12)
		{
			if (M_Interventions[i].Get_WorkloadC_Temp() < (Min))
			{
				Min = M_Interventions[i].Get_WorkloadC_Temp();
				elegido = i;
			}
		}
		if (type == 13)
		{
			if (M_Interventions[i].Get_Min_RiskQ_Temp() < (Min))
			{
				Min = M_Interventions[i].Get_Min_RiskQ_Temp();
				elegido = i;
			}
		}
		if (type == 14)
		{
			if (M_Interventions[i].Get_RegretQ() < (Min))
			{
				Min = M_Interventions[i].Get_RegretQ();
				elegido = i;
			}
		}
	}
	return elegido;
}

int Maintenance::ElegirMayorAleatorizadoSPG(int type)
{
	double Max = -1;
	int elegido = -1;
	for (int k = 0; k < M_N_Interventions; k++)

	{
		int i = M_Vec_Alea_I[k];
		if (M_Interventions_Colocada[i] == true)
			continue;
		if (M_Interventions[i].Get_T_Ordenados_Risk_Temp().size() == 0)
			continue;
		if (elegido != -1 && get_random(0, 100) < M_Delta) 
			continue;

		if (type == 10)
		{
			if (M_Interventions[i].Get_Min_Risk_Temp() > (Max))
			{
				Max = M_Interventions[i].Get_Min_Risk_Temp();
				elegido = i;
			}
		}
		if (type == 11)
		{
			if (M_Interventions[i].Get_Regret() > (Max))
			{
				Max = M_Interventions[i].Get_Regret();
				elegido = i;
			}
		}
		if (type == 12)
		{
			if (M_Interventions[i].Get_WorkloadC_Temp() > (Max))
			{
				Max = M_Interventions[i].Get_WorkloadC_Temp();
				elegido = i;
			}
		}
		if (type == 13)
		{
			if (M_Interventions[i].Get_Min_RiskQ_Temp() > (Max))
			{
				Max = M_Interventions[i].Get_Min_RiskQ_Temp();
				elegido = i;
			}
		}
		if (type == 14)
		{
			if (M_Interventions[i].Get_RegretQ() > (Max))
			{
				Max = M_Interventions[i].Get_RegretQ();
				elegido = i;
			}
		}
	}
	return elegido;
}
int Maintenance::ElegirMayorAleatorizado(int type)
{
	double Max = -1;
	int elegido = -1;
	for (int k = 0; k < M_N_Interventions; k++)

	{
		int indice = M_Vec_Alea_I[k];
		if (M_Interventions_Colocada[indice] == true)
			continue;
		if (M_Interventions[indice].Get_T_Ordenados_Risk_Temp().size() == 0)
			continue;

		if (type == 10)
		{
			if (M_Interventions[indice].Get_Min_Risk_Temp() > (Max))
			{
				Max = M_Interventions[indice].Get_Min_Risk_Temp();
				elegido = indice;
			}
		}
		if (type == 11)
		{
			if (M_Interventions[indice].Get_Regret() > (Max ))
			{
				Max = M_Interventions[indice].Get_Regret();
				elegido = indice;
			}
		}
		if (type == 12)
		{
			if (M_Interventions[indice].Get_WorkloadC_Temp() > (Max))
			{
				Max = M_Interventions[indice].Get_WorkloadC_Temp();
				elegido = indice;
			}
		}
		if (type == 13)
		{
			if (M_Interventions[indice].Get_Min_RiskQ_Temp() > (Max))
			{
				Max = M_Interventions[indice].Get_Min_RiskQ_Temp();
				elegido = indice;
			}
		}
		if (type == 14)
		{
			if (M_Interventions[indice].Get_RegretQ() > (Max))
			{
				Max = M_Interventions[indice].Get_RegretQ();
				elegido = indice;
			}
		}
	}
	bool pasa = false;
//	printf("Elegido %d", elegido);
	if (elegido != (-1))
	{
		double umbral = (double)(M_Delta / 100) * Max;
		for (int k = 0; k < M_N_Interventions; k++)
		{
			int indice = M_Vec_Alea_I[k];
			if (M_Interventions_Colocada[indice] == true)
				continue;
			if (M_Interventions[indice].Get_T_Ordenados_Risk_Temp().size() == 0)
				continue;
			pasa = true;
			if (type == 10)
			{
				if (M_Interventions[indice].Get_Min_Risk_Temp() > (umbral-__FLT_EPSILON__))
				{
					return indice;

				}
			}
			if (type == 11)
			{
				if (M_Interventions[indice].Get_Regret() > (umbral - __FLT_EPSILON__))
				{
					return indice;
				}
			}
			if (type == 12)
			{
				if (M_Interventions[indice].Get_WorkloadC_Temp() > (umbral - __FLT_EPSILON__))
				{
					return indice;
				}
			}
			if (type == 13)
			{
				if (M_Interventions[indice].Get_Min_RiskQ_Temp() > (umbral - __FLT_EPSILON__))
				{
					return indice;

				}
			}
			if (type == 14)
			{
				if (M_Interventions[indice].Get_RegretQ() > (umbral - __FLT_EPSILON__))
				{
					return indice;
				}
			}

		}
	}
	return elegido;

}
bool Maintenance::ActualizarMejorSolution(double val,int lugar)
{
//	return false;
	if (val <= __FLT_EPSILON__) return false;
	if (val < M_Best_fo - __FLT_EPSILON__)
	{

		//es la primera vez
//		if (RepararSolutionBestT())
	//		val = FuncionObjetivo();
/*		double kk2 = FuncionObjetivo();

		if (val <(kk2 - __FLT_EPSILON__) || val>(kk2 + __FLT_EPSILON__))
		{
			double kk3 = FuncionObjetivoTemp();
		}*/
		M_Best_fo = val;
//		M_Best_I_T_Solution = M_I_T_Solution;
		
	
		M_Best_I_T = M_I_T;
		M_Best_Iter = M_Iter;

		M_Lugar = lugar;
		M_Best_fo_1 = M_Alpha * (M_fo_temp / (double)M_T);
		M_Best_fo_2= (1 - M_Alpha) * (M_fo2_temp / (double)M_T);
		CalcularSumasT(M_BestT_Suma);
//		Global_Best_Objective_Function = M_Best_fo;
		if (!M_Challenge_Mode) printf("\n %.5f Iter %d Tipo %d Lugar %d\n  ", M_Best_fo, M_Iter, M_Tipo, lugar);
//		cout << endl<< M_Best_fo << " Iter "<< M_Iter << " Tipo "<< M_Tipo<<" Lugar "<<lugar<<endl;
//		EjecutarValidadorPython();
		
//		Copy();
		VerifySolution(); 

		return true; 
	}
	return false;
}
void Maintenance::CalcularSumasT(vector<double>& SumaT)
{
	if (M_Pool == false)
	{
		for (int i = 0; i < M_T; i++)
		{

			SumaT[i] = M_T_Temp_Risk_S[i][0];
		}
	}
	else
	{
		for (int i = 0; i < M_T; i++)
		{
			SumaT[i]= M_T_Temp_Risk_FO_Suma[i];
//			SumaT[i] = accumulate(M_T_Temp_Risk_FO[i].begin(), M_T_Temp_Risk_FO[i].end(), 0.0);
		}
	}
}

void Maintenance::CalcularSumasT(vector<double> &SumaT, vector<int> &sol)
{
	for (register int i = 0; i < M_N_Interventions; i++)
	{
		SumaT[sol[i]] += M_Interventions[i].Get_RiskC(sol[i]);
	}

}
//Me dice si está en un t que ha cambiado algo
bool Maintenance::HaCambiadoT(int time, int duracion, vector<bool> &TiemposCambiado)
{
	for (register int i = time; i < time + duracion; i++)
	{
		if (TiemposCambiado[i])
			return true;
	}
	return false;
}
bool Maintenance::CompararDosSumasT(vector<bool> &Cambios,vector<double>& SumaT1, vector<double>& SumaT2)
{
	bool alguno = false;
	for (register int i = 0; i < M_T; i++)
	{
		if (SumaT1[i] < SumaT2[i] - __FLT_EPSILON__)
		{
			Cambios[i] = true;
			alguno = true;
			continue;
		}
		if (SumaT1[i] > SumaT2[i] + __FLT_EPSILON__)
		{
			Cambios[i] = true;
			alguno = true;
			continue;
		}
		Cambios[i] = false;
	}
	return alguno;
}
bool Maintenance::MejoraIntercambiosFo2()
{
//	return false;
	if (M_Pool == false)
		return false;
	bool Cambio = false;
	do
	{
		sort(M_Ivfo2_temp.begin(), M_Ivfo2_temp.end(),
			[&](size_t i, size_t j) { return M_vfo2_temp[i] > M_vfo2_temp[j]; });
		Cambio = false;
		double fobjAntiguo = FuncionObjetivoTemp();
		//Vamos probando para ir cambiando a donde se pueda mejorar
//		for (int t0 = 0; t0 < M_T && Cambio != true && M_vfo2_temp[M_Ivfo2_temp[t0]]>0; t0++)
		int cont_veces = 0;
		for (int t0 = 0; t0 < M_T && Cambio != true && cont_veces<2; t0++)
		{
			cont_veces++;
			//		random_shuffle(M_Vec_Alea_I.begin(), M_Vec_Alea_I.end());
			int t = M_Ivfo2_temp[t0];

			for (list<pair<int, int>>::iterator it = M_I_T_Solution.begin(); it != M_I_T_Solution.end() && Cambio != true; it++)
			{
				int inter = (*it).first;
				int t_inter = (*it).second;
				//			for (int k = 0; k < M_N_Interventions && Cambio!=true; k++)
				//			{
				//					int i = M_Vec_Alea_I[k];
				//					int inter = i;
				//					int t_inter = M_I_T[inter];

				//				double fobjAntiguo1 = FuncionObjetivoTemp();
								//No está aquí asignda
				if (M_Interventions[inter].Get_T_Temp() > t || M_Interventions[inter].Get_T_F_Temp() <= t)
					continue;
				if (M_Pool == false && M_Interventions[inter].Get_RiskC(t_inter) < (M_Interventions[inter].Get_Min_Risk() + __FLT_EPSILON__))
					continue;
				//			for (int k = 0; k < M_N_Interventions && Cambio!=true; k++)
				//			{
				//					int i = M_Vec_Alea_I[k];
				//					int inter = i;
				//					int t_inter = M_I_T[inter];

				//				double fobjAntiguo1 = FuncionObjetivoTemp();
								//No está aquí asignda
//				if (M_Interventions[inter].Get_T_Temp() > t || M_Interventions[inter].Get_T_F_Temp() <= t)
//					continue;
				double fobj1 = M_Interventions[inter].Get_RiskC(t_inter);
				M_fo_temp -= fobj1;

				//Quito lo de la función objetvio
				SubtractScenarios(inter);

				FuncionObjetivoTemp(M_Interventions[inter].Get_T_Temp(), M_Interventions[inter].Get_T_F_Temp());
				//Quito recursos
				QuitarRecursos(inter);
				//Recorro en orden de los iniciales
				int cont = 0;
				for (vector<int> ::iterator itt = M_Interventions[inter].Get_T_Ordenados_Risk().begin(); itt != M_Interventions[inter].Get_T_Ordenados_Risk().end() && Cambio != true && cont < get_random(6, M_N_Interventions); itt++, cont++)
				{
					if (t_inter == (*itt))
						continue;
					int timeini = (*itt);
					M_fo_temp += M_Interventions[inter].Get_RiskC(timeini);
					if ((M_Alpha * (M_fo_temp / (double)M_T)) > fobjAntiguo)
					{
						M_fo_temp -= M_Interventions[inter].Get_RiskC(timeini);
						continue;
					}

					if (PuedoColocar(inter, timeini))
					{

						int timefin = timeini + M_Interventions[inter].Get_Delta(timeini);
						for (int t3 = timeini; t3 < timefin; t3++)
						{
							for (int si = 0; si < M_Scenarios_number[t3]; si++)
							{
								M_T_Temp_Risk_S[t3][si] += M_Interventions[inter].Get_Risk()[t3][timeini][si];
							}
						}


						//						double fobjNew = FuncionObjetivoTemp();
						double fobjNew = FuncionObjetivoTemp(timeini, timefin);

						if (fobjNew < (fobjAntiguo - 0.0001))
						{
							//	(*it).second = timeini;
	//							CambiarSolution(inter, timeini);
//							PonerAlFinalSolution(inter, timeini);
							M_I_T[inter] = timeini;
							M_I_Seasons[inter] = M_T_Season[timeini];
							M_Interventions[inter].Set_T_Temp(timeini);
							AddRecursos(inter);
							Cambio = true;
							ActualizarMejorSolution(fobjNew,12);
						}
						else
						{
							M_fo_temp -= M_Interventions[inter].Get_RiskC(timeini);
							for (int t3 = timeini; t3 < timefin; t3++)
							{
								for (int si = 0; si < M_Scenarios_number[t3]; si++)
								{
									M_T_Temp_Risk_S[t3][si] -= M_Interventions[inter].Get_Risk()[t3][timeini][si];
								}
							}
							FuncionObjetivoTemp(timeini, timefin);
						}


					}
					else
						M_fo_temp -= M_Interventions[inter].Get_RiskC(timeini);


				}
				if (Cambio != true)
				{
					AddRecursos(inter);
					M_fo_temp += fobj1;
					for (int tt0 = M_Interventions[inter].Get_T_Temp(); tt0 < M_Interventions[inter].Get_T_F_Temp(); tt0++)
					{
						for (int si = 0; si < M_Scenarios_number[tt0]; si++)
						{
							M_T_Temp_Risk_S[tt0][si] += M_Interventions[inter].Get_Risk()[tt0][t_inter][si];
						}
					}
					FuncionObjetivoTemp(M_Interventions[inter].Get_T_Temp(), M_Interventions[inter].Get_T_F_Temp());
				}

			}
		}

		} while (Cambio == true);

		return false;
}
bool Maintenance::PerturbarSolution()
{


//		for (list<pair<int, int>>::iterator it = M_I_T_Solution.begin(); it != M_I_T_Solution.end() ; it++)
//		{
		for( int i=0;i<M_N_Interventions;i++)
		{
			if (get_random(0, 50) >= 5)
				continue;
			bool Cambio = false;
			int inter = i;
			int t_inter = M_I_T[i];
			//			for (int k = 0; k < M_N_Interventions && Cambio!=true; k++)
			//			{
			//					int i = M_Vec_Alea_I[k];
			//					int inter = i;
			//					int t_inter = M_I_T[inter];

			//				double fobjAntiguo1 = FuncionObjetivoTemp();
							//No está aquí asignda
//				if (M_Interventions[inter].Get_T_Temp() > t || M_Interventions[inter].Get_T_F_Temp() <= t)
//					continue;
			double fobj1 = M_Interventions[inter].Get_RiskC(t_inter);
			M_fo_temp -= fobj1;

			//Quito lo de la función objetvio
			for (int t = M_Interventions[inter].Get_T_Temp(); t < M_Interventions[inter].Get_T_F_Temp(); t++)
			{
				for (int si = 0; si < M_Scenarios_number[t]; si++)
				{
					M_T_Temp_Risk_S[t][si] -= M_Interventions[inter].Get_Risk()[t][t_inter][si];
				}
			}

			//Quito recursos
			QuitarRecursos(inter);
			//Recorro en orden de los iniciales
			int cont = 0;
			for (vector<int> ::iterator itt = M_Interventions[inter].Get_T_Ordenados_Risk().begin(); itt != M_Interventions[inter].Get_T_Ordenados_Risk().end() && Cambio != true; itt++, cont++)
			{
				if (t_inter == (*itt))
					continue;
				int timeini = (*itt);
				if (PuedoColocar(inter, timeini))
				{
					M_fo_temp += M_Interventions[inter].Get_RiskC(timeini);

					int timefin = timeini + M_Interventions[inter].Get_Delta(timeini);
					for (int t3 = timeini; t3 < timefin; t3++)
					{
						for (int si = 0; si < M_Scenarios_number[t3]; si++)
						{
							M_T_Temp_Risk_S[t3][si] += M_Interventions[inter].Get_Risk()[t3][timeini][si];
						}
					}

					CambiarSolution(inter, timeini);
					M_I_T[inter] = timeini;
					M_I_Seasons[inter] = M_T_Season[timeini];
					M_Interventions[inter].Set_T_Temp(timeini);
					AddRecursos(inter);
					Cambio = true;

				}




			}
			if (Cambio == false)
			{

				M_fo_temp += M_Interventions[inter].Get_RiskC(t_inter);
				for (int t = M_Interventions[inter].Get_T_Temp(); t < M_Interventions[inter].Get_T_F_Temp(); t++)
				{
					for (int si = 0; si < M_Scenarios_number[t]; si++)
					{
						M_T_Temp_Risk_S[t][si] += M_Interventions[inter].Get_Risk()[t][t_inter][si];
					}
				}

			}
	

		}

		return true;
}
double Maintenance::SimpleTabuMejorT()
{
	bool Cambio = false;

	bool Algun_Cambio = false;
	double fobjAntiguo = FuncionObjetivoTemp();
	double best_Move = fobjAntiguo;
	double	time_v1 = 0;
	int niter=0;
	do
	{
		Cambio = false;
		Algun_Cambio = false;
		if (M_Pool == false || get_random(0, 1) == 0)
		{
			//if (get_random(0, 4) >= 1)
			//	sort(M_Interventions_Dif.begin(), M_Interventions_Dif.end(),
			//		[&](size_t i, size_t j)
			//{ if ((M_Interventions[i].Get_RiskC(M_I_T[i]) - M_Interventions[i].Get_Min_Risk()) > (M_Interventions[j].Get_RiskC(M_I_T[j]) - M_Interventions[j].Get_Min_Risk()))
			//		return true;
			//else
			//		return (M_Interventions[i].Get_Min_Risk() > M_Interventions[j].Get_Min_Risk());
			//});
			//else
			random_shuffle(M_Interventions_Dif.begin(), M_Interventions_Dif.end());
		}
		else
		{
			ComputeExcessTemp();
			sort(M_Interventions_Dif.begin(), M_Interventions_Dif.end(), [&](size_t i, size_t j) { return ((M_Interventions[i].Get_Excess_temp()) > (M_Interventions[j].Get_Excess_temp())); });
		}


		struct timeb t1;
		ftime(&t1);
		time_v1 = ((double)((t1.time - M_Time_Initial.time) * 1000 + t1.millitm - M_Time_Initial.millitm)) / 1000;


		for (int index = 0; index < (M_N_Interventions) && (Algun_Cambio != true || M_Busqueda_Local == true) && time_v1 < M_Total_Time; index++)
		{
			Cambio = false;
			//		for (list<pair<int, int>>::iterator it = M_I_T_Solution.begin(); it != M_I_T_Solution.end() && Cambio != true; it++)
			//		{

				//		int inter = (*it).first;
				//		int t_inter = (*it).second;
			int inter = M_Interventions_Dif[index];



			int t_inter = M_I_T[inter];
			//El otro

			//		for (list<pair<int, int>>::iterator it = M_I_T_Solution.begin(); it != M_I_T_Solution.end() && Cambio != true; it++)
			//		{

			//			int inter = (*it).first;
			//			int t_inter = (*it).second;

			if (M_Pool == false && M_Interventions[inter].Get_RiskC(t_inter) < (M_Interventions[inter].Get_Min_Risk() + __FLT_EPSILON__))
				continue;

			if (!PuedoQuitar(inter, t_inter)) continue;
			double fobj1 = M_Interventions[inter].Get_RiskC(t_inter);
			M_fo_temp -= fobj1;

			bool primero_entra = false;
			//Recorro en orden de los iniciales
			int cont = 0;
			for (vector<int> ::iterator itt = M_Interventions[inter].Get_T_Ordenados_Risk().begin(); itt != M_Interventions[inter].Get_T_Ordenados_Risk().end() && Cambio != true; itt++)
			{
				if (t_inter == (*itt))
					continue;
				int timeini = (*itt);
				M_fo_temp += M_Interventions[inter].Get_RiskC(timeini);
				if ((M_Alpha * (M_fo_temp / (double)M_T)) > (fobjAntiguo - __FLT_EPSILON__ - M_Min_Improve))
				{
					M_fo_temp -= M_Interventions[inter].Get_RiskC(timeini);
					break;
				}
				if (M_Pool == false && M_Interventions[inter].Get_RiskC(timeini) > (M_Interventions[inter].Get_RiskC(t_inter) - __FLT_EPSILON__))
				{
					M_fo_temp -= M_Interventions[inter].Get_RiskC(timeini);
					break;

				}

				if (PuedoColocar(inter, timeini))
				{
					cont++;
					if (primero_entra == false)
					{


						//Quito lo de la función objetvio
						SubtractScenarios(inter);
						/*for (int t = M_Interventions[inter].Get_T_Temp(); t < M_Interventions[inter].Get_T_F_Temp(); t++)
						{
							for (int si = 0; si < M_Scenarios_number[t]; si++)
							{
								M_T_Temp_Risk_S[t][si] -= M_Interventions[inter].Get_RiskN(t,t_inter,si);
							}
						}*/
						double fobjNewkk = FuncionObjetivoTemp(M_Interventions[inter].Get_T_Temp(), M_Interventions[inter].Get_T_F_Temp());
						//Quito recursos
						QuitarRecursos(inter);
						primero_entra = true;
					}
					int timefin = timeini + M_Interventions[inter].Get_Delta(timeini);
					AddScenarios(inter, timeini, timefin);


					if (M_Pool) CopyFO2();
					//						double fobjNew = FuncionObjetivoTemp();
					double fobjNew = FuncionObjetivoTemp(timeini, timefin);

					//					if (fobjNew < (fobjAntiguo - __FLT_EPSILON__))
					if (fobjNew < (fobjAntiguo - M_Min_Improve) || (fobjNew < (M_Best_fo - __FLT_EPSILON__)))

					{
						//	(*it).second = timeini;
//							CambiarSolution(inter, timeini);
//						PonerAlFinalSolution(inter, timeini);
						M_I_T[inter] = timeini;
						//						M_I_Seasons[inter] = M_T_Season[timeini];
						M_Interventions[inter].Set_T_Temp(timeini);
						AddRecursos(inter);
						Cambio = true;
						Algun_Cambio = true;
						ActualizarMejorSolution(fobjNew, 3);
						best_Move = fobjNew;
						fobjAntiguo = fobjNew;
					}
					else
					{
						M_fo_temp -= M_Interventions[inter].Get_RiskC(timeini);
						SubtractScenarios(inter, timeini, timefin);
						/*						for (int t3 = timeini; t3 < timefin; t3++)
												{
													for (int si = 0; si < M_Scenarios_number[t3]; si++)
													{
														M_T_Temp_Risk_S[t3][si] -= M_Interventions[inter].Get_Risk()[t3][timeini][si];
													}
												}*/
						if (M_Pool) CopyBackFO2();
						//	FuncionObjetivoTemp(timeini, timefin);
					}


				}
				else
					M_fo_temp -= M_Interventions[inter].Get_RiskC(timeini);

				ftime(&t1);
				time_v1 = ((double)((t1.time - M_Time_Initial.time) * 1000 + t1.millitm - M_Time_Initial.millitm)) / 1000;


			}
			if (Cambio != true)
			{
				M_fo_temp += fobj1;
				if (primero_entra == true)
				{
					AddRecursos(inter);
					AddScenarios(inter);
					/*					for (int tt0 = M_Interventions[inter].Get_T_Temp(); tt0 < M_Interventions[inter].Get_T_F_Temp(); tt0++)
										{
											for (int si = 0; si < M_Scenarios_number[tt0]; si++)
											{
												M_T_Temp_Risk_S[tt0][si] += M_Interventions[inter].Get_Risk()[tt0][t_inter][si];
											}
										}*/
					FuncionObjetivoTemp(M_Interventions[inter].Get_T_Temp(), M_Interventions[inter].Get_T_F_Temp());
				}
			}

		}
		ftime(&t1);
		time_v1 = ((double)((t1.time - M_Time_Initial.time) * 1000 + t1.millitm - M_Time_Initial.millitm)) / 1000;
		niter++;
	} while (niter<100 && time_v1 < M_Total_Time);


	return best_Move;
}

//Esta función elije una intervention y la pone en el mejor t disponible
double Maintenance::MejoraIntercambiosMejorT()
{
	bool Cambio = false;

	bool Algun_Cambio = false;
	double fobjAntiguo = FuncionObjetivoTemp();
	double best_Move = fobjAntiguo;

	do
	{

		bool alguno_nuevo=CompararDosSumasT(M_T_M1, M_T_M1_Suma, M_BestT_Suma);
//		bool alguno_nuevo = true;
		M_T_M1_Suma = M_BestT_Suma;
		if (alguno_nuevo == false)
			return best_Move;
//		for (int ki = 0; ki < M_T; ki++) M_T_M1[ki] = true;
		Cambio = false;
		Algun_Cambio = false;
		if (M_Pool == false || get_random(0, 1) == 0)
		{
				//if (get_random(0, 4) >= 1)
				//	sort(M_Interventions_Dif.begin(), M_Interventions_Dif.end(),
				//		[&](size_t i, size_t j)
				//{ if ((M_Interventions[i].Get_RiskC(M_I_T[i]) - M_Interventions[i].Get_Min_Risk()) > (M_Interventions[j].Get_RiskC(M_I_T[j]) - M_Interventions[j].Get_Min_Risk()))
				//		return true;
				//else
				//		return (M_Interventions[i].Get_Min_Risk() > M_Interventions[j].Get_Min_Risk());
				//});
				//else
					random_shuffle(M_Interventions_Dif.begin(), M_Interventions_Dif.end());
		}
		else
		{
			ComputeExcessTemp();
			sort(M_Interventions_Dif.begin(), M_Interventions_Dif.end(), [&](size_t i, size_t j) { return ((M_Interventions[i].Get_Excess_temp()) > (M_Interventions[j].Get_Excess_temp())); });
		}


		struct timeb t1;
		ftime(&t1);
		double	time_v1 = ((double)((t1.time - M_Time_Initial.time) * 1000 + t1.millitm - M_Time_Initial.millitm)) / 1000;


		for (int index = 0; index < (M_N_Interventions) && (Algun_Cambio != true || M_Busqueda_Local==true) && time_v1<M_Total_Time; index++)
		{
			Cambio = false;
			//		for (list<pair<int, int>>::iterator it = M_I_T_Solution.begin(); it != M_I_T_Solution.end() && Cambio != true; it++)
			//		{

				//		int inter = (*it).first;
				//		int t_inter = (*it).second;
			int inter = M_Interventions_Dif[index];



			int t_inter = M_I_T[inter];
			//El otro

			//		for (list<pair<int, int>>::iterator it = M_I_T_Solution.begin(); it != M_I_T_Solution.end() && Cambio != true; it++)
			//		{

			//			int inter = (*it).first;
			//			int t_inter = (*it).second;

			if (M_Pool == false && M_Interventions[inter].Get_RiskC(t_inter) < (M_Interventions[inter].Get_Min_Risk() + __FLT_EPSILON__))
				continue;

			if (!PuedoQuitar(inter, t_inter)) continue;
			double fobj1 = M_Interventions[inter].Get_RiskC(t_inter);
			M_fo_temp -= fobj1;
			bool CambiaTOrigen = HaCambiadoT(t_inter, M_Interventions[inter].Get_Delta(t_inter), M_T_M1);
			bool primero_entra = false;
			//Recorro en orden de los iniciales
			int cont = 0;
			for (vector<int> ::iterator itt = M_Interventions[inter].Get_T_Ordenados_Risk().begin(); itt != M_Interventions[inter].Get_T_Ordenados_Risk().end() && (Cambio != true || M_Busqueda_Local==true) ; itt++)
			{
				if (t_inter == (*itt))
					continue;
				int timeini = (*itt);
				if (CambiaTOrigen==false
					&& HaCambiadoT((*itt), M_Interventions[inter].Get_Delta((*itt)), M_T_M1)==false)
					continue;

				M_fo_temp += M_Interventions[inter].Get_RiskC(timeini);
				if ((M_Alpha * (M_fo_temp / (double)M_T)) > (fobjAntiguo - __FLT_EPSILON__ - M_Min_Improve))
				{
					M_fo_temp -= M_Interventions[inter].Get_RiskC(timeini);
					break;
				}
				if (M_Pool == false && M_Interventions[inter].Get_RiskC(timeini) > (M_Interventions[inter].Get_RiskC(t_inter) - __FLT_EPSILON__))
				{
					M_fo_temp -= M_Interventions[inter].Get_RiskC(timeini);
					break;

				}

				if (PuedoColocar(inter, timeini))
				{
					cont++;
					if (primero_entra == false)
					{


						//Quito lo de la función objetvio
						SubtractScenarios(inter);
						/*for (int t = M_Interventions[inter].Get_T_Temp(); t < M_Interventions[inter].Get_T_F_Temp(); t++)
						{
							for (int si = 0; si < M_Scenarios_number[t]; si++)
							{
								M_T_Temp_Risk_S[t][si] -= M_Interventions[inter].Get_RiskN(t,t_inter,si);
							}
						}*/
						double fobjNewkk = FuncionObjetivoTemp(M_Interventions[inter].Get_T_Temp(), M_Interventions[inter].Get_T_F_Temp());
						//Quito recursos
						QuitarRecursos(inter);
						primero_entra = true;
					}
					int timefin = timeini + M_Interventions[inter].Get_Delta(timeini);
					AddScenarios(inter, timeini, timefin);


					if (M_Pool) CopyFO2();
					//						double fobjNew = FuncionObjetivoTemp();
					double fobjNew = FuncionObjetivoTemp(timeini, timefin);

//					if (fobjNew < (fobjAntiguo - __FLT_EPSILON__))
					if (fobjNew < (fobjAntiguo - M_Min_Improve) || (fobjNew < (M_Best_fo - __FLT_EPSILON__)))

					{
						//	(*it).second = timeini;
//							CambiarSolution(inter, timeini);
//						PonerAlFinalSolution(inter, timeini);
						M_I_T[inter] = timeini;
						//						M_I_Seasons[inter] = M_T_Season[timeini];
						M_Interventions[inter].Set_T_Temp(timeini);
						AddRecursos(inter);
						Cambio = true;
						Algun_Cambio = true;
						ActualizarMejorSolution(fobjNew, 3);
						best_Move = fobjNew;
						fobjAntiguo = fobjNew;
						CompararDosSumasT(M_T_M1, M_T_M1_Suma, M_BestT_Suma);
					}
					else
					{
						M_fo_temp -= M_Interventions[inter].Get_RiskC(timeini);
						SubtractScenarios(inter, timeini, timefin);
						/*						for (int t3 = timeini; t3 < timefin; t3++)
												{
													for (int si = 0; si < M_Scenarios_number[t3]; si++)
													{
														M_T_Temp_Risk_S[t3][si] -= M_Interventions[inter].Get_Risk()[t3][timeini][si];
													}
												}*/
						if (M_Pool) CopyBackFO2();
					//	FuncionObjetivoTemp(timeini, timefin);
					}


				}
				else
					M_fo_temp -= M_Interventions[inter].Get_RiskC(timeini);

				ftime(&t1);
				time_v1 = ((double)((t1.time - M_Time_Initial.time) * 1000 + t1.millitm - M_Time_Initial.millitm)) / 1000;


			}
			if (Cambio != true)
			{
				M_fo_temp += fobj1;
				if (primero_entra == true)
				{
					AddRecursos(inter);
					AddScenarios(inter);
					/*					for (int tt0 = M_Interventions[inter].Get_T_Temp(); tt0 < M_Interventions[inter].Get_T_F_Temp(); tt0++)
										{
											for (int si = 0; si < M_Scenarios_number[tt0]; si++)
											{
												M_T_Temp_Risk_S[tt0][si] += M_Interventions[inter].Get_Risk()[tt0][t_inter][si];
											}
										}*/
					FuncionObjetivoTemp(M_Interventions[inter].Get_T_Temp(), M_Interventions[inter].Get_T_F_Temp());
				}
			}

		}


	}while (Algun_Cambio == true && M_Busqueda_Local);

	
	return best_Move;
}

double Maintenance::PerturbarSolution(double fobjAntiguo)
{
	bool Cambio = false;
	int Porcentaje = 60;

	//	double fobjAntiguo = FuncionObjetivoTemp();

	double bestMove = fobjAntiguo;
	Cambio = false;
	double fobjNueva = 0;
	do
	{
		sort(M_Interventions_Dif.begin(), M_Interventions_Dif.end(),
			[&](size_t i, size_t j) { return (M_Interventions[i].Get_RiskC(M_I_T[i]) - M_Interventions[i].Get_Min_Risk()) > (M_Interventions[j].Get_RiskC(M_I_T[j]) - M_Interventions[j].Get_Min_Risk()); });

		for (int index = 0; index < M_N_Interventions; index++)
		{
			//		for (list<pair<int, int>>::iterator it = M_I_T_Solution.begin(); it != M_I_T_Solution.end() && Cambio != true; it++)
			//		{
					//		int inter = (*it).first;
					//		int t_inter = (*it).second;
			int inter = M_Interventions_Dif[index];

			int t_inter = M_I_T[inter];

			//Si son de las buenas
			if (M_Interventions[inter].Get_RiskC(t_inter) - (M_Interventions[inter].Get_Min_Risk()) < __FLT_EPSILON__)
			{
				if (get_random(0, 100) < Porcentaje)
					continue;
			}
			else
				if (get_random(0, 100) < (Porcentaje))
					continue;

			//			for (int k = 0; k < M_N_Interventions && Cambio!=true; k++)
			//			{
			//					int i = M_Vec_Alea_I[k];
			//					int inter = i;
			//					int t_inter = M_I_T[inter];

			//				double fobjAntiguo1 = FuncionObjetivoTemp();
							//No está aquí asignda
	//				if (M_Interventions[inter].Get_T_Temp() > t || M_Interventions[inter].Get_T_F_Temp() <= t)
	//					continue;
			double fobj1 = M_Interventions[inter].Get_RiskC(t_inter);
			M_fo_temp -= fobj1;
			//Quito lo de la función objetvio
			SubtractScenarios(inter);

			FuncionObjetivoTemp(M_Interventions[inter].Get_T_Temp(), M_Interventions[inter].Get_T_F_Temp());
			//Quito recursos
			QuitarRecursos(inter);
			M_I_T[inter] = -1;
			M_Interventions_Colocada[inter] = false;

//			Borrar(inter, t_inter);


		}
		fobjNueva = TerminarSolutionCopy(fobjAntiguo);
		Porcentaje -= 2;
	} while (fobjNueva > (bestMove - __FLT_EPSILON__) && fobjNueva < (bestMove + __FLT_EPSILON__) && Porcentaje>2);
	return fobjNueva;
}
double Maintenance::MejoraQuitar(double fobjAntiguo)
{

	bool Cambio = false;
	int Porcentaje = 70;

//	double fobjAntiguo = FuncionObjetivoTemp();

	Copy(false);
	struct timeb t1;
	ftime(&t1);
//	Porcentaje = get_random(50, 90);
	double	time_v1 = ((double)((t1.time - M_Time_Initial.time) * 1000 + t1.millitm - M_Time_Initial.millitm)) / 1000;

	double bestMove = fobjAntiguo;
	do
	{
		Cambio = false;

//		sort(M_Interventions_Dif.begin(), M_Interventions_Dif.end(),
//			[&](size_t i, size_t j) { return (M_Interventions[i].Get_RiskC(M_I_T[i]) - M_Interventions[i].Get_Min_Risk()) > (M_Interventions[j].Get_RiskC(M_I_T[j]) - M_Interventions[j].Get_Min_Risk()); });
		double fobjQuitado = 0;
		ftime(&t1);
		time_v1 = ((double)((t1.time - M_Time_Initial.time) * 1000 + t1.millitm - M_Time_Initial.millitm)) / 1000;

		for (int index = 0; index < M_N_Interventions && time_v1<M_Total_Time; index++)
		{

			//		for (list<pair<int, int>>::iterator it = M_I_T_Solution.begin(); it != M_I_T_Solution.end() && Cambio != true; it++)
			//		{

				//		int inter = (*it).first;
				//		int t_inter = (*it).second;
			int inter = M_Interventions_Dif[index];



			int t_inter = M_I_T[inter];

			//Si son de las buenas
			if (M_Interventions[inter].Get_RiskC(t_inter) - (M_Interventions[inter].Get_Min_Risk()) < __FLT_EPSILON__)
			{
				if (get_random(0, 100) < 90)
					continue;
			}
			else
				if (get_random(0, 100) < (Porcentaje))
					continue;
			if (PuedoQuitar(inter, t_inter) == false) continue;
			//			for (int k = 0; k < M_N_Interventions && Cambio!=true; k++)
			//			{
			//					int i = M_Vec_Alea_I[k];
			//					int inter = i;
			//					int t_inter = M_I_T[inter];

			//				double fobjAntiguo1 = FuncionObjetivoTemp();
							//No está aquí asignda
//				if (M_Interventions[inter].Get_T_Temp() > t || M_Interventions[inter].Get_T_F_Temp() <= t)
//					continue;
			double fobj1 = M_Interventions[inter].Get_RiskC(t_inter);
			M_fo_temp -= fobj1;
			//Quito lo de la función objetvio
			SubtractScenarios(inter);

			fobjQuitado = FuncionObjetivoTemp(M_Interventions[inter].Get_T_Temp(), M_Interventions[inter].Get_T_F_Temp());
			//Quito recursos
			QuitarRecursos(inter);
			M_I_T[inter] = -1;
			M_Interventions_Colocada[inter] = false;
			M_NI_Colocadas--;
//			Borrar(inter, t_inter);

			
		}
		double fobjNueva=TerminarSolutionCopy(bestMove);
		if (fobjNueva<__FLT_EPSILON__ || fobjNueva >(bestMove - __FLT_EPSILON__-M_Min_Improve))
		{
			CopyBack(false);
			Porcentaje += 3;

		}
		else
		{
			Copy(false);
			bestMove = fobjNueva;
		}



	} while (Porcentaje<100 && (bestMove>(fobjAntiguo-__FLT_EPSILON__) || M_Busqueda_Local) && time_v1<M_Total_Time);

	FuncionObjetivoTemp();
	return bestMove;
}

double Maintenance::MejoraQuitarT(double fobjAntiguo)
{

	bool Cambio = false;
	int Porcentaje = 10;

	//	double fobjAntiguo = FuncionObjetivoTemp();

	Copy(false);
	struct timeb t1;
	ftime(&t1);
	
	double	time_v1 = ((double)((t1.time - M_Time_Initial.time) * 1000 + t1.millitm - M_Time_Initial.millitm)) / 1000;

	double bestMove = fobjAntiguo;

	do
	{
		Cambio = false;
		int Porcentaje_Quitadas = (double)Porcentaje * (double)M_N_Interventions / 100;
		int Numero_Quitadas = 0;
		sort(M_T_Excess.begin(), M_T_Excess.end(), [&](size_t i, size_t j) { return ((M_vfo2_temp[i]) > (M_vfo2_temp[j])); });
		//Quito de los ts en orden
		vector<int> lista_quitadas;
		vector<bool> Quitadas(M_N_Interventions);
		double fobjQuitado = 0;
		for (int i = 0; i < M_T && (Numero_Quitadas < (2*Porcentaje_Quitadas)); i++)
		{

			if (get_random(0, 1) == 1)
				continue;
			//		sort(M_Interventions_Dif.begin(), M_Interventions_Dif.end(),
			//			[&](size_t i, size_t j) { return (M_Interventions[i].Get_RiskC(M_I_T[i]) - M_Interventions[i].Get_Min_Risk()) > (M_Interventions[j].Get_RiskC(M_I_T[j]) - M_Interventions[j].Get_Min_Risk()); });

			ftime(&t1);
			time_v1 = ((double)((t1.time - M_Time_Initial.time) * 1000 + t1.millitm - M_Time_Initial.millitm)) / 1000;
			
			//Quita por T
			for (int index = 0; index < M_N_Interventions && time_v1 < M_Total_Time && (Numero_Quitadas < Porcentaje_Quitadas); index++)
			{
				int inter=M_Interventions_Dif[index];
				if (Quitadas[inter] == true) continue;
				if (M_Interventions[inter].EstaT(M_T_Excess[i]) == false)
					continue;
				lista_quitadas.push_back(inter);
				Quitadas[inter] = true;
				Numero_Quitadas++;
				if (Numero_Quitadas > Porcentaje_Quitadas)
					break;
			}
			int PorcentajeDePredileccion = (double)10 * (double)M_T / (double) 100;
			//Quitar por predileccion
			for (int index = 0; index < M_N_Interventions && time_v1 < M_Total_Time && (Numero_Quitadas < (2*Porcentaje_Quitadas)); index++)
			{
				
				int inter = M_Interventions_Dif[index];
				if (Quitadas[inter] == true) continue;
				if (M_Interventions[inter].EstaT(M_T_Excess[i]) == true)
					continue;
				if (M_Interventions[inter].QuiereT(M_T_Excess[i], PorcentajeDePredileccion) == false)
					continue;
				lista_quitadas.push_back(inter);
				Quitadas[inter] = true;
				Numero_Quitadas++;
				if (Numero_Quitadas > (2*Porcentaje_Quitadas))
					break;
			}

		}
		for (int index = 0; index < lista_quitadas.size(); index++)
		{
			int inter = lista_quitadas[index];
			int t_inter = M_Interventions[inter].Get_T_Temp();
			if (PuedoQuitar(inter, t_inter) == false) continue;
			//			for (int k = 0; k < M_N_Interventions && Cambio!=true; k++)
			//			{
			//					int i = M_Vec_Alea_I[k];
			//					int inter = i;
			//					int t_inter = M_I_T[inter];

			//				double fobjAntiguo1 = FuncionObjetivoTemp();
							//No está aquí asignda
//				if (M_Interventions[inter].Get_T_Temp() > t || M_Interventions[inter].Get_T_F_Temp() <= t)
//					continue;
			double fobj1 = M_Interventions[inter].Get_RiskC(t_inter);
			M_fo_temp -= fobj1;
			//Quito lo de la función objetvio
			SubtractScenarios(inter);

			fobjQuitado = FuncionObjetivoTemp(M_Interventions[inter].Get_T_Temp(), M_Interventions[inter].Get_T_F_Temp());
			//Quito recursos
			QuitarRecursos(inter);
			M_I_T[inter] = -1;
			M_Interventions_Colocada[inter] = false;
			M_NI_Colocadas--;


			//			Borrar(inter, t_inter);


		}
		double fobjNueva = TerminarSolutionCopy(bestMove);
		if (fobjNueva<__FLT_EPSILON__ || fobjNueva >(bestMove - __FLT_EPSILON__))
		{
			CopyBack(false);
			Porcentaje += 5;

		}
		else
		{
			Copy(false);
			bestMove = fobjNueva;
		}



	} while (Porcentaje < 25 && (bestMove > (fobjAntiguo - __FLT_EPSILON__) || M_Busqueda_Local) && time_v1 < M_Total_Time);

	FuncionObjetivoTemp();
	return bestMove;
}
double Maintenance::QuitarT(double fobjAntiguo)
{

	bool Cambio = false;
	int Porcentaje = get_random(10,50);

	//	double fobjAntiguo = FuncionObjetivoTemp();

	Copy(false);
	struct timeb t1;
	ftime(&t1);

	double	time_v1 = ((double)((t1.time - M_Time_Initial.time) * 1000 + t1.millitm - M_Time_Initial.millitm)) / 1000;
	double fobjNueva = 0;
	double bestMove = fobjAntiguo;

	do
	{
		Cambio = false;
		int Porcentaje_Quitadas = (double)Porcentaje * (double)M_N_Interventions / 100;
		int Numero_Quitadas = 0;
		sort(M_T_Excess.begin(), M_T_Excess.end(), [&](size_t i, size_t j) { return ((M_vfo2_temp[i]) > (M_vfo2_temp[j])); });
		//Quito de los ts en orden
		vector<int> lista_quitadas;
		vector<bool> Quitadas(M_N_Interventions);
		double fobjQuitado = 0;
		for (int i = 0; i < M_T && (Numero_Quitadas < (2 * Porcentaje_Quitadas)); i++)
		{

			if (get_random(0, 1) == 1)
				continue;
			//		sort(M_Interventions_Dif.begin(), M_Interventions_Dif.end(),
			//			[&](size_t i, size_t j) { return (M_Interventions[i].Get_RiskC(M_I_T[i]) - M_Interventions[i].Get_Min_Risk()) > (M_Interventions[j].Get_RiskC(M_I_T[j]) - M_Interventions[j].Get_Min_Risk()); });

			ftime(&t1);
			time_v1 = ((double)((t1.time - M_Time_Initial.time) * 1000 + t1.millitm - M_Time_Initial.millitm)) / 1000;

			//Quita por T
			for (int index = 0; index < M_N_Interventions && time_v1 < M_Total_Time && (Numero_Quitadas < Porcentaje_Quitadas); index++)
			{
				int inter = M_Interventions_Dif[index];
				if (Quitadas[inter] == true) continue;
				if (M_Interventions[inter].EstaT(M_T_Excess[i]) == false)
					continue;
				lista_quitadas.push_back(inter);
				Quitadas[inter] = true;
				Numero_Quitadas++;
				if (Numero_Quitadas > Porcentaje_Quitadas)
					break;
			}
			int PorcentajeDePredileccion = (double)10 * (double)M_T / (double)100;
			//Quitar por predileccion
			for (int index = 0; index < M_N_Interventions && time_v1 < M_Total_Time && (Numero_Quitadas < (2 * Porcentaje_Quitadas)); index++)
			{

				int inter = M_Interventions_Dif[index];
				if (Quitadas[inter] == true) continue;
				if (M_Interventions[inter].EstaT(M_T_Excess[i]) == true)
					continue;
				if (M_Interventions[inter].QuiereT(M_T_Excess[i], PorcentajeDePredileccion) == false)
					continue;
				lista_quitadas.push_back(inter);
				Quitadas[inter] = true;
				Numero_Quitadas++;
				if (Numero_Quitadas > (2 * Porcentaje_Quitadas))
					break;
			}

		}
		for (int index = 0; index < lista_quitadas.size(); index++)
		{
			int inter = lista_quitadas[index];
			int t_inter = M_Interventions[inter].Get_T_Temp();
			if (PuedoQuitar(inter, t_inter) == false) continue;
			//			for (int k = 0; k < M_N_Interventions && Cambio!=true; k++)
			//			{
			//					int i = M_Vec_Alea_I[k];
			//					int inter = i;
			//					int t_inter = M_I_T[inter];

			//				double fobjAntiguo1 = FuncionObjetivoTemp();
							//No está aquí asignda
//				if (M_Interventions[inter].Get_T_Temp() > t || M_Interventions[inter].Get_T_F_Temp() <= t)
//					continue;
			double fobj1 = M_Interventions[inter].Get_RiskC(t_inter);
			M_fo_temp -= fobj1;
			//Quito lo de la función objetvio
			SubtractScenarios(inter);

			fobjQuitado = FuncionObjetivoTemp(M_Interventions[inter].Get_T_Temp(), M_Interventions[inter].Get_T_F_Temp());
			//Quito recursos
			QuitarRecursos(inter);
			M_I_T[inter] = -1;
			M_Interventions_Colocada[inter] = false;
			M_NI_Colocadas--;


			//			Borrar(inter, t_inter);


		}
		fobjNueva = TerminarSolutionPrimeraCopy(bestMove);
		if (fobjNueva<__FLT_EPSILON__ )
		{
			CopyBack(false);
			Porcentaje += 5;

		}
		else
		{
			Copy(false);
			bestMove = fobjNueva;
			FuncionObjetivoTemp();
			return bestMove;
		}



	} while (Porcentaje < 35 && fobjNueva>__FLT_MAX__ && time_v1 < M_Total_Time);

	FuncionObjetivoTemp();
	return bestMove;
}
double Maintenance::DestruirCplex(double fobjAntiguo)
{
	int tipo = get_random(0, 2);
	bool Cambio = false;
	//Total variables sin exclusiones
	timeb t1;
	int Porcentaje = 90;
//	int PorcentajeFin = (double)(M_Iter - M_No_Sol) * (double)100 / (double)(M_Iter + 1);
	if (!M_Challenge_Mode)
		printf("Reparar Hilo %d Iter %d\n", M_Hilo, M_Iter);
	//	if (tipo == 2)
	//		Porcentaje = 80;

		//	double fobjAntiguo = FuncionObjetivoTemp();
	double	time_v1 = 0;
	Copy(false);

	bool stop = false;
	M_Formulacion_Cuantiles = false;
	M_Max_Formulation = false;
	do
	{
		tipo = get_random(0, 2);
		Cambio = false;
		M_Mixed_Risk = false;

		//		sort(M_Interventions_Dif.begin(), M_Interventions_Dif.end(),
		//			[&](size_t i, size_t j) { return (M_Interventions[i].Get_RiskC(M_I_T[i]) - M_Interventions[i].Get_Min_Risk()) > (M_Interventions[j].Get_RiskC(M_I_T[j]) - M_Interventions[j].Get_Min_Risk()); });
		double fobjQuitado = 0;

		random_shuffle(M_Interventions_Dif.begin(), M_Interventions_Dif.end());
		for (int index = 0; index < M_N_Interventions; index++)
		{

			//		for (list<pair<int, int>>::iterator it = M_I_T_Solution.begin(); it != M_I_T_Solution.end() && Cambio != true; it++)
			//		{

				//		int inter = (*it).first;
				//		int t_inter = (*it).second;
			int inter = M_Interventions_Dif[index];



			int t_inter = M_I_T[inter];


			if (M_Interventions_Colocada[inter] != false)
			{
				//Si son de las buenas
				if (M_Interventions[inter].Get_RiskC(t_inter) - (M_Interventions[inter].Get_Min_Risk()) < __FLT_EPSILON__)
				{
					if (get_random(0, 100) < 90)
						continue;
				}
				//Si son de las buenas
				if (get_random(0, 100) < (Porcentaje))
					continue;
				if (PuedoQuitar(inter, t_inter) == false) continue;

				//			for (int k = 0; k < M_N_Interventions && Cambio!=true; k++)
				//			{
				//					int i = M_Vec_Alea_I[k];
				//					int inter = i;
				//					int t_inter = M_I_T[inter];

				//				double fobjAntiguo1 = FuncionObjetivoTemp();
								//No está aquí asignda
	//				if (M_Interventions[inter].Get_T_Temp() > t || M_Interventions[inter].Get_T_F_Temp() <= t)
	//					continue;
				double fobj1 = M_Interventions[inter].Get_RiskC(t_inter);
				M_fo_temp -= fobj1;
				//Quito lo de la función objetvio
				SubtractScenarios(inter);

				fobjQuitado = FuncionObjetivoTemp(M_Interventions[inter].Get_T_Temp(), M_Interventions[inter].Get_T_F_Temp());
				//Quito recursos
				QuitarRecursos(inter);
				M_I_T[inter] = -1;
				M_Interventions_Colocada[inter] = false;
				M_NI_Colocadas--;
				//		Borrar(inter, t_inter);

			}
		}


		ftime(&t1);
		time_v1 = ((double)((t1.time - M_Time_Initial.time) * 1000 + t1.millitm - M_Time_Initial.millitm)) / 1000;
		M_Time_Modelo = M_Total_Time - time_v1;

		if (M_Time_Modelo > 0)
		{
			M_Reparar = false;
			double fobjNueva = IntegerFormulationPartial(0);
			if (fobjNueva > 0)
			{
				ActualizarMejorSolution(fobjNueva, 6);

				stop = true;
				fobjNueva = MejoraQuitar(fobjNueva);
			}

		}
		if (stop != true)
		{

			CopyBack(false);
			Porcentaje -= 10;

		}

		ftime(&t1);
		time_v1 = ((double)((t1.time - M_Time_Initial.time) * 1000 + t1.millitm - M_Time_Initial.millitm)) / 1000;



	} while (stop == false && M_NI_Colocadas > 0 && time_v1 < M_Total_Time );
//	if (!M_Challenge_Mode)
//		printf("Reparar Hilo %d Iter %d Reparada %d\n", M_Hilo, M_Iter, stop);
	double valor=FuncionObjetivoTemp();
	VerifySolution();
	return valor;
}



double Maintenance::RepararCplex(double fobjAntiguo)
{
	int tipo = get_random(0, 2);
	bool Cambio = false;
	M_Reparar = true;
	//Total variables sin exclusiones
	timeb t1;
	int Porcentaje = 50;
	int PorcentajeBuenas = 90;
	if (M_Best_I_T.size() == 0)
	{
		Porcentaje = 10;
		PorcentajeBuenas = 10;
	}
	int PorcentajeFin = (double)(M_Iter - M_No_Sol) * (double)100 / (double)(M_Iter + 1);
	if (!M_Challenge_Mode)
		printf("Reparar Hilo %d Iter %d\n", M_Hilo, M_Iter);
	//	if (tipo == 2)
	//		Porcentaje = 80;

		//	double fobjAntiguo = FuncionObjetivoTemp();
	double	time_v1 = 0;
	Copy(false);

	bool stop = false;
	M_Formulacion_Cuantiles = false; 
	M_Max_Formulation = false;
	do
	{
		tipo = get_random(0, 2);
		Cambio = false;
		M_Mixed_Risk = false;

		//		sort(M_Interventions_Dif.begin(), M_Interventions_Dif.end(),
		//			[&](size_t i, size_t j) { return (M_Interventions[i].Get_RiskC(M_I_T[i]) - M_Interventions[i].Get_Min_Risk()) > (M_Interventions[j].Get_RiskC(M_I_T[j]) - M_Interventions[j].Get_Min_Risk()); });
		double fobjQuitado = 0;

		random_shuffle(M_Interventions_Dif.begin(), M_Interventions_Dif.end());
		for (int index = 0; index < M_N_Interventions; index++)
		{

			//		for (list<pair<int, int>>::iterator it = M_I_T_Solution.begin(); it != M_I_T_Solution.end() && Cambio != true; it++)
			//		{

				//		int inter = (*it).first;
				//		int t_inter = (*it).second;
			int inter = M_Interventions_Dif[index];



			int t_inter = M_I_T[inter];


			if (M_Interventions_Colocada[inter] != false)
			{
				//Si son de las buenas
				if (M_Interventions[inter].Get_RiskC(t_inter) - (M_Interventions[inter].Get_Min_Risk()) < __FLT_EPSILON__)
				{
					if (get_random(0, 100) < PorcentajeBuenas)
						continue;
				}
				//Si son de las buenas
				if (get_random(0, 100) < (Porcentaje))
					continue;
				if (PuedoQuitar(inter, t_inter) == false) continue;

				//			for (int k = 0; k < M_N_Interventions && Cambio!=true; k++)
				//			{
				//					int i = M_Vec_Alea_I[k];
				//					int inter = i;
				//					int t_inter = M_I_T[inter];

				//				double fobjAntiguo1 = FuncionObjetivoTemp();
								//No está aquí asignda
	//				if (M_Interventions[inter].Get_T_Temp() > t || M_Interventions[inter].Get_T_F_Temp() <= t)
	//					continue;
				double fobj1 = M_Interventions[inter].Get_RiskC(t_inter);
				M_fo_temp -= fobj1;
				//Quito lo de la función objetvio
				SubtractScenarios(inter);

				fobjQuitado = FuncionObjetivoTemp(M_Interventions[inter].Get_T_Temp(), M_Interventions[inter].Get_T_F_Temp());
				//Quito recursos
				QuitarRecursos(inter);
				M_I_T[inter] = -1;
				M_Interventions_Colocada[inter] = false;
				M_NI_Colocadas--;
				//		Borrar(inter, t_inter);

			}
		}


		ftime(&t1);
		time_v1 = ((double)((t1.time - M_Time_Initial.time) * 1000 + t1.millitm - M_Time_Initial.millitm)) / 1000;
		M_Time_Modelo = M_Total_Time- time_v1;

		if (M_Time_Modelo > 0)
		{
			double fobjNueva = IntegerFormulationPartial(0);
			if (fobjNueva > 0)
			{
				ActualizarMejorSolution(fobjNueva, 6);

				stop = true;
				fobjNueva = MejoraQuitar(fobjNueva);
			}

		}
		if (stop!=true)
		{

			CopyBack(false);
			Porcentaje -= 10;

		}

		ftime(&t1);
		time_v1 = ((double)((t1.time - M_Time_Initial.time) * 1000 + t1.millitm - M_Time_Initial.millitm)) / 1000;



	} while (stop == false && M_NI_Colocadas>0 && time_v1<M_Total_Time && ((Porcentaje>PorcentajeFin)|| (M_Best_I_T.size() == 0 && Porcentaje>-5)));
	if (!M_Challenge_Mode)
		printf("Reparar Hilo %d Iter %d Reparada %d\n", M_Hilo, M_Iter,stop);
	FuncionObjetivoTemp();
	VerifySolution();
	return stop;
}

double Maintenance::MejoraQuitarCplex(double fobjAntiguo)
{
	int tipo = get_random(0, 2);
	bool Cambio = false;
	//Total variables sin exclusiones

	int Porcentaje = max((double)85,100-((double) (50*100)/(double)M_N_Interventions));
//	if (tipo == 2)
//		Porcentaje = 80;

	//	double fobjAntiguo = FuncionObjetivoTemp();

	Copy(false);
	double bestMove = fobjAntiguo;
	bool stop = false;
	do
	{
		tipo = get_random(0, 2);
		Cambio = false;
		if (get_random(0, 4) == 0)
			M_Mixed_Risk = true;
		else
			M_Mixed_Risk = false;

//		sort(M_Interventions_Dif.begin(), M_Interventions_Dif.end(),
//			[&](size_t i, size_t j) { return (M_Interventions[i].Get_RiskC(M_I_T[i]) - M_Interventions[i].Get_Min_Risk()) > (M_Interventions[j].Get_RiskC(M_I_T[j]) - M_Interventions[j].Get_Min_Risk()); });
		double fobjQuitado = 0;


		for (int index = 0; index < M_N_Interventions; index++)
		{

			//		for (list<pair<int, int>>::iterator it = M_I_T_Solution.begin(); it != M_I_T_Solution.end() && Cambio != true; it++)
			//		{

				//		int inter = (*it).first;
				//		int t_inter = (*it).second;
			int inter = M_Interventions_Dif[index];



			int t_inter = M_I_T[inter];

			//Si son de las buenas
			if (get_random(0, 100) < (Porcentaje))
				continue;
			if (PuedoQuitar(inter, t_inter) == false) continue;

			//			for (int k = 0; k < M_N_Interventions && Cambio!=true; k++)
			//			{
			//					int i = M_Vec_Alea_I[k];
			//					int inter = i;
			//					int t_inter = M_I_T[inter];

			//				double fobjAntiguo1 = FuncionObjetivoTemp();
							//No está aquí asignda
//				if (M_Interventions[inter].Get_T_Temp() > t || M_Interventions[inter].Get_T_F_Temp() <= t)
//					continue;
			double fobj1 = M_Interventions[inter].Get_RiskC(t_inter);
			M_fo_temp -= fobj1;
			//Quito lo de la función objetvio
			SubtractScenarios(inter);

			fobjQuitado = FuncionObjetivoTemp(M_Interventions[inter].Get_T_Temp(), M_Interventions[inter].Get_T_F_Temp());
			//Quito recursos
			QuitarRecursos(inter);
			M_I_T[inter] = -1;
			M_Interventions_Colocada[inter] = false;
			M_NI_Colocadas--;
	//		Borrar(inter, t_inter);


		}

		if (tipo == 1) {
			M_Formulacion_Cuantiles = true; M_Max_Formulation = false;
		}
		if (tipo == 3)		M_Max_Formulation = true;
		if (tipo == 0 || M_Pool==false) {
			M_Formulacion_Cuantiles = false; M_Max_Formulation = false;
		}
		double fobjNueva =IntegerFormulationPartial(0);
		ActualizarMejorSolution(fobjNueva, 6);
		if (fobjNueva < 0)
			stop = true;
		if (fobjNueva<__FLT_EPSILON__ || fobjNueva >(bestMove - __FLT_EPSILON__-M_Min_Improve))
		{
			
			CopyBack(false);
			Porcentaje -= 5;

		}
		else
		{
			Copy(false);
			bestMove = fobjNueva;
		}



	} while (stop==false && Porcentaje>55 && (bestMove>(fobjAntiguo - __FLT_EPSILON__-M_Min_Improve) || M_Busqueda_Local));

	FuncionObjetivoTemp();
	VerifySolution();
	return bestMove;
}
double Maintenance::MejoraQuitarF2(int Type)
{

	bool Cambio = false;

	int cont_veces = 2;
	double fobjAntiguo = FuncionObjetivoTemp();
	Copy();
	double bestMove = fobjAntiguo;
	do
	{
		Cambio = false;

		sort(M_Ivfo2_temp.begin(), M_Ivfo2_temp.end(),
			[&](size_t i, size_t j) { return M_vfo2_temp[i] > M_vfo2_temp[j]; });


		//Vamos probando para ir cambiando a donde se pueda mejorar
//		for (int t0 = 0; t0 < M_T && Cambio != true && M_vfo2_temp[M_Ivfo2_temp[t0]]>0; t0++)
		int cont = 0;
		for (int t0 = 0; t0 < M_T && cont < cont_veces; t0++)
		{
			cont++;
			//		random_shuffle(M_Vec_Alea_I.begin(), M_Vec_Alea_I.end());
			int ti = M_Ivfo2_temp[t0];

			for (list<pair<int, int>>::iterator it = M_I_T_Solution.begin(); it != M_I_T_Solution.end() ; it++)
			{
				int inter = (*it).first;
				int t_inter = (*it).second;
				if (M_Interventions[inter].Get_T_Temp() > ti || M_Interventions[inter].Get_T_F_Temp() <= ti)
					continue;
				//			for (int k = 0; k < M_N_Interventions && Cambio!=true; k++)
				//			{
				//					int i = M_Vec_Alea_I[k];
				//					int inter = i;
				//					int t_inter = M_I_T[inter];

				//				double fobjAntiguo1 = FuncionObjetivoTemp();
								//No está aquí asignda

			//			for (int k = 0; k < M_N_Interventions && Cambio!=true; k++)
			//			{
			//					int i = M_Vec_Alea_I[k];
			//					int inter = i;
			//					int t_inter = M_I_T[inter];

			//				double fobjAntiguo1 = FuncionObjetivoTemp();
							//No está aquí asignda
//				if (M_Interventions[inter].Get_T_Temp() > t || M_Interventions[inter].Get_T_F_Temp() <= t)
//					continue;
				double fobj1 = M_Interventions[inter].Get_RiskC(t_inter);
				M_fo_temp -= fobj1;
				//Quito lo de la función objetvio
				for (int t = M_Interventions[inter].Get_T_Temp(); t < M_Interventions[inter].Get_T_F_Temp(); t++)
				{
					for (int si = 0; si < M_Scenarios_number[t]; si++)
					{
						M_T_Temp_Risk_S[t][si] -= M_Interventions[inter].Get_Risk()[t][t_inter][si];
					}
				}
				double fobjQuitado = FuncionObjetivoTemp(M_Interventions[inter].Get_T_Temp(), M_Interventions[inter].Get_T_F_Temp());
				//Quito recursos
				QuitarRecursos(inter);
				M_I_T[inter] = -1;
//				Borrar(inter, t_inter);


			}
		}
		double fobjNueva = TerminarSolutionCopy(Type);
		if (fobjNueva<0 || fobjNueva >(bestMove - __FLT_EPSILON__))
		{
			CopyBack();
			cont_veces += 2;
			if (Type == 13)
				cont_veces += 3;
		}
		else
		{
			Copy();
			bestMove = fobjNueva;
		}



	} while (cont_veces < 12);
	if (bestMove < fobjAntiguo - __FLT_EPSILON__)
	{
		M_Mejora4++;
		return true;
	}
	return false;
}
bool Maintenance::MejoraIntercambiosFo1Fast()
{
	bool Algun_Cambio = false;
	bool Cambio = false;
	do
	{
		Algun_Cambio = false;
		Cambio = false;
		double fobjAntiguo = FuncionObjetivoTemp();
		double best_Move = fobjAntiguo;
		//	sort(M_Interventions_Dif.begin(), M_Interventions_Dif.end(),
		//		[&](size_t i, size_t j) { return (M_Interventions[i].Get_RiskC(M_I_T[i])- M_Interventions[i].Get_Min_Risk()) > (M_Interventions[j].Get_RiskC(M_I_T[j]) - M_Interventions[j].Get_Min_Risk()); });
	/*		double kk2 = FuncionObjetivo();
			if (fobjAntiguo <(kk2 - __FLT_EPSILON__) || fobjAntiguo>(kk2 + __FLT_EPSILON__))
			{
				int kk = 9;
			}*/
			//Vamos probando para ir cambiando a donde se pueda mejorar
	//		for (int t0 = 0; t0 < M_T && Cambio != true && M_vfo2_temp[M_Ivfo2_temp[t0]]>0; t0++)

		for (list<pair<int, int>>::iterator it = M_I_T_Solution.begin(); it != M_I_T_Solution.end() ; it++)
		{
			if (Cambio == true)
			{
				fobjAntiguo = FuncionObjetivoTemp();
				best_Move = fobjAntiguo;
			}
			Cambio = false;
			int inter = (*it).first;
			int t_inter = (*it).second;
//			if (t_inter == 76 && inter == 49)
//				int kk = 9;
			if (M_Pool == false && M_Interventions[inter].Get_RiskC(t_inter) < (M_Interventions[inter].Get_Min_Risk() + __FLT_EPSILON__))
				continue;
			//			for (int k = 0; k < M_N_Interventions && Cambio!=true; k++)
			//			{
			//					int i = M_Vec_Alea_I[k];
			//					int inter = i;
			//					int t_inter = M_I_T[inter];

			//				double fobjAntiguo1 = FuncionObjetivoTemp();
							//No está aquí asignda
//				if (M_Interventions[inter].Get_T_Temp() > t || M_Interventions[inter].Get_T_F_Temp() <= t)
//					continue;
			double fobj1 = M_Interventions[inter].Get_RiskC(t_inter);
			M_fo_temp -= fobj1;

			//Quito lo de la función objetvio
			for (int t = M_Interventions[inter].Get_T_Temp(); t < M_Interventions[inter].Get_T_F_Temp(); t++)
			{
				for (int si = 0; si < M_Scenarios_number[t]; si++)
				{
					M_T_Temp_Risk_S[t][si] -= M_Interventions[inter].Get_Risk()[t][t_inter][si];
				}
			}
			FuncionObjetivoTemp(M_Interventions[inter].Get_T_Temp(), M_Interventions[inter].Get_T_F_Temp());
			//Quito recursos
			QuitarRecursos(inter);
			//Recorro en orden de los iniciales
			int cont = 0;
			for (vector<int> ::iterator itt = M_Interventions[inter].Get_T_Ordenados_Risk().begin(); itt != M_Interventions[inter].Get_T_Ordenados_Risk().end() && Cambio != true && cont < get_random(6, M_N_Interventions); itt++, cont++)
			{
				if (t_inter == (*itt))
					continue;
				int timeini = (*itt);
				M_fo_temp += M_Interventions[inter].Get_RiskC(timeini);
				if ((M_Alpha * (M_fo_temp / (double)M_T)) > fobjAntiguo)
				{
					M_fo_temp -= M_Interventions[inter].Get_RiskC(timeini);
					continue;
				}
				if (M_Pool == false && M_Interventions[inter].Get_RiskC(timeini) > (M_Interventions[inter].Get_RiskC(t_inter) - __FLT_EPSILON__))
				{
					M_fo_temp -= M_Interventions[inter].Get_RiskC(timeini);
					continue;
				}
				if (PuedoColocar(inter, timeini))
				{

					int timefin = timeini + M_Interventions[inter].Get_Delta(timeini);
					for (int t3 = timeini; t3 < timefin; t3++)
					{
						for (int si = 0; si < M_Scenarios_number[t3]; si++)
						{
							M_T_Temp_Risk_S[t3][si] += M_Interventions[inter].Get_Risk()[t3][timeini][si];
						}
					}


					//						double fobjNew = FuncionObjetivoTemp();
					double fobjNew = FuncionObjetivoTemp(timeini, timefin);

					if (fobjNew < (fobjAntiguo - 0.0001))
					{
						//	(*it).second = timeini;
//							CambiarSolution(inter, timeini);
//						PonerAlFinalSolution(inter, timeini);
						M_I_T[inter] = timeini;
						M_I_Seasons[inter] = M_T_Season[timeini];
						M_Interventions[inter].Set_T_Temp(timeini);
						AddRecursos(inter);
						Cambio = true;
						Algun_Cambio = true;
						ActualizarMejorSolution(fobjNew,3);
					}
					else
					{
						M_fo_temp -= M_Interventions[inter].Get_RiskC(timeini);
						for (int t3 = timeini; t3 < timefin; t3++)
						{
							for (int si = 0; si < M_Scenarios_number[t3]; si++)
							{
								M_T_Temp_Risk_S[t3][si] -= M_Interventions[inter].Get_Risk()[t3][timeini][si];
							}
						}
						FuncionObjetivoTemp(timeini, timefin);
					}


				}
				else
					M_fo_temp -= M_Interventions[inter].Get_RiskC(timeini);


			}
			if (Cambio != true)
			{
				AddRecursos(inter);
				M_fo_temp += fobj1;
				for (int tt0 = M_Interventions[inter].Get_T_Temp(); tt0 < M_Interventions[inter].Get_T_F_Temp(); tt0++)
				{
					for (int si = 0; si < M_Scenarios_number[tt0]; si++)
					{
						M_T_Temp_Risk_S[tt0][si] += M_Interventions[inter].Get_Risk()[tt0][t_inter][si];
					}
				}
				FuncionObjetivoTemp(M_Interventions[inter].Get_T_Temp(), M_Interventions[inter].Get_T_F_Temp());
			}

		}

	} while (Algun_Cambio == true);

	return false;
}
bool Maintenance::MejoraIntercambiosFo1Order()
{
	bool Cambio = false;
	bool stop = false;
	do
	{
		Cambio = false;
		double fobjAntiguo = FuncionObjetivoTemp();

		sort(M_Interventions_Dif.begin(), M_Interventions_Dif.end(),
			[&](size_t i, size_t j) { return (M_Interventions[i].Get_RiskC(M_I_T[i]) - M_Interventions[i].Get_Min_Risk()) > (M_Interventions[j].Get_RiskC(M_I_T[j]) - M_Interventions[j].Get_Min_Risk()); });
		/*		double kk2 = FuncionObjetivo();
				if (fobjAntiguo <(kk2 - __FLT_EPSILON__) || fobjAntiguo>(kk2 + __FLT_EPSILON__))
				{
					int kk = 9;
				}*/
				//Vamos probando para ir cambiando a donde se pueda mejorar
		//		for (int t0 = 0; t0 < M_T && Cambio != true && M_vfo2_temp[M_Ivfo2_temp[t0]]>0; t0++)
		int cont_veces = 0;
		for (int index=0;index<M_N_Interventions && Cambio!=true && stop!=true ;index++)
		{
			cont_veces++;
//		for (list<pair<int, int>>::iterator it = M_I_T_Solution.begin(); it != M_I_T_Solution.end() && Cambio != true; it++)
//		{
			
	//		int inter = (*it).first;
	//		int t_inter = (*it).second;
			int inter = M_Interventions_Dif[index];
			int t_inter = M_I_T[inter];
			if (M_Pool == false && (M_Interventions[inter].Get_RiskC(t_inter) - M_Interventions[inter].Get_Min_Risk()) < __FLT_EPSILON__)
			{
				stop = true;
				continue;
			}
//			if (t_inter == 76 && inter == 49)
//				int kk = 9;
			if (M_Pool == false && M_Interventions[inter].Get_RiskC(t_inter) < (M_Interventions[inter].Get_Min_Risk() + __FLT_EPSILON__))
				continue;
			//			for (int k = 0; k < M_N_Interventions && Cambio!=true; k++)
			//			{
			//					int i = M_Vec_Alea_I[k];
			//					int inter = i;
			//					int t_inter = M_I_T[inter];

			//				double fobjAntiguo1 = FuncionObjetivoTemp();
							//No está aquí asignda
//				if (M_Interventions[inter].Get_T_Temp() > t || M_Interventions[inter].Get_T_F_Temp() <= t)
//					continue;
			double fobj1 = M_Interventions[inter].Get_RiskC(t_inter);
			M_fo_temp -= fobj1;

			//Quito lo de la función objetvio
			for (int t = M_Interventions[inter].Get_T_Temp(); t < M_Interventions[inter].Get_T_F_Temp(); t++)
			{
				for (int si = 0; si < M_Scenarios_number[t]; si++)
				{
					M_T_Temp_Risk_S[t][si] -= M_Interventions[inter].Get_Risk()[t][t_inter][si];
				}
			}
			FuncionObjetivoTemp(M_Interventions[inter].Get_T_Temp(), M_Interventions[inter].Get_T_F_Temp());
			//Quito recursos
			QuitarRecursos(inter);
			//Recorro en orden de los iniciales
			int cont = 0;
			for (vector<int> ::iterator itt = M_Interventions[inter].Get_T_Ordenados_Risk().begin(); itt != M_Interventions[inter].Get_T_Ordenados_Risk().end() && Cambio != true && cont < get_random(6, M_N_Interventions); itt++, cont++)
			{
				if (t_inter == (*itt))
					continue;
				int timeini = (*itt);
				M_fo_temp += M_Interventions[inter].Get_RiskC(timeini);
				if ((M_Alpha * (M_fo_temp / (double)M_T)) > fobjAntiguo)
				{
					M_fo_temp -= M_Interventions[inter].Get_RiskC(timeini);
					continue;
				}
				if (M_Pool == false && M_Interventions[inter].Get_RiskC(timeini) > (M_Interventions[inter].Get_RiskC(t_inter) - __FLT_EPSILON__))
				{
					M_fo_temp -= M_Interventions[inter].Get_RiskC(timeini);
					continue;
				}
				if (PuedoColocar(inter, timeini))
				{

					int timefin = timeini + M_Interventions[inter].Get_Delta(timeini);
					for (int t3 = timeini; t3 < timefin; t3++)
					{
						for (int si = 0; si < M_Scenarios_number[t3]; si++)
						{
							M_T_Temp_Risk_S[t3][si] += M_Interventions[inter].Get_Risk()[t3][timeini][si];
						}
					}


					//						double fobjNew = FuncionObjetivoTemp();
					double fobjNew = FuncionObjetivoTemp(timeini, timefin);

					if (fobjNew < (fobjAntiguo - 0.0001))
					{
						//	(*it).second = timeini;
//							CambiarSolution(inter, timeini);
//						PonerAlFinalSolution(inter, timeini);
						M_I_T[inter] = timeini;
						M_I_Seasons[inter] = M_T_Season[timeini];
						M_Interventions[inter].Set_T_Temp(timeini);
						AddRecursos(inter);
						Cambio = true;
						ActualizarMejorSolution(fobjNew,3);
					}
					else
					{
						M_fo_temp -= M_Interventions[inter].Get_RiskC(timeini);
						for (int t3 = timeini; t3 < timefin; t3++)
						{
							for (int si = 0; si < M_Scenarios_number[t3]; si++)
							{
								M_T_Temp_Risk_S[t3][si] -= M_Interventions[inter].Get_Risk()[t3][timeini][si];
							}
						}
						FuncionObjetivoTemp(timeini, timefin);
					}


				}
				else
					M_fo_temp -= M_Interventions[inter].Get_RiskC(timeini);


			}
			if (Cambio != true)
			{
				AddRecursos(inter);
				M_fo_temp += fobj1;
				for (int tt0 = M_Interventions[inter].Get_T_Temp(); tt0 < M_Interventions[inter].Get_T_F_Temp(); tt0++)
				{
					for (int si = 0; si < M_Scenarios_number[tt0]; si++)
					{
						M_T_Temp_Risk_S[tt0][si] += M_Interventions[inter].Get_Risk()[tt0][t_inter][si];
					}
				}
				FuncionObjetivoTemp(M_Interventions[inter].Get_T_Temp(), M_Interventions[inter].Get_T_F_Temp());
			}

		}

	} while (Cambio == true);

	return false;
}
bool Maintenance::MejoraIntercambiosFo1OrderFast()
{
	bool Cambio = false;
	bool Algun_Cambio = false;
	do
	{
		Algun_Cambio = false;

		sort(M_Interventions_Dif.begin(), M_Interventions_Dif.end(),
			[&](size_t i, size_t j) { return (M_Interventions[i].Get_RiskC(M_I_T[i]) - M_Interventions[i].Get_Min_Risk()) > (M_Interventions[j].Get_RiskC(M_I_T[j]) - M_Interventions[j].Get_Min_Risk()); });
		/*		double kk2 = FuncionObjetivo();
				if (fobjAntiguo <(kk2 - __FLT_EPSILON__) || fobjAntiguo>(kk2 + __FLT_EPSILON__))
				{
					int kk = 9;
				}*/
				//Vamos probando para ir cambiando a donde se pueda mejorar
		//		for (int t0 = 0; t0 < M_T && Cambio != true && M_vfo2_temp[M_Ivfo2_temp[t0]]>0; t0++)
		for (int index = 0; index < M_N_Interventions; index++)
		{
			Cambio = false;
			double fobjAntiguo = FuncionObjetivoTemp();

			//		for (list<pair<int, int>>::iterator it = M_I_T_Solution.begin(); it != M_I_T_Solution.end() && Cambio != true; it++)
			//		{

				//		int inter = (*it).first;
				//		int t_inter = (*it).second;
			int inter = M_Interventions_Dif[index];
			int t_inter = M_I_T[inter];
//			if (t_inter == 76 && inter == 49)
//				int kk = 9;
			if (M_Pool == false && M_Interventions[inter].Get_RiskC(t_inter) < (M_Interventions[inter].Get_Min_Risk() + __FLT_EPSILON__))
				continue;
			//			for (int k = 0; k < M_N_Interventions && Cambio!=true; k++)
			//			{
			//					int i = M_Vec_Alea_I[k];
			//					int inter = i;
			//					int t_inter = M_I_T[inter];

			//				double fobjAntiguo1 = FuncionObjetivoTemp();
							//No está aquí asignda
//				if (M_Interventions[inter].Get_T_Temp() > t || M_Interventions[inter].Get_T_F_Temp() <= t)
//					continue;
			double fobj1 = M_Interventions[inter].Get_RiskC(t_inter);
			M_fo_temp -= fobj1;

			//Quito lo de la función objetvio
			for (int t = M_Interventions[inter].Get_T_Temp(); t < M_Interventions[inter].Get_T_F_Temp(); t++)
			{
				for (int si = 0; si < M_Scenarios_number[t]; si++)
				{
					M_T_Temp_Risk_S[t][si] -= M_Interventions[inter].Get_Risk()[t][t_inter][si];
				}
			}
			FuncionObjetivoTemp(M_Interventions[inter].Get_T_Temp(), M_Interventions[inter].Get_T_F_Temp());
			//Quito recursos
			QuitarRecursos(inter);
			//Recorro en orden de los iniciales
			int cont = 0;
			for (vector<int> ::iterator itt = M_Interventions[inter].Get_T_Ordenados_Risk().begin(); itt != M_Interventions[inter].Get_T_Ordenados_Risk().end() && Cambio != true && cont < get_random(6, M_N_Interventions); itt++, cont++)
			{
				if (t_inter == (*itt))
					continue;
				int timeini = (*itt);
				M_fo_temp += M_Interventions[inter].Get_RiskC(timeini);
				if ((M_Alpha * (M_fo_temp / (double)M_T)) > fobjAntiguo)
				{
					M_fo_temp -= M_Interventions[inter].Get_RiskC(timeini);
					continue;
				}
				if (M_Pool == false && M_Interventions[inter].Get_RiskC(timeini) > (M_Interventions[inter].Get_RiskC(t_inter) - __FLT_EPSILON__))
				{
					M_fo_temp -= M_Interventions[inter].Get_RiskC(timeini);
					continue;
				}
				if (PuedoColocar(inter, timeini))
				{

					int timefin = timeini + M_Interventions[inter].Get_Delta(timeini);
					for (int t3 = timeini; t3 < timefin; t3++)
					{
						for (int si = 0; si < M_Scenarios_number[t3]; si++)
						{
							M_T_Temp_Risk_S[t3][si] += M_Interventions[inter].Get_Risk()[t3][timeini][si];
						}
					}


					//						double fobjNew = FuncionObjetivoTemp();
					double fobjNew = FuncionObjetivoTemp(timeini, timefin);

					if (fobjNew < (fobjAntiguo - 0.0001))
					{
						//	(*it).second = timeini;
//							CambiarSolution(inter, timeini);
//						PonerAlFinalSolution(inter, timeini);
						M_I_T[inter] = timeini;
						M_I_Seasons[inter] = M_T_Season[timeini];
						M_Interventions[inter].Set_T_Temp(timeini);
						AddRecursos(inter);
						Cambio = true;
						Algun_Cambio = true;
						ActualizarMejorSolution(fobjNew,14);
					}
					else
					{
						M_fo_temp -= M_Interventions[inter].Get_RiskC(timeini);
						for (int t3 = timeini; t3 < timefin; t3++)
						{
							for (int si = 0; si < M_Scenarios_number[t3]; si++)
							{
								M_T_Temp_Risk_S[t3][si] -= M_Interventions[inter].Get_Risk()[t3][timeini][si];
							}
						}
						FuncionObjetivoTemp(timeini, timefin);
					}


				}
				else
					M_fo_temp -= M_Interventions[inter].Get_RiskC(timeini);


			}
			if (Cambio != true)
			{
				AddRecursos(inter);
				M_fo_temp += fobj1;
				for (int tt0 = M_Interventions[inter].Get_T_Temp(); tt0 < M_Interventions[inter].Get_T_F_Temp(); tt0++)
				{
					for (int si = 0; si < M_Scenarios_number[tt0]; si++)
					{
						M_T_Temp_Risk_S[tt0][si] += M_Interventions[inter].Get_Risk()[tt0][t_inter][si];
					}
				}
				FuncionObjetivoTemp(M_Interventions[inter].Get_T_Temp(), M_Interventions[inter].Get_T_F_Temp());
			}

		}

	} while (Algun_Cambio == true);

	return false;
}
inline void Maintenance::SubtractScenarios(int Nint)
{
	int tini = M_Interventions[Nint].Get_T_Temp();
	for (register int t = M_Interventions[Nint].Get_T_Temp(); t < M_Interventions[Nint].Get_T_F_Temp(); t++)
	{
		for (register int si = 0; si < M_Scenarios_number[t]; si++)
		{
			M_T_Temp_Risk_S[t][si] -= M_Interventions[Nint].Get_RiskN(t,tini,si);
		}
	}
}
void Maintenance::VerificarRiskMap()
{
	for (int i = 0; i < M_N_Interventions; i++)
	{
		int Nint = i;
		for (register int tini = 0; tini < M_Interventions[i].Get_Tmax(); tini++)
		{

			for (register int t = tini; t < tini+M_Interventions[i].Get_Delta(tini); t++)
			{
				for (register int si = 0; si < M_Scenarios_number[t]; si++)
				{
					if ((M_Interventions[Nint].Get_Risk(t, tini, si) < (M_Interventions[Nint].Get_RiskN(t, tini, si) - __FLT_EPSILON__))
						|| (M_Interventions[Nint].Get_Risk(t, tini, si) > (M_Interventions[Nint].Get_RiskN(t, tini, si) + __FLT_EPSILON__)))
						Problema(1111);
					//				M_T_Temp_Risk_S[t][si] += M_Interventions[Nint].Get_Risk(t, tini, si);
				}
			}
		}
	}
	/*
	for (int i = 0; i < M_N_Resources; i++)
	{
		int Nint = i;
		for (register int tini = 0; tini < M_Interventions[i].Get_Tmax(); tini++)
		{

			for (register int t = tini; t < tini + M_Interventions[i].Get_Delta(tini); t++)
			{

				if ((M_Interventions[Nint].Get_Workload(t, tini, si) < (M_Interventions[Nint].Get_WorkloadN(t, tini, si) - __FLT_EPSILON__))
						|| (M_Interventions[Nint].Get_Workload(t, tini, si) > (M_Interventions[Nint].Get_WorkloadN(t, tini, si) + __FLT_EPSILON__)))
						Problema(1111);
					//				M_T_Temp_Risk_S[t][si] += M_Interventions[Nint].Get_Risk(t, tini, si);

			}
		}
	}*/
}
inline void Maintenance::AddScenarios(int Nint)
{
	int tini=M_Interventions[Nint].Get_T_Temp();
	for (register int t = M_Interventions[Nint].Get_T_Temp(); t < M_Interventions[Nint].Get_T_F_Temp(); t++)
	{
		for (register int si = 0; si < M_Scenarios_number[t]; si++)
		{
			M_T_Temp_Risk_S[t][si] += M_Interventions[Nint].Get_RiskN(t,tini,si);
		}
	}
}
inline void Maintenance::SubtractScenarios(int Nint,int t_ini, int t_fin)
{

	for (register int t = t_ini; t < t_fin; t++)
	{
		for (register int si = 0; si < M_Scenarios_number[t]; si++)
		{
			M_T_Temp_Risk_S[t][si] -= M_Interventions[Nint].Get_RiskN(t,t_ini,si);
		}
	}
}
inline void Maintenance::AddScenarios(int Nint,int t_ini, int t_fin)
{
	for (register int t = t_ini; t < t_fin; t++)
	{
		for (register int si = 0; si < M_Scenarios_number[t]; si++)
		{
			M_T_Temp_Risk_S[t][si] += M_Interventions[Nint].Get_RiskN(t,t_ini,si);
		}
	}
}
double Maintenance::MejoraIntercambiosDosI(double val)
{
	bool Cambio = false;
	bool Algun_Cambio = false;
	double fobjAntiguo = val;
//	if (M_Iter == 197)
//		int kk = 9;
//	double kk2 = FuncionObjetivo();

//	if (val <(kk2 - __FLT_EPSILON__) || val>(kk2 + __FLT_EPSILON__))
//	{
//		double kk3 = FuncionObjetivoTemp();
//	}
//	ActualizarMejorSolution(val, 4);

	do
	{
		Cambio = false;
		Algun_Cambio = false;
		bool alguno_nuevo=CompararDosSumasT(M_T_M2, M_T_M2_Suma, M_BestT_Suma);
//		bool alguno_nuevo = true;
		M_T_M2_Suma = M_BestT_Suma;
		if (alguno_nuevo == false)
			return fobjAntiguo;
//		for (int ki = 0; ki < M_T; ki++) M_T_M1[ki] = true;
		/*		double kk2 = FuncionObjetivo();
				if (fobjAntiguo <(kk2 - __FLT_EPSILON__) || fobjAntiguo>(kk2 + __FLT_EPSILON__))
				{
					int kk = 9;
				}*/
				//Vamos probando para ir cambiando a donde se pueda mejorar
		//		for (int t0 = 0; t0 < M_T && Cambio != true && M_vfo2_temp[M_Ivfo2_temp[t0]]>0; t0++)
//		if (get_random(0,1)==0)
		if (M_Pool == false || get_random(0, 1) == 0)
		{
			//if (get_random(0, 4) >= 1)
			//	sort(M_Interventions_Dif.begin(), M_Interventions_Dif.end(),
			//		[&](size_t i, size_t j)
			//{ if ((M_Interventions[i].Get_RiskC(M_I_T[i]) - M_Interventions[i].Get_Min_Risk()) > (M_Interventions[j].Get_RiskC(M_I_T[j]) - M_Interventions[j].Get_Min_Risk()))
			//		return true;
			//else
			//		return (M_Interventions[i].Get_Min_Risk() > M_Interventions[j].Get_Min_Risk());
			//});
			//else
				random_shuffle(M_Interventions_Dif.begin(), M_Interventions_Dif.end());
		}
		else
		{
			ComputeExcessTemp();
			sort(M_Interventions_Dif.begin(), M_Interventions_Dif.end(), [&](size_t i, size_t j) { return ((M_Interventions[i].Get_Excess_temp()) > (M_Interventions[j].Get_Excess_temp())); });
		}
		struct timeb t1;
		ftime(&t1);
		double	time_v1 = ((double)((t1.time - M_Time_Initial.time) * 1000 + t1.millitm - M_Time_Initial.millitm)) / 1000;

		for (int index = 0; index < ((M_N_Interventions - 1)) && (Algun_Cambio != true || M_Busqueda_Local) && time_v1 < M_Total_Time; index++)
		{
			Cambio = false;
			int inter = M_Interventions_Dif[index];
			int t_inter = M_I_T[inter];
//			int t_interf = M_Interventions[inter].Get_T_F_Temp();
			if (M_Pool == false && M_Interventions[inter].Get_RiskC(t_inter) < (M_Interventions[inter].Get_Min_Risk() + __FLT_EPSILON__))
				continue;
			if (!PuedoQuitar(inter, t_inter)) continue;
			double fobj1 = M_Interventions[inter].Get_RiskC(t_inter);
			M_fo_temp -= fobj1;
			//Quito lo de la función objetvio
			SubtractScenarios(inter);
			FuncionObjetivoTemp(M_Interventions[inter].Get_T_Temp(), M_Interventions[inter].Get_T_F_Temp());
			//Quito recursos
			QuitarRecursos(inter);
			M_I_T[inter] = -1;
			M_Interventions_Colocada[inter] = false;
			bool CambiaTOrigen = HaCambiadoT(t_inter, M_Interventions[inter].Get_Delta(t_inter), M_T_M2);

			//			cout << "Quito " << inter << " Time " << t_inter << endl;
			for (int index2 = index + 1; index2 < M_N_Interventions && Cambio != true && time_v1<M_Total_Time; index2++)
			{
				int inter2 = M_Interventions_Dif[index2];
//				if (index == 3 && index2 == 74)
//					int kk = 9;
				int t_inter2 = M_I_T[inter2];
//				int t_inter2f = M_Interventions[inter2].Get_T_F_Temp();
				if (CambiaTOrigen == false && HaCambiadoT(t_inter2, M_Interventions[inter2].Get_Delta(t_inter2), M_T_M2) == false)
					continue;
				int timeini2 = t_inter;
				int timeini = t_inter2;
				if (!PuedoQuitar(inter2, t_inter2)) continue;
				if (t_inter == t_inter2)
					continue;
				if (find(M_Interventions[inter].Get_T_Ordenados_Risk().begin(), M_Interventions[inter].Get_T_Ordenados_Risk().end(), timeini) == M_Interventions[inter].Get_T_Ordenados_Risk().end())
					continue;
				if (find(M_Interventions[inter2].Get_T_Ordenados_Risk().begin(), M_Interventions[inter2].Get_T_Ordenados_Risk().end(), timeini2) == M_Interventions[inter2].Get_T_Ordenados_Risk().end())
					continue;
				double fobj2 = M_Interventions[inter2].Get_RiskC(t_inter2);
				M_fo_temp -= fobj2;
				M_fo_temp += M_Interventions[inter].Get_RiskC(timeini);
				M_fo_temp += M_Interventions[inter2].Get_RiskC(timeini2);
				if ((M_Alpha * (M_fo_temp / (double)M_T)) > (fobjAntiguo - __FLT_EPSILON__ - M_Min_Improve))
				{

					M_fo_temp -= M_Interventions[inter2].Get_RiskC(timeini2);
					M_fo_temp -= M_Interventions[inter].Get_RiskC(timeini);
					M_fo_temp += fobj2;
					continue;
				}
				//Quito lo de la función objetvio
				SubtractScenarios(inter2);
				FuncionObjetivoTemp(M_Interventions[inter2].Get_T_Temp(), M_Interventions[inter2].Get_T_F_Temp());
				//Quito recursos
				QuitarRecursos(inter2);
				M_I_T[inter2] = -1;
				M_Interventions_Colocada[inter2] = false;
				if (PuedoColocar(inter2, timeini2))
				{
					int timefin2 = timeini2 + M_Interventions[inter2].Get_Delta(timeini2);
					AddScenarios(inter2, timeini2, timefin2);
					if (M_Pool) CopyFO2_2();
					FuncionObjetivoTemp(timeini2, timefin2);
					//Falta añadir aquí recursos del anterior para poder ver si entra o no la otra
					M_Interventions[inter2].Set_T_Temp(timeini2);
					AddRecursos(inter2);
					M_I_T[inter2] = timeini2;
					M_I_Seasons[inter2] = M_T_Season[timeini2];
					M_Interventions_Colocada[inter2] = true;
					int cont = 0;
					if (PuedoColocar(inter, timeini))
					{
						cont++;
						int timefin = timeini + M_Interventions[inter].Get_Delta(timeini);
						AddScenarios(inter, timeini, timefin);
						if (M_Pool) CopyFO2();
						double fobjNew = FuncionObjetivoTemp(timeini, timefin);
						if (fobjNew < (fobjAntiguo -  M_Min_Improve) || (fobjNew < (M_Best_fo - __FLT_EPSILON__)))
//						if (fobjNew < (fobjAntiguo - __FLT_EPSILON__))
						{
//							PonerAlFinalSolution(inter, timeini);
							M_I_T[inter] = timeini;
							M_I_Seasons[inter] = M_T_Season[timeini];
							M_Interventions[inter].Set_T_Temp(timeini);
							M_Interventions_Colocada[inter] = true;
							AddRecursos(inter);
//							PonerAlFinalSolution(inter2, timeini2);
							fobjAntiguo = fobjNew;
							Cambio = true;
							Algun_Cambio = true;
							CalcularSumasT(M_BestT_Suma);
							ActualizarMejorSolution(fobjNew, 4);
							CompararDosSumasT(M_T_M2, M_T_M2_Suma, M_BestT_Suma);
						}
						else
						{
							SubtractScenarios(inter, timeini, timefin);
							if (M_Pool) CopyBackFO2();
							//FuncionObjetivoTemp(timeini, timefin);
						}
					}
					if (Cambio != true)
					{
						SubtractScenarios(inter2, timeini2, timefin2);
						if (M_Pool) CopyBackFO2_2();
//						FuncionObjetivoTemp(timeini2, timefin2);
						QuitarRecursos(inter2);
						M_Interventions[inter2].Set_T_Temp(t_inter2);
						M_Interventions_Colocada[inter2] = true;
						M_I_T[inter2] = t_inter2;
						M_I_Seasons[inter2] = M_T_Season[t_inter2];
					}
				}
				if (Cambio != true)
				{
					M_Interventions[inter2].Set_T_Temp(t_inter2);
					M_fo_temp += fobj2;
					M_fo_temp -= M_Interventions[inter2].Get_RiskC(timeini2);
					M_fo_temp -= M_Interventions[inter].Get_RiskC(timeini);
					M_Interventions_Colocada[inter2] = true;
					M_I_T[inter2] = t_inter2;
					AddRecursos(inter2);
					AddScenarios(inter2);
					
					FuncionObjetivoTemp(M_Interventions[inter2].Get_T_Temp(), M_Interventions[inter2].Get_T_F_Temp());
				}
				//				VerifySolution();
			}
			ftime(&t1);
			time_v1 = ((double)((t1.time - M_Time_Initial.time) * 1000 + t1.millitm - M_Time_Initial.millitm)) / 1000;

			if (Cambio != true)
			{
				M_Interventions[inter].Set_T_Temp(t_inter);
				M_fo_temp += fobj1;
				M_Interventions_Colocada[inter] = true;
				M_I_T[inter] = t_inter;
				AddRecursos(inter);
				AddScenarios(inter);
				FuncionObjetivoTemp(M_Interventions[inter].Get_T_Temp(), M_Interventions[inter].Get_T_F_Temp());
			}
		}

	}while (Algun_Cambio == true && M_Busqueda_Local);



return fobjAntiguo;
}
void Maintenance::Insertar(list<pair<vector<int>, double>>& lista)
{

	for (auto it = lista.begin(); it != lista.end(); it++)
	{
		InsertarOrdenadoPool((*it).second, (*it).first, M_Pool_Solutions_Integer);
	}
}
void Maintenance::CrearListasFormulations()
{
	struct timeb t0;
	ftime(&t0);
	double time = 0;
//	return;
//	M_Time_Modelo = min(1200, (M_Total_Time-(int)M_Time_Leer) / 2) / 3;
	M_Time_Modelo = M_Total_Time / 6;//6
//	M_Time_Modelo = 20;
	M_Pool_Solutions = M_Pool;
//	M_Time_Modelo = 0;
//	if (M_N_Interventions>500)
//	M_Time_Modelo = M_Time_Modelo * 3;
	int tiempo_ini_modelo = M_Time_Modelo;
	if (M_Pool == false || M_Lower_Resources)
		M_Time_Modelo = M_Time_Modelo * 2;
	//	M_Mixed_Risk = false;
	M_Formulacion_Cuantiles = false;
	M_Max_Formulation = false;
//	M_GapPool = 10;

	if (M_Pool==false)
	IntegerFormulation(1);
	struct timeb t1;
	struct timeb t2;
	ftime(&t2);
	time = ((double)((t2.time - M_Time_Initial.time) * 1000 + t2.millitm - M_Time_Initial.millitm)) / 1000;

	if (M_Pool != false && M_Time_Modelo>15 && time<(2*M_Total_Time / 3) )
//	if (M_Pool != false )
	{
		//		M_Mixed_Risk = false;
		M_Formulacion_Cuantiles = true;
		M_Max_Formulation = false;
//		ftime(&t1);
//		time = ((double)((t1.time - t0.time) * 1000 + t1.millitm - t0.millitm)) / 1000;
		//Si no ha conseguido nada antes 

		M_Time_Modelo = tiempo_ini_modelo;
		//Quito 22062021
//		M_Pool_Solutions_Integer.clear();
//		if (M_Pool_Solutions_Integer.size() == 0) M_Time_Modelo = 2 * M_Time_Modelo;
		M_Time_Modelo = 2 * M_Time_Modelo;
		//esto es para que no está más de la mitad del tiempo aquí
//		M_Time_Modelo = min((M_Total_Time / 2) - time, (double)M_Time_Modelo);
		
//		M_GapPool = get_random(1,5);
		IntegerFormulation(2);
			ftime(&t2);
			time = ((double)((t2.time - M_Time_Initial.time) * 1000 + t2.millitm - M_Time_Initial.millitm)) / 1000;

/*		if (M_Pool_Solutions_Integer.size() < (4*12 * M_Iter) && time<M_Total_Time/3)
		 {
			M_GapPool = 200;
			M_Formulacion_Cuantiles = false;
			IntegerFormulation(1);
			M_Formulacion_Cuantiles = true;
			M_GapPool = 10;
		}
	*/		
		//Formulacion max


//		ftime(&t2);
//		time = ((double)((t2.time - t1.time) * 1000 + t2.millitm - t1.millitm)) / 1000;
		
//				M_Mixed_Risk = true;
	//	M_Time_Modelo = tiempo_ini_modelo + (tiempo_ini_modelo - time);
/*
		M_Formulacion_Cuantiles = false;
		M_Max_Formulation = true;
		M_GapPool = 1000;
		M_Time_Modelo = tiempo_ini_modelo ;

		ftime(&t2);
		time = ((double)((t2.time - M_Time_Initial.time) * 1000 + t2.millitm - M_Time_Initial.millitm)) / 1000;
		if (time < (M_Total_Time / 2))
		{
			IntegerFormulation(4);
		}*/
	}
//	return;
	M_Encuentra_Solutions = false;
	if (M_Pool_Solutions_Integer.size()+ M_Pool_Solutions_IntegerQuantiles.size() == 0)
		return;
	M_Encuentra_Solutions = true;
	ftime(&t1);
	time = ((double)((t1.time - M_Time_Initial.time) * 1000 + t1.millitm - M_Time_Initial.millitm)) / 1000;

	//	if (M_Pool == false)
	//		return;
	M_Iter = 0;
	/*
	if (M_Pool_Solutions_IntegerQuantiles.size() > 0)
	{
		printf("Aqui 25");
		vector<vector<int>> temp(M_Pool_Solutions_IntegerQuantiles.begin(), M_Pool_Solutions_IntegerQuantiles.end());
		random_shuffle(temp.begin(), temp.end());
		printf("Aqui 26");
		for (vector<vector<int>>::iterator itv = temp.begin(); itv != temp.end(); itv++)
			M_Pool_Solutions_IntegerQuantiles.push_back(*itv);
	}
	if (M_Pool_Solutions_IntegerMax.size() > 0)
	{
		printf("Aqui 27");
		vector<vector<int>> temp2(M_Pool_Solutions_IntegerMax.begin(), M_Pool_Solutions_IntegerMax.end());

		random_shuffle(temp2.begin(), temp2.end());
		printf("Aqui 28");
		for (vector<vector<int>>::iterator itv = temp2.begin(); itv != temp2.end(); itv++)
			M_Pool_Solutions_IntegerMax.push_back(*itv);
	}*/
//	random_shuffle(M_Pool_Solutions_IntegerQuantiles.begin(), M_Pool_Solutions_IntegerQuantiles.end());
//	random_shuffle(M_Pool_Solutions_IntegerMax.begin(), M_Pool_Solutions_IntegerMax.end());

	list<pair<vector<int>, double>> ::reverse_iterator it = M_Pool_Solutions_Integer.rbegin();
	list<pair<vector<int>, double>> ::reverse_iterator it2 = M_Pool_Solutions_IntegerQuantiles.rbegin();
	list<pair<vector<int>, double>> ::reverse_iterator it3 = M_Pool_Solutions_IntegerMax.rbegin();
	printf("\n Lista Primera %zd\n", M_Pool_Solutions_Integer.size());
	printf("Lista Cuantiles %zd\n", M_Pool_Solutions_IntegerQuantiles.size());

	printf("Lista Max %zd\n", M_Pool_Solutions_IntegerMax.size());

	ftime(&t0);
	vector<list<pair<vector<int>,double>>> Lista(M_Number_Threads);
	M_Pool_Solutions_Lista = Lista;

//	M_Pool_Solutions_Lista.push_back();
	bool alguna = false;
	int lista = 0;
	do
	{
		
		alguna = false;
		if (it != M_Pool_Solutions_Integer.rend() && time < M_Total_Time)
		{
			alguna = true;
			M_Pool_Solutions_Lista[lista].push_back((*it));

			it++;
			lista++;
		}
		if (lista == M_Number_Threads) lista = 0;
		if (it2 != M_Pool_Solutions_IntegerQuantiles.rend() && time < M_Total_Time)
		{
			alguna = true;
			M_Pool_Solutions_Lista[lista].push_back((*it2));
			it2++;
			lista++;
		}
		if (lista == M_Number_Threads) lista = 0;
		if (it3 != M_Pool_Solutions_IntegerMax.rend() && time < M_Total_Time)
		{
			alguna = true;
			M_Pool_Solutions_Lista[lista].push_back((*it3));


			it3++;
			lista++;
		}

		if (lista ==M_Number_Threads) lista = 0;





	} while (alguna == true );
	M_Pool_Solutions_Integer = M_Pool_Solutions_Lista[0];
	//Si le queda tiempo
//	if (time < Tiempo)
//		GRASP();

}
void Maintenance::PruebaHash()
{

	//(8, 2, 15) y(3, 14, 15) ?
	vector<int> p;
	p.push_back(8);
	p.push_back(2);
	p.push_back(15);
//	int kk = hash_operator(p);
	vector<int> p2;
	p2.push_back(3);
	p2.push_back(14);
	p2.push_back(15);
//	int kk2 = hash_operator(p2);


}
void Maintenance::PoolGoodSolutions(vector<int> &solution, double value)
{
//	if (M_Fase_Intensification)
//		return;
	if (value< M_MinPoolSolutions)
	{

		if (M_PoolGoodSolutions.size() == 0)
		{
			M_PoolGoodSolutionsD.insert(hash_operator(solution)).second;

			pair<vector<int>, double> p(solution, value);
			M_PoolGoodSolutions.push_back(p);
			return;
		}

		bool metido=M_PoolGoodSolutionsD.insert(hash_operator(solution)).second;
		if (metido == false)
			return;

		for (list<pair<vector<int>,double>> ::iterator it=M_PoolGoodSolutions.begin(); it!=M_PoolGoodSolutions.end();it++)
		{
			if ((value ) < ((*it).second-__FLT_EPSILON__))
			{
				M_PoolGoodSolutionsD.insert(hash_operator(solution));
				pair<vector<int>, double> p(solution, value);


				M_PoolGoodSolutions.insert(it, p);
				if (M_PoolGoodSolutions.size() > M_SizePoolSolutions)
				{

					
					M_PoolGoodSolutionsD.erase(hash_operator(M_PoolGoodSolutions.back().first));
					M_PoolGoodSolutions.pop_back();
					M_MinPoolSolutions = M_PoolGoodSolutions.back().second;
				}
				return;
			}
		}
		if (M_PoolGoodSolutions.size() < M_SizePoolSolutions)
		{
			M_PoolGoodSolutionsD.insert(hash_operator(solution));
			pair<vector<int>, double> p(solution, value);


			M_PoolGoodSolutions.push_back(p);
			M_MinPoolSolutions = M_PoolGoodSolutions.back().second;
		}
	}


}
void Maintenance::QuitarNoVisitadas(set<int> &vec)
{
	for (auto it = vec.begin(); it != vec.end(); it++)
	{
		M_Integer_Sol_Distintas.erase(*it);

	}
}
void Maintenance::MejorarPoolSolutions2(int hilo)
{
//	return;
//	list<vector<int>> Lista_Solutions=;
	M_No_Visitadas.clear();
	list<pair<vector<int>,double>> ::iterator it;
	struct timeb t1;
	struct timeb t0;
	double time=0;
//	return;
	bool alguna = false;
	if (M_Pool_Solutions_Integer.size() > 0)
		M_Encuentra_Solutions = true;
	if (M_Encuentra_Solutions)
		it =M_Pool_Solutions_Integer.begin();
	else
		goto G_r_a_s_p;


	M_Intensification = true;
	if (M_Intensification)
	{
		M_Time_Intensification = M_Total_Time / 8;

//		if (M_Iter)
//		M_Intensification = 0;
		int PoolSize = M_Pool_Solutions_Integer.size();
		M_SizePoolSolutions = max(PoolSize/8,20);
		if (M_Lower_Resources)
			M_Time_Intensification = M_Time_Intensification / 4;
	}
	else
		M_Time_Intensification = 0;
	M_Iter = 0;
	M_Total_Time = M_Total_Time - M_Time_Intensification;
			M_VMejora1 = 0; M_VMejora2 = 0; M_VMejora3 = 0; M_VMejora4 = 0; M_VMejora5 = 0;
		M_Mejora1 = 0; M_Mejora2 = 0; M_Mejora3 = 0; M_Mejora4 = 0; M_Mejora5 = 0;
	        M_distintos.clear();
	do
	{
		alguna = false;
		
		if (it != M_Pool_Solutions_Integer.end() && time < M_Total_Time - M_Time_Intensification)
		{
			alguna = true;
			M_fo_temp = 0;

			LimpiarVectores();
			LimpiarRiskTempS();

			for (int i = 0; i < M_N_Interventions; i++)
			{
				M_I_T[i] = (*it).first[i];
//				pair<int, int> p(i, (*it)[i]);
				M_Interventions[i].Set_T_Temp((*it).first[i]);
//				M_I_T_Solution.push_back(p);
				M_Interventions_Colocada[i] = true;
				M_NI_Colocadas++;
				AddScenarios(i);
				AddRecursos(i);
				M_fo_temp += M_Interventions[i].Get_RiskC((*it).first[i]);

			}

			double val = FuncionObjetivoTemp();
//			double val_ini = val;
			ActualizarMejorSolution(val, 100);
			if (val < M_fo_formulation2)
				M_fo_formulation2=val;
			VerifySolution();
			ftime(&t0);
			time = ((double)((t0.time - M_Time_Initial.time) * 1000 + t0.millitm - M_Time_Initial.millitm)) / 1000;
	//		if (!M_Challenge_Mode) printf("Hilo1 %d %.3f", hilo, time);
			double val2 = 0;

				if ( get_random(0, 1) == 0)
					VND(val);
				else
					VND2(val);


//			if (M_Intensification)
				PoolGoodSolutions(M_I_T,val);
			ftime(&t1);

			time = ((double)((t1.time - M_Time_Initial.time) * 1000 + t1.millitm - M_Time_Initial.millitm)) / 1000;
	//		if (!M_Challenge_Mode) printf("Hilo1 %d %.3f", hilo, time);

			M_Iter++;
	//		it++;

			it = M_Pool_Solutions_Integer.erase(it);
		}






	} while (alguna == true && (time < M_Total_Time));
	//Tengo que borrar de M_Integer_Sol_Distintas las que no me haya dado tiempo a recorre
/*	if (M_Encuentra_Solutions)
	{
		//Las que no he revisado, las quito porque no me ha dado tiempo y quizás venga bien revisarlas
		for (; it != M_Pool_Solutions_Integer.end(); it++)
		{
			M_No_Visitadas.insert(hash_operator((*it).first));
		}
	}*/
	M_Total_Time = M_Total_Time + M_Time_Intensification;
	if (M_Intensification)
	{
		set<int> Temp = M_distintos;

		M_Fase_Intensification = true;
		M_VMejora1 = 0; M_VMejora2 = 0; M_VMejora3 = 0; M_VMejora4 = 0; M_VMejora5 = 0;
		M_Mejora1 = 0; M_Mejora2 = 0; M_Mejora3 = 0; M_Mejora4 = 0; M_Mejora5 = 0;
		int tempIter = M_Iter;
		M_Iter = 0;
		int cont = 0;
		do
		{
			list<pair<vector<int>, double>> tempGS = M_PoolGoodSolutions;
			cont++;
			M_distintos.clear();
			if (!M_Challenge_Mode) cout <<"Best Solution "<<M_Best_fo << "Intensification " << M_PoolGoodSolutions.size() << " Tiempo " << time << "Hilo " << hilo << "Iter " << M_Iter << endl;
			for (list<pair<vector<int>, double>>::iterator itGS = tempGS.begin(); itGS != tempGS.end() && time < M_Total_Time ; itGS++)
			{
				M_fo_temp = 0;


				LimpiarVectores();
				LimpiarRiskTempS();

				for (int i = 0; i < M_N_Interventions; i++)
				{
					M_I_T[i] = (*itGS).first[i];

					M_Interventions[i].Set_T_Temp((*itGS).first[i]);
					M_NI_Colocadas++;
					M_Interventions_Colocada[i] = true;
					AddScenarios(i);
					AddRecursos(i);
					M_fo_temp += M_Interventions[i].Get_RiskC((*itGS).first[i]);

				}

				double val = FuncionObjetivoTemp();
				ActualizarMejorSolution(val, 101);
				VerifySolution();
				ftime(&t0);
//				double val2=VNS(val);

				if (get_random(0, 1) == 0)
					VND(val);
				else
					VND2(val);
//				if (cuenta>5)
//				val = FuncionObjetivoTemp();
//				double val2=VNS(val);
//				if (M_Intensification)
//					PoolGoodSolutions(M_I_T, val);

				ftime(&t1);

				time = ((double)((t1.time - M_Time_Initial.time) * 1000 + t1.millitm - M_Time_Initial.millitm)) / 1000;
				M_Iter++;
				it++;
			}
		}while (cont<20 && time < M_Total_Time);
		M_Iter = M_Iter + tempIter;
			M_Fase_Intensification = true;
					
		M_distintos.insert(Temp.begin(), Temp.end());
	}
	time = ((double)((t1.time - M_Time_Initial.time) * 1000 + t1.millitm - M_Time_Initial.millitm)) / 1000;
	M_Fase_Intensification = false;
	//
G_r_a_s_p:
	 cout << "Hilo "<< M_Hilo<< " GraspF " << M_Iter <<" Tiempo "<< time<< endl;
	//Si le queda tiempo
	if (time < M_Total_Time && M_Lower_Resources==false)
		GRASP();

}
void Maintenance::MejorarPoolSolutions()
{
	struct timeb t0;
	ftime(&t0);
	double time = 0;
//	GRASP();
	M_Time_Modelo = min(100,M_Total_Time/2)/3;
	if (M_Pool == false)
		M_Time_Modelo = M_Time_Modelo * 2;
	int tiempo_ini_modelo = M_Time_Modelo;
//	M_Mixed_Risk = false;
	M_Formulacion_Cuantiles = false;
	M_Max_Formulation = false;
//	IntegerFormulation(1);
	struct timeb t1;

	ftime(&t1);
	time = ((double)((t1.time - t0.time) * 1000 + t1.millitm - t0.millitm)) / 1000;

	if (M_Pool != false)
	{
//		M_Mixed_Risk = false;
		M_Formulacion_Cuantiles = true;
		M_Max_Formulation = false;
		M_Time_Modelo = tiempo_ini_modelo+ (tiempo_ini_modelo - time);


		IntegerFormulation(2);
		/*
		//Formulacion max
		ftime(&t2);
		time = ((double)((t2.time - t1.time) * 1000 + t2.millitm - t1.millitm)) / 1000;

//		M_Mixed_Risk = false;
		M_Formulacion_Cuantiles = false;
		M_Max_Formulation = true;
		M_Time_Modelo = tiempo_ini_modelo ;

		IntegerFormulation(3);*/
	}

	ftime(&t1);
	time = ((double)((t1.time - M_Time_Initial.time) * 1000 + t1.millitm - M_Time_Initial.millitm)) / 1000;
	double Tiempo = M_Total_Time - time;
//	if (M_Pool == false)
//		return;
	M_Iter = 0;
	list<pair<vector<int>,double>> ::reverse_iterator it = M_Pool_Solutions_Integer.rbegin();
	list<pair<vector<int>, double>> ::iterator it2 = M_Pool_Solutions_IntegerQuantiles.begin();
	list<pair<vector<int>, double>> ::iterator it3 = M_Pool_Solutions_IntegerMax.begin();
	printf("\n Lista Primera %zd\n", M_Pool_Solutions_Integer.size());
	printf("Lista Cuantiles %zd\n", M_Pool_Solutions_IntegerQuantiles.size());

	printf("Lista Max %zd\n", M_Pool_Solutions_IntegerMax.size());

	ftime(&t0);
	
	
	bool alguna = false;
	do
	{
		alguna = false;
		if (it != M_Pool_Solutions_Integer.rend() && time < M_Total_Time)
		{
			alguna = true;
			M_fo_temp = 0;

			LimpiarVectores();
			LimpiarRiskTempS();

			for (int i = 0; i < M_N_Interventions; i++)
			{
				M_I_T[i] = (*it).first[i];

				M_Interventions[i].Set_T_Temp((*it).first[i]);
				M_NI_Colocadas++;
				M_Interventions_Colocada[i] = true;
				AddScenarios(i);
				AddRecursos(i);
				M_fo_temp += M_Interventions[i].Get_RiskC((*it).first[i]);

			}

			double val = FuncionObjetivoTemp();
			ActualizarMejorSolution(val, 102);
			VerifySolution();
			VND(val);

			ftime(&t1);

			time = ((double)((t1.time - M_Time_Initial.time) * 1000 + t1.millitm - M_Time_Initial.millitm)) / 1000;
			M_Iter++;
			it++;
		}

		if (it2 != M_Pool_Solutions_IntegerQuantiles.end() && time < M_Total_Time)
		{
			alguna = true;
			M_fo_temp = 0;


			LimpiarVectores();
			LimpiarRiskTempS();

			for (int i = 0; i < M_N_Interventions; i++)
			{
				M_I_T[i] = (*it2).first[i];

				M_Interventions[i].Set_T_Temp((*it2).first[i]);

				M_Interventions_Colocada[i] = true;
				AddScenarios(i);
				AddRecursos(i);
				M_fo_temp += M_Interventions[i].Get_RiskC((*it2).first[i]);

			}

			double val = FuncionObjetivoTemp();
			ActualizarMejorSolution(val, 103);
			VerifySolution();
			VND(val);

			ftime(&t1);

			time = ((double)((t1.time - M_Time_Initial.time) * 1000 + t1.millitm - M_Time_Initial.millitm)) / 1000;
			M_Iter++;
			it2++;
		}

		if (it3 != M_Pool_Solutions_IntegerMax.end() && time < M_Total_Time)
		{
			alguna = true;
			M_fo_temp = 0;


			LimpiarVectores();
			LimpiarRiskTempS();

			for (int i = 0; i < M_N_Interventions; i++)
			{
				M_I_T[i] = (*it3).first[i];

				M_Interventions[i].Set_T_Temp((*it3).first[i]);

				M_Interventions_Colocada[i] = true;
				AddScenarios(i);
				AddRecursos(i);
				M_fo_temp += M_Interventions[i].Get_RiskC((*it3).first[i]);

			}

			double val = FuncionObjetivoTemp();
			ActualizarMejorSolution(val, 104);
			VerifySolution();
			VND(val);

			ftime(&t1);

			time = ((double)((t1.time - M_Time_Initial.time) * 1000 + t1.millitm - M_Time_Initial.millitm)) / 1000;
			M_Iter++;
			it3++;
		}






	} while (alguna==true && time < M_Total_Time);
	//Si le queda tiempo
	if (time < Tiempo)
		GRASP();
}
double Maintenance::PathRelinkingDeAaB(vector<int> &A, vector<int> &B)
{
	//Solución nueva
	vector <int> Temporal = A;
	//voy metiendo intervenciones del B en el A

	for (int t = 0; t < M_T; t++)
	{
		vector<int>::iterator it = B.begin();
		do
		{
			//Primero colocado en B

//			it = find_if(it, B.end(), [](int ki) {return (ki == j); });
			if (it == B.end())
				continue;
			//Quito lo de la intervention 
			M_Interventions[(*it)].Set_T_Temp(Temporal[(*it)]);
			SubtractScenarios((*it));
			QuitarRecursos((*it));
			//Coloco la nueva
			Temporal[(*it)] = t;
			M_Interventions[(*it)].Set_T_Temp(t);
			AddScenarios((*it));
			AddRecursos((*it));
			
			//quitamos todo lo que haga que la solución no es posible
			//puede ser por recursos o por exclusiones




		} while (it != B.end());


	}
	return 0;
}
void Maintenance::ComputeExcessTemp()
{
	for (int i = 0; i < M_N_Interventions; i++)
	{
		double temp = 0.0;
		for (int t = M_Interventions[i].Get_T_Temp(); t < M_Interventions[i].Get_T_F_Temp(); t++)
		{
			temp += M_vfo2_temp[t];
		}
		M_Interventions[i].Set_Excess_temp(temp);
	}
}
double Maintenance::MejoraIntercambiosEjection(int tipo)
{
	bool Cambio = false;
	bool Algun_Cambio = false;
	double fobjAntiguo = FuncionObjetivoTemp();
	bool busqueda = M_Busqueda_Local;
	M_Busqueda_Local = false;
	if (tipo == 1)
	{
		ComputeExcessTemp();
	}
	struct timeb t1;
	ftime(&t1);
	double	time_v1 = ((double)((t1.time - M_Time_Initial.time) * 1000 + t1.millitm - M_Time_Initial.millitm)) / 1000;

	do
	{
		Cambio = false;
		Algun_Cambio = false;

		/*		double kk2 = FuncionObjetivo();
				if (fobjAntiguo <(kk2 - __FLT_EPSILON__) || fobjAntiguo>(kk2 + __FLT_EPSILON__))
				{
					int kk = 9;
				}*/
				//Vamos probando para ir cambiando a donde se pueda mejorar
		//		for (int t0 = 0; t0 < M_T && Cambio != true && M_vfo2_temp[M_Ivfo2_temp[t0]]>0; t0++)
	//		if (get_random(0,1)==0)
		if (M_Pool == false || get_random(0, 1) == 0)
		{
			//if (get_random(0, 4) >= 1)
			//	sort(M_Interventions_Dif.begin(), M_Interventions_Dif.end(),
			//		[&](size_t i, size_t j)
			//{ if ((M_Interventions[i].Get_RiskC(M_I_T[i]) - M_Interventions[i].Get_Min_Risk()) > (M_Interventions[j].Get_RiskC(M_I_T[j]) - M_Interventions[j].Get_Min_Risk()))
			//		return true;
			//else
			//		return (M_Interventions[i].Get_Min_Risk() > M_Interventions[j].Get_Min_Risk());
			//});
			//else
				random_shuffle(M_Interventions_Dif.begin(), M_Interventions_Dif.end());
		}
		else
			sort(M_Interventions_Dif.begin(), M_Interventions_Dif.end(), [&](size_t i, size_t j) { return ((M_Interventions[i].Get_Excess_temp()) > (M_Interventions[j].Get_Excess_temp())); });


		//		sort(M_Interventions_Dif.begin(), M_Interventions_Dif.end(),
		//			[&](size_t i, size_t j) { return (M_Interventions[i].Get_RiskC(M_I_T[i]) > M_Interventions[j].Get_RiskC(M_I_T[j])); });


		ftime(&t1);
		time_v1 = ((double)((t1.time - M_Time_Initial.time) * 1000 + t1.millitm - M_Time_Initial.millitm)) / 1000;

		for (int index = 0; index < ((M_N_Interventions - 1)) && (Algun_Cambio != true || M_Busqueda_Local) && time_v1<M_Total_Time; index++)
		{
			Cambio = false;

			//		for (list<pair<int, int>>::iterator it = M_I_T_Solution.begin(); it != M_I_T_Solution.end() && Cambio != true; it++)
			//		{

				//		int inter = (*it).first;
				//		int t_inter = (*it).second;
			int inter = M_Interventions_Dif[index];

			//			if (M_Interventions[inter].Get_Exclusions().size() == 0)
			//				continue;

			int t_inter = M_I_T[inter];
			int t_interf = M_Interventions[inter].Get_T_F_Temp();
			//		for (list<pair<int, int>>::iterator it = M_I_T_Solution.begin(); it != M_I_T_Solution.end() && Cambio != true; it++)
			//		{

			//			int inter = (*it).first;
			//			int t_inter = (*it).second;

			if (tipo == 0 && M_Interventions[inter].Get_RiskC(t_inter) < (M_Interventions[inter].Get_Min_Risk() + __FLT_EPSILON__))
				break;
			if (tipo == 1 && index > ((M_N_Interventions - 1) / 3))
				break;
			if (!PuedoQuitar(inter, t_inter)) continue;
			/*if (M_Interventions[inter].Get_RiskC(t_inter) < (M_Interventions[inter].Get_Min_Risk() + __FLT_EPSILON__))
			{
				if (get_random(0, 1) == 0)
					continue;
			}
			else
			{
				if (get_random(0, 10) <= 3)
					continue;
			}*/

			double fobj1 = M_Interventions[inter].Get_RiskC(t_inter);
			M_fo_temp -= fobj1;

			//Quito lo de la función objetvio
			SubtractScenarios(inter);
			FuncionObjetivoTemp(M_Interventions[inter].Get_T_Temp(), M_Interventions[inter].Get_T_F_Temp());
			//Quito recursos
			QuitarRecursos(inter);
			M_I_T[inter] = -1;
			M_Interventions_Colocada[inter] = false;
			//			cout << "Quito " << inter << " Time " << t_inter << endl;
			for (int index2 = index + 1; index2 < M_N_Interventions && Cambio != true && time_v1<M_Total_Time; index2++)
			{
				int inter2 = M_Interventions_Dif[index2];


				//				if (M_Interventions[inter2].Get_Exclusions().size() == 0)
				//					continue;
				int t_inter2 = M_I_T[inter2];
				int t_inter2f = M_Interventions[inter2].Get_T_F_Temp();
				//				if ( M_Interventions[inter].Get_RiskC(t_inter) < (M_Interventions[inter].Get_Min_Risk() + __FLT_EPSILON__) &&
				//					M_Interventions[inter2].Get_RiskC(t_inter2) < (M_Interventions[inter2].Get_Min_Risk() + __FLT_EPSILON__))
					//				continue;


				if (!PuedoQuitar(inter2, t_inter2)) continue;
				double fobj2 = M_Interventions[inter2].Get_RiskC(t_inter2);
				M_fo_temp -= fobj2;

				//Quito lo de la función objetvio
				SubtractScenarios(inter2);
				FuncionObjetivoTemp(M_Interventions[inter2].Get_T_Temp(), M_Interventions[inter2].Get_T_F_Temp());

				//Quito recursos
				QuitarRecursos(inter2);
				M_I_T[inter2] = -1;
				M_Interventions_Colocada[inter2] = false;
				//				cout << "Quito2 " << inter2 << " Time " << t_inter2 << endl;
								//Recorro en orden de los iniciales
				int cont2 = 0;
				for (vector<int> ::iterator itt2 = M_Interventions[inter2].Get_T_Ordenados_Risk().begin(); itt2 != M_Interventions[inter2].Get_T_Ordenados_Risk().end() && Cambio != true && cont2 < 10 && time_v1<M_Total_Time; itt2++)
				{
					//donde estaba
					if (t_inter2 == (*itt2))
						continue;
					int timeini2 = (*itt2);
					int timef2 = timeini2 + M_Interventions[inter2].Get_Delta(timeini2);

					M_fo_temp += M_Interventions[inter2].Get_RiskC(timeini2);
					if ((M_Alpha * (M_fo_temp / (double)M_T)) > (fobjAntiguo - __FLT_EPSILON__ - M_Min_Improve))
					{
						M_fo_temp -= M_Interventions[inter2].Get_RiskC(timeini2);
						break;
					}
					if (PuedoColocar(inter2, timeini2))
					{


						cont2++;

						int timefin2 = timeini2 + M_Interventions[inter2].Get_Delta(timeini2);
						AddScenarios(inter2, timeini2, timefin2);
						if (M_Pool) CopyFO2_2();
						double fobj_prev = FuncionObjetivoTemp( timeini2, timefin2);
						//Falta añadir aquí recursos del anterior para poder ver si entra o no la otra
						M_Interventions[inter2].Set_T_Temp(timeini2);
						AddRecursos(inter2);
						M_I_T[inter2] = timeini2;
						M_I_Seasons[inter2] = M_T_Season[timeini2];
						M_Interventions_Colocada[inter2] = true;
						//						cout << "add11 " << inter2 << " Time " << timeini2 << endl;
						int cont = 0;
						for (vector<int> ::iterator itt = M_Interventions[inter].Get_T_Ordenados_Risk().begin(); itt != M_Interventions[inter].Get_T_Ordenados_Risk().end() && Cambio != true && cont < 10; itt++)
						{
							if (t_inter == (*itt))
								continue;
							int timeini = (*itt);
							int timef = timeini + M_Interventions[inter].Get_Delta(timeini);
							//Una no está dónde estaba la otra nada
							if ((timeini2 >= t_interf || timef2 <= t_inter) && (timeini >= t_inter2f || timef <= t_inter2))
								continue;
							M_fo_temp += M_Interventions[inter].Get_RiskC(timeini);
							double partial = fobj_prev + (M_Alpha * (M_Interventions[inter].Get_RiskC(timeini) / (double)M_T));
							if ((partial) > (fobjAntiguo - __FLT_EPSILON__ - M_Min_Improve))
							{
								M_fo_temp -= M_Interventions[inter].Get_RiskC(timeini);
								break;
							}
							if (PuedoColocar(inter, timeini))
							{
								cont++;

								int timefin = timeini + M_Interventions[inter].Get_Delta(timeini);
								if (M_Pool)  CopyFO2();
								AddScenarios(inter, timeini, timefin);
//								if (inter == 5 && timeini == 10 && timefin == 12 && M_Iter==2 && inter2==16)
//									int kk = 9;
								double fobjNew = FuncionObjetivoTemp(timeini, timefin);
								//								cout << "add12 " << inter << " Time " << timeini << endl;
								//								double fobjNew = FuncionObjetivoTemp();
								//								if (fobjNew2 > fobjNew + __FLT_EPSILON__ || fobjNew2 < fobjNew - __FLT_EPSILON__)
								//									int problema = 9;
								if (fobjNew < (fobjAntiguo -  M_Min_Improve) || (fobjNew < (M_Best_fo - __FLT_EPSILON__)))
		//						if (fobjNew < (fobjAntiguo - __FLT_EPSILON__))
								{
									//	(*it).second = timeini;
			//							CambiarSolution(inter, timeini);
//									PonerAlFinalSolution(inter, timeini);
									M_I_T[inter] = timeini;
									M_I_Seasons[inter] = M_T_Season[timeini];
									M_Interventions[inter].Set_T_Temp(timeini);
									M_Interventions_Colocada[inter] = true;
									AddRecursos(inter);
//									M_I_T[inter2] = timeini2;
//									PonerAlFinalSolution(inter2, timeini2);

									fobjAntiguo = fobjNew;

									Cambio = true;
									Algun_Cambio = true;
									ActualizarMejorSolution(fobjNew, 5);
								}
								else
								{
									M_fo_temp -= M_Interventions[inter].Get_RiskC(timeini);
									if (M_Pool)  CopyBackFO2();
									SubtractScenarios(inter, timeini, timefin);
//									FuncionObjetivoTemp(timeini, timefin);
									//									cout << "quitar12 " << inter << " Time " << timeini << endl;
								}

							}
							else
								M_fo_temp -= M_Interventions[inter].Get_RiskC(timeini);


						}
						if (Cambio != true)
						{
							M_fo_temp -= M_Interventions[inter2].Get_RiskC(timeini2);
							SubtractScenarios(inter2, timeini2, timefin2);
							if (M_Pool) CopyBackFO2_2();
//							FuncionObjetivoTemp(timeini2, timefin2);

							QuitarRecursos(inter2);
							M_Interventions[inter2].Set_T_Temp(t_inter2);
							M_Interventions_Colocada[inter2] = true;
							M_I_T[inter2] = t_inter2;
							M_I_Seasons[inter2] = M_T_Season[t_inter2];
							//							cout << "quitar11 " << inter2 << " Time " << timeini2 << endl;

						}


					}
					else
						M_fo_temp -= M_Interventions[inter2].Get_RiskC(timeini2);

						ftime(&t1);
						time_v1 = ((double)((t1.time - M_Time_Initial.time) * 1000 + t1.millitm - M_Time_Initial.millitm)) / 1000;


				}
				if (Cambio != true)
				{
					AddRecursos(inter2);
					M_fo_temp += fobj2;
					M_Interventions_Colocada[inter2] = true;
					M_I_T[inter2] = t_inter2;
					AddScenarios(inter2);
					FuncionObjetivoTemp(M_Interventions[inter2].Get_T_Temp(), M_Interventions[inter2].Get_T_F_Temp());

				}
				ftime(&t1);
				time_v1 = ((double)((t1.time - M_Time_Initial.time) * 1000 + t1.millitm - M_Time_Initial.millitm)) / 1000;


			}


			if (Cambio != true)
			{
				AddRecursos(inter);
				M_fo_temp += fobj1;
				M_Interventions_Colocada[inter] = true;
				M_I_T[inter] = t_inter;
				AddScenarios(inter);
				FuncionObjetivoTemp(M_Interventions[inter].Get_T_Temp(), M_Interventions[inter].Get_T_F_Temp());

				//				cout << "add " << inter << " Time " << t_inter << endl;
			}
			ftime(&t1);
			time_v1 = ((double)((t1.time - M_Time_Initial.time) * 1000 + t1.millitm - M_Time_Initial.millitm)) / 1000;

		}

	}while (Algun_Cambio == true && M_Busqueda_Local && time_v1<M_Total_Time);
		M_Busqueda_Local = busqueda;

	return fobjAntiguo;
}
bool Maintenance::MejoraIntercambiosFo1Best()
{
	bool Cambio = false;
	do
	{
		Cambio = false;

		double fobjAntiguo = FuncionObjetivoTemp();
		double best_Move = fobjAntiguo;
		/*		double kk2 = FuncionObjetivo();
				if (fobjAntiguo <(kk2 - __FLT_EPSILON__) || fobjAntiguo>(kk2 + __FLT_EPSILON__))
				{
					int kk = 9;
				}*/
				//Vamos probando para ir cambiando a donde se pueda mejorar
		//		for (int t0 = 0; t0 < M_T && Cambio != true && M_vfo2_temp[M_Ivfo2_temp[t0]]>0; t0++)

		for (list<pair<int, int>>::iterator it = M_I_T_Solution.begin(); it != M_I_T_Solution.end() && Cambio != true; it++)
		{

			int inter = (*it).first;
			int t_inter = (*it).second;
			if (M_Pool == false && M_Interventions[inter].Get_RiskC(t_inter) < (M_Interventions[inter].Get_Min_Risk() + __FLT_EPSILON__))
				continue;
			//			for (int k = 0; k < M_N_Interventions && Cambio!=true; k++)
			//			{
			//					int i = M_Vec_Alea_I[k];
			//					int inter = i;
			//					int t_inter = M_I_T[inter];

			//				double fobjAntiguo1 = FuncionObjetivoTemp();
							//No está aquí asignda
//				if (M_Interventions[inter].Get_T_Temp() > t || M_Interventions[inter].Get_T_F_Temp() <= t)
//					continue;
			double fobj1 = M_Interventions[inter].Get_RiskC(t_inter);
			M_fo_temp -= fobj1;

			//Quito lo de la función objetvio
			for (int t = M_Interventions[inter].Get_T_Temp(); t < M_Interventions[inter].Get_T_F_Temp(); t++)
			{
				for (int si = 0; si < M_Scenarios_number[t]; si++)
				{
					M_T_Temp_Risk_S[t][si] -= M_Interventions[inter].Get_Risk()[t][t_inter][si];
				}
			}

			//Quito recursos
			QuitarRecursos(inter);
			//Recorro en orden de los iniciales
			int cont = 0;
			double fobjNew = 0;
			int best_timeini = -1;
			for (vector<int> ::iterator itt = M_Interventions[inter].Get_T_Ordenados_Risk().begin(); itt != M_Interventions[inter].Get_T_Ordenados_Risk().end()  && cont < get_random(6, M_N_Interventions); itt++, cont++)
			{
				if (t_inter == (*itt))
					continue;
				int timeini = (*itt);
				M_fo_temp += M_Interventions[inter].Get_RiskC(timeini);
				if ((M_Alpha * (M_fo_temp / (double)M_T)) > fobjAntiguo)
				{
					M_fo_temp -= M_Interventions[inter].Get_RiskC(timeini);
					continue;
				}

				if (PuedoColocar(inter, timeini))
				{

					int timefin = timeini + M_Interventions[inter].Get_Delta(timeini);
					for (int t3 = timeini; t3 < timefin; t3++)
					{
						for (int si = 0; si < M_Scenarios_number[t3]; si++)
						{
							M_T_Temp_Risk_S[t3][si] += M_Interventions[inter].Get_RiskN(t3,timeini,si);
						}
					}


					fobjNew = FuncionObjetivoTemp();

					if (fobjNew < (best_Move - 0.0001))
					{
						//	(*it).second = timeini;
//							CambiarSolution(inter, timeini);
//						if (best_timeini != -1)
//							int kk = 9;
						best_timeini = timeini;
						
						best_Move = fobjNew;
						Cambio = true;

					}
					M_fo_temp -= M_Interventions[inter].Get_RiskC(timeini);
					for (int t3 = timeini; t3 < timefin; t3++)
					{
						for (int si = 0; si < M_Scenarios_number[t3]; si++)
						{
							M_T_Temp_Risk_S[t3][si] -= M_Interventions[inter].Get_RiskN(t3, timeini, si);
						}
					}
					


				}
				else
					M_fo_temp -= M_Interventions[inter].Get_RiskC(timeini);


			}
			if (Cambio != true)
			{
				AddRecursos(inter);
				M_fo_temp += fobj1;
				for (int tt0 = M_Interventions[inter].Get_T_Temp(); tt0 < M_Interventions[inter].Get_T_F_Temp(); tt0++)
				{
					for (int si = 0; si < M_Scenarios_number[tt0]; si++)
					{
						M_T_Temp_Risk_S[tt0][si] += M_Interventions[inter].Get_Risk()[tt0][t_inter][si];
					}
				}
			}
			else
			{
				
				M_fo_temp += M_Interventions[inter].Get_RiskC(best_timeini);

				int best_timefin = best_timeini + M_Interventions[inter].Get_Delta(best_timeini);
				for (int t3 = best_timeini; t3 < best_timefin; t3++)
				{
					for (int si = 0; si < M_Scenarios_number[t3]; si++)
					{
						M_T_Temp_Risk_S[t3][si] += M_Interventions[inter].Get_Risk()[t3][best_timeini][si];
					}
				}
//				PonerAlFinalSolution(inter, best_timeini);
				M_I_T[inter] = best_timeini;
				M_I_Seasons[inter] = M_T_Season[best_timeini];
				M_Interventions[inter].Set_T_Temp(best_timeini);
				AddRecursos(inter);
				Cambio = true;
				ActualizarMejorSolution(fobjNew,3);
			}

		}

	} while (Cambio == true);

	return false;
}
void Maintenance::PonerAlFinalSolution(int inter, int time)
{
	for (list<pair<int, int>>::iterator it = M_I_T_Solution.begin(); it != M_I_T_Solution.end(); it++)
	{
		if ((*it).first == inter)
		{
			M_I_T_Solution.erase(it);
			pair<int, int> p(inter, time);
			M_I_T_Solution.push_back(p);
			return;
		}
	}
	Problema(56);
}

void Maintenance::CambiarSolution(int inter, int time)
{
	for (list<pair<int, int>>::iterator it = M_I_T_Solution.begin(); it != M_I_T_Solution.end(); it++)
	{
		if ((*it).first == inter)
		{
			(*it).second = time;
			return;
		}
	}
	Problema(44);
}
bool Maintenance::RepararSolutionBestT()
{
	if (M_NI_Colocadas == 0)
		return false;
	list<pair<int, int>> lista = M_I_T_Solution;
	LimpiarVectores();
	for (list<pair<int, int>>::iterator it = lista.begin(); it != lista.end(); it++)
	{
		ColocarIEnTiempo((*it).first, (*it).second);
		ModificarRiskTempAll(M_Interventions[(*it).first].Get_T_Temp(), M_Interventions[(*it).first].Get_T_F_Temp());

		//		file << M_Interventions[(*it).first].Get_Name() << " " << (*it).second + 1 << endl;
	}
	int elegido = 0;
	bool algo = false;
	do
	{
		elegido = ElegidoMayorDif();
		if (elegido == (-1))
			continue;
		algo = true;
		ColocarIEnBestTiempo(elegido);
		ModificarRiskTempAll(M_Interventions[elegido].Get_T_Temp(), M_Interventions[elegido].Get_T_F_Temp());


	} while (elegido != (-1));

	return algo;
}
/*
bool Maintenance::MejorarSolutionBestT()
{
	if (M_I_T_Solution.size() == 0)
		return false;
	list<pair<int, int>> lista = M_I_T_Solution;
	LimpiarVectores();
	for (list<pair<int, int>>::iterator it = M_I_T_Solution.begin(); it != M_I_T_Solution.end(); it++)
	{
		int num_int = (*it).first;
		bool stop = false;
		list<int> ::iterator it = M_Interventions[num_int].Get_T_Ordenados_Risk_Temp().begin();
		do
		{

			double fobj1 = M_Interventions[num_int].Get_RiskC((*it));
			double fobj2 = 0;
			if (fobj1 > Min)
			{
				stop = true;
				break;

			}

			if (PuedoColocar(num_int, (*it)))
			{
				fobj2 = SegundaParteFObj(num_int, (*it), (*it) + M_Interventions[num_int].Get_Delta((*it)));
				//		if ((M_Interventions[n_int].Get_RiskC()[(*it)] > (min + __FLT_EPSILON__)) || vec_temp.size() > 1)
				if ((fobj1 + fobj2 < Min))
				{
					elegido = (*it);
					Min = fobj1 + fobj2;
				}
				it++;

			}
			else
			{
				if (M_Interventions[num_int].Get_T_Ordenados_RiskQ_Temp().size() != M_Interventions[num_int].Get_T_Ordenados_Risk_Temp().size())
					int kk = 9;
				int borrado = (*it);
				M_Interventions[num_int].Get_T_Ordenados_RiskQ_Temp().erase(find(M_Interventions[num_int].Get_T_Ordenados_RiskQ_Temp().begin(), M_Interventions[num_int].Get_T_Ordenados_RiskQ_Temp().end(), borrado));

				it = M_Interventions[num_int].Get_T_Ordenados_Risk_Temp().erase(it);
				if (M_Interventions[num_int].Get_T_Ordenados_RiskQ_Temp().size() != M_Interventions[num_int].Get_T_Ordenados_Risk_Temp().size())
					int kk = 9;
				if (M_Interventions[num_int].Get_T_Ordenados_Risk_Temp().size() == 0)
					stop = true;
			}




		} while (it != M_Interventions[num_int].Get_T_Ordenados_Risk_Temp().end() && stop != true);
	}

	return algo;
}*/
int Maintenance::ElegidoMayorDif()
{
	double max = 0;
	int choosen = (-1);
	double aporte = 0;
	for (int k = 0; k < M_N_Interventions; k++)
	{
		int i = M_Vec_Alea_I[k];
		if (M_Interventions[i].Get_T_Ordenados_Risk_Temp().size() == 0) continue;
		if ((M_Interventions[i].Get_RiskC()[M_Interventions[i].Get_T_Temp()] - M_Interventions[i].Get_Min_Risk_Temp()) > (max+__FLT_EPSILON__))
		{
			max = (M_Interventions[i].Get_RiskC()[M_Interventions[i].Get_T_Temp()] - M_Interventions[i].Get_Min_Risk_Temp());
			choosen = i;
			aporte = max / M_T;
		}
	}
	return choosen;
}

//Tenemos una , buscamos una intervention que no esté en su mínimo, la colocamos y colocamos las
//que se solapen en otros puntos

void Maintenance::BusquedaLocalIntercambio()
{
	//Empezamos moviendo los que no están en su mínimo valor
	double Quito = 0;

	Copy();
	vector<int> quitados;
	double anterior_fo= M_fo_temp;
	bool stop = false;
	int iter = 0;
	do
	{

		stop = false;
		quitados.clear();
		Quito = 0;
		//Quito mal colocadas 
		for (int i = 0; i < M_N_Interventions && stop != true; i++)
		{
			int pos = M_Vec_Alea_I[i];
			if (M_Interventions_Colocada[pos] == false)
				continue;
			if (M_Interventions[pos].Get_RiskC()[M_Interventions[pos].Get_T_Temp()] > M_Interventions[pos].Get_Min_Risk()+__FLT_EPSILON__)
			{

				quitados.push_back(pos);
				double val = 1;
				Quito += (M_Interventions[pos].Get_RiskC()[M_Interventions[pos].Get_T_Temp()]- M_Interventions[pos].Get_Min_Risk())/M_T ;
				if ((val*M_Best_fo) > (M_fo_temp - Quito) )
				{
					stop = true;
				}
				//Este es posible para mejorar
				//este lo pondría en un t mejor pero debo quitar otro

		//		bool posible=QuitarInterventionParaMeter(i, t);


			}
		}
//		if (quitados.size() > 2)
//			int kk = 9;
		//
		for (int i = 0; i < quitados.size(); i++)
		{
			int elegido = quitados[i];
			for (vector<int>::iterator itt = M_Interventions[elegido].Get_Workload_Res().begin(); itt != M_Interventions[elegido].Get_Workload_Res().end(); itt++)
			{


//			for (map<int, map<int, map <int, double>>> ::iterator itt = M_Interventions[elegido].Get_Workload().begin(); itt != M_Interventions[elegido].Get_Workload().end(); itt++)
//			{
				for (int tprima = M_Interventions[elegido].Get_T_Temp(); tprima < (M_Interventions[elegido].Get_T_Temp() + M_Interventions[elegido].Get_Delta()[M_Interventions[elegido].Get_T_Temp()]); tprima++)
				{
					M_Resources[(*itt)].Subtract(M_Interventions[elegido].Get_WorkloadN((*itt), M_Interventions[elegido].Get_T_Temp(),tprima),tprima);

				}
			}
			M_fo_temp -= M_Interventions[elegido].Get_RiskC()[M_Interventions[elegido].Get_T_Temp()] / M_T;
			M_Interventions_Colocada[elegido] = false;
//			Borrar(elegido, M_Interventions_T[elegido]);
			M_I_T[elegido] = -1;
			M_Interventions[elegido].Set_T_Temp(-1);

			//Pongo la season en la que pongo la pieza
			M_I_Seasons[elegido] = (-1);
		}
//		if (M_Iter == 1 && iter == 7)
//			int kk = 9;
		double val_nuevo = anterior_fo;

		if (val_nuevo <= (M_Best_fo * (1.01)))
		{
			Copy2();
			for (int i = 0; i < 10; i++)
			{
				val_nuevo = RellenarSolution(i);
				if (val_nuevo < anterior_fo)
				{
					anterior_fo = val_nuevo;
					iter = 0;
					Copy();
				}
				else
					CopyBack2();

			}
			val_nuevo = RellenarSolutionAv2(get_random(10, 11));
		}
		else
		{
			if (get_random(0, 4) != 0)
				val_nuevo = RellenarSolutionAv2(get_random(10, 11));
			else
				val_nuevo = RellenarSolution(get_random(0, 9));
		}

		iter++;
		random_shuffle(M_Vec_Alea_I.begin(), M_Vec_Alea_I.end());
		
		if (val_nuevo < anterior_fo)
		{
			anterior_fo = val_nuevo;
			iter = 0;
			Copy();
		}
		else
			CopyBack();
			
		//Las pongo donde pueda
	} while (iter < 10);
}
bool Maintenance::QuitarInterventionParaMeter(int n_int, int t)
{
	//Voy a meter la intervention Nint
		//Si he llegado aquí es que puedo ponerla
	//Puedo ponerla

	for (vector<int>::iterator it = M_Interventions[n_int].Get_Workload_Res().begin(); it != M_Interventions[n_int].Get_Workload_Res().end(); it++)
	{

//	for (map<int, map<int, map <int, double>>> ::iterator it = M_Interventions[n_int].Get_Workload().begin(); it != M_Interventions[n_int].Get_Workload().end(); it++)
//	{
		//Recursos
		for (int tprima = t; tprima < (t + M_Interventions[n_int].Get_Delta(t)); tprima++)
		{

			M_Resources[(*it)].Add(M_Interventions[n_int].Get_WorkloadN((*it),t,tprima), tprima);
			//			if (M_Interventions[n_int].Get_Workload()[(*it).first][tprima][t] > 0 && (*it).first == 1 && tprima==6)
			//				cout << M_I_Name[n_int] << " " << tprima << " " << t << " " << M_R_Name[(*it).first] << " " << M_Interventions[n_int].Get_Workload()[(*it).first][tprima][t] << " Temp "<< M_Resources[(*it).first].Get_Temp()[tprima-1]<<  endl;

		}

	}

	//Esto es para poner la nueva intervention
	pair<int, int> p(n_int, t);
	M_I_T_Solution.push_back(p);
	//Pongo la season en la que pongo la pieza
	M_I_Seasons[n_int] = M_T_Season[t];
	M_Interventions[n_int].Set_T_Temp(t);
	//Hay que borrar las otras 


	for (vector<int>::iterator it = M_Interventions[n_int].Get_Workload_Res().begin(); it != M_Interventions[n_int].Get_Workload_Res().end(); it++)
	{
//	for (map<int, map<int, map <int, double>>> ::iterator it = M_Interventions[n_int].Get_Workload().begin(); it != M_Interventions[n_int].Get_Workload().end(); it++)
//	{
		//Recursos
		for (int tprima = t; tprima < (t + M_Interventions[n_int].Get_Delta(t)); tprima++)
		{
			if ((M_Interventions[n_int].Get_WorkloadN((*it),t,tprima) + M_Resources[(*it)].Get_Temp()[tprima] > (M_Resources[(*it)].Get_Upper()[tprima]+__FLT_EPSILON__)) ||
				(M_Interventions[n_int].Get_WorkloadN((*it),t,tprima) + M_Resources[(*it)].Get_Temp()[tprima] < (M_Resources[(*it)].Get_Lower()[tprima] - __FLT_EPSILON__)))
				return false;
		}
		//Exclusiones
		//TO DO cambiar con lo de arreglar esta exclusion
		if (Exclusion(n_int, t) == true)
			return false;

	}


}

int  Maintenance::AleatorizarIgualesListInterventions(int tipo)
{

	bool tipo_elegido = false;
	do
	{
		switch (tipo)
		{
		default:
			if (M_Interventions_MaxDifR[0].second > M_Interventions_MaxDifR[M_Interventions_MaxDifR.size() - 1].second)
			{
				tipo_elegido = true;
				//Aleatorizar los valores iguales
				int index_anterior = 0;
				for (list<int> ::iterator it = M_Interventions_MaxDifR_I.begin(); it != M_Interventions_MaxDifR_I.end(); it++)
				{
					if (((*it) - index_anterior) > 1)
						random_shuffle(M_Interventions_MaxDifR.begin()+index_anterior, M_Interventions_MaxDifR.begin() + (*it));
					index_anterior = (*it);
				}
			}
			else 				tipo++;
			break;
		case 1:
			if (M_Interventions_MaxDifW[0].second > M_Interventions_MaxDifW[M_Interventions_MaxDifW.size() - 1].second)
			{
				tipo_elegido = true;
				int index_anterior = 0;

				for (list<int> ::iterator it = M_Interventions_MaxDifW_I.begin(); it != M_Interventions_MaxDifW_I.end(); it++)
				{
					if (((*it) - index_anterior) > 1)
						random_shuffle(M_Interventions_MaxDifW.begin() + index_anterior, M_Interventions_MaxDifW.begin() + (*it));
					index_anterior = (*it);
				}
			}
			else 				tipo++;
			break;
		case 2:
			if (M_Interventions_MinR[0].second > M_Interventions_MinR[M_Interventions_MinR.size() - 1].second)
			{

				tipo_elegido = true;
				int index_anterior = 0;
				for (list<int> ::iterator it = M_Interventions_MinR_I.begin(); it != M_Interventions_MinR_I.end(); it++)
				{
					if (((*it) - index_anterior) > 1)
						random_shuffle(M_Interventions_MinR.begin() + index_anterior, M_Interventions_MinR.begin() + (*it));
					index_anterior = (*it);
				}
			}
			else 				tipo++;
			break;
		case 3:
			if (M_Interventions_MinW[0].second > M_Interventions_MinW[M_Interventions_MinW.size() - 1].second)
			{
				tipo_elegido = true;
				int index_anterior = 0;
				for (list<int> ::iterator it = M_Interventions_MinW_I.begin(); it != M_Interventions_MinW_I.end(); it++)
				{
					if (((*it) - index_anterior) > 1)
						random_shuffle(M_Interventions_MinW.begin() + index_anterior, M_Interventions_MinW.begin() + (*it));
					index_anterior = (*it);
				}
			}
			else 				tipo++;
			break;
		case 4:
			if (M_Interventions_MinWxT[0].second > M_Interventions_MinWxT[M_Interventions_MinWxT.size() - 1].second)
			{
				tipo_elegido = true;
				int index_anterior = 0;
				for (list<int> ::iterator it = M_Interventions_MinWxT_I.begin(); it != M_Interventions_MinWxT_I.end(); it++)
				{
					if (((*it) - index_anterior) > 1)
						random_shuffle(M_Interventions_MinWxT.begin()+index_anterior, M_Interventions_MinWxT.begin() + (*it));
					index_anterior = (*it);
				}

			}
			else 				tipo++;
			break;
		case 5:
			if (M_Interventions_MinRxT[0].second > M_Interventions_MinRxT[M_Interventions_MinRxT.size() - 1].second)
			{
				tipo_elegido = true;
				int index_anterior = 0;
				for (list<int> ::iterator it = M_Interventions_MinRxT_I.begin(); it != M_Interventions_MinRxT_I.end(); it++)
				{
					if (((*it) - index_anterior) > 1)
						random_shuffle(M_Interventions_MinRxT.begin()+index_anterior, M_Interventions_MinRxT.begin() + (*it));
					index_anterior = (*it);
				}


			}
			else 		tipo++;

			break;
		case 6:
			if (M_Interventions_Min_WXR[0].second > M_Interventions_Min_WXR[M_Interventions_Min_WXR.size() - 1].second)
			{
				tipo_elegido = true;
				int index_anterior = 0;
				for (list<int> ::iterator it = M_Interventions_Min_WXR_I.begin(); it != M_Interventions_Min_WXR_I.end(); it++)
				{
					if (((*it) - index_anterior) > 1)
						random_shuffle(M_Interventions_Min_WXR.begin()+index_anterior, M_Interventions_Min_WXR.begin() + (*it));
					index_anterior = (*it);
				}

			}
			else 		tipo++;

			break;
		case 7:
			if (M_Interventions_Min_WtXRt[0].second > M_Interventions_Min_WtXRt[M_Interventions_Min_WtXRt.size() - 1].second)
			{
				tipo_elegido = true;
				int index_anterior = 0;
				for (list<int> ::iterator it = M_Interventions_Min_WtXRt_I.begin(); it != M_Interventions_Min_WtXRt_I.end(); it++)
				{

					if (((*it) - index_anterior) > 1)
						random_shuffle(M_Interventions_Min_WtXRt.begin()+index_anterior, M_Interventions_Min_WtXRt.begin() + (*it));
					index_anterior = (*it);
				}

			}
			else 		tipo = 8;

			break;
		case 8:
			if (M_Interventions_Exclusions[0].second > M_Interventions_Exclusions[M_Interventions_Exclusions.size() - 1].second)
			{
				tipo_elegido = true;
				int index_anterior = 0;
				for (list<int> ::iterator it = M_Interventions_Exclusions_I.begin(); it != M_Interventions_Exclusions_I.end(); it++)
				{

					if (((*it) - index_anterior) > 1)
						random_shuffle(M_Interventions_Exclusions.begin() + index_anterior, M_Interventions_Exclusions.begin() + (*it));
					index_anterior = (*it);
				}

			}
			else 		tipo = 9;

			break;
		case 9:

				tipo_elegido = true;
				sort(M_Interventions_Out_Ordenada.begin(), M_Interventions_Out_Ordenada.end(),
					[&](size_t i, size_t j) { return M_Interventions_Out[i]>M_Interventions_Out[j]; });


			break;
		}
	} while (tipo_elegido != true);
	return tipo;
}
bool Maintenance::ConstructiveRXT(int tipo)
{
	//Recorro los T
	double umbral = 0;
	LimpiarVectores();

	M_Tipo = AleatorizarIgualesListInterventions(tipo);
	//El primero que esté en su mínimo lo pongo
	for (int tiempo = 0; tiempo < M_T; tiempo++)
	{
		//Esta función me devuelve la intervention que se pueda colocar aquí 

		//ya me dice si puede o no
		int nint = BuscarIntervention(tiempo, umbral);
		if (nint >= 0)
		{
			ColocarIEnTiempo(nint, tiempo);
		}

	}
	//Segunda pasada
	umbral = min((M_Best_fo - M_Cota)/ get_random(3,10),M_Cota*2);
	//El primero que esté en su mínimo lo pongo
	for (int tiempo = 0; tiempo < M_T; tiempo++)
	{
		//Esta función me devuelve la intervention que se pueda colocar aquí 

		//ya me dice si puede o no
		int nint = BuscarIntervention(tiempo, umbral);
		if (nint >= 0)
		{
			ColocarIEnTiempo(nint, tiempo);
		}

	}
	if (M_NI_Colocadas == M_N_Interventions)
		ActualizarMejorSolution(FuncionObjetivo(),15);
	else
	{
		Copy();

		RepararSolutionCopy();
	}
	return true;
}
int Maintenance::BuscarIntervention(int time, double threshold)
{
	for (int i = 0; i < M_N_Interventions; i++)
	{
		int choose = IndiceTipo(M_Tipo, i);


		//No está colocada
		if (M_Interventions_Colocada[choose] == true) continue;
		//El riesgo está en el umbral
		if ((M_Interventions[choose].Get_Min_Risk() + __FLT_EPSILON__) < (M_Interventions[choose].Get_RiskC()[time] - threshold))
			continue;
		//Si la puedo colocar
		if (PuedoColocar(choose, time))
			return choose;

	}
	return (-1);
}
bool Maintenance::ConstructiveR(int tipo) //Si tipo es 0 es por diferencia, si es 1 por dif de workload, si es 2 por min R, si es 3 por min W
{

	//Primera opción
	//Coger las interventions ordenadas de mayor a menor por riesgo minimo y poner donde menor riesgo tenga
	//y sea posible

//	M_I_Seasons.clear();
	LimpiarVectores();

	M_Tipo = AleatorizarIgualesListInterventions(tipo);

	//	for (vector<Intervention>::iterator it = M_Interventions.begin(); it != M_Interventions.end(); it++)
	bool colocado = true;
	int choose = -1;
	bool terminado = false;
	bool copy_solution = false;

	do
	{
		terminado = false;
		bool primero = false;
		bool elegido = false;
		int choose_elegido = -1;
		int ielegido = 0;
//		if (M_Iter == 87 && M_I_T_Solution.size() == 107)
//			int kk = 9;
		for (int i = 0; i < M_Interventions.size() && elegido != true; i++)
		{
			
			//			if (M_Iter%3==0) ielegido = M_N_Interventions - 1 - i;
			choose = IndiceTipo(M_Tipo, i);
			ielegido = i;
			int alea = 0;
			if (M_Interventions_Colocada[choose] == true) continue;
			alea = get_random(0, M_Delta);
			//			if (alea ==M_Delta && primero==true) continue;
			//			if (primero == true || alea!=M_Delta) elegido = true;
			if (alea >= 1 && primero == true) continue;
			if (primero == true || alea == 0) elegido = true;

			primero = true;

			choose_elegido = choose;


		}
//		if (choose_elegido == 252)
//			int kk = 9;
		colocado = ColocarEnTiempoRValor(choose_elegido);
//				colocado = ColocarEnTiempoRGreedy(choose_elegido);
		//			colocado = ColocarEnTiempo(choose_elegido);

		if (colocado == true)
		{
			M_Interventions_Colocada[choose_elegido] = true;
		}
		else
		{
			if (!M_Silent) cout << "No puede colocar  la intervention " << choose_elegido << endl << endl;
			if (M_Tipo != 9) M_Interventions_Out[choose_elegido]++;
			return false;
		}
		if (M_NI_Colocadas == M_N_Interventions)
			return true;
		if (copy_solution == false && M_NI_Colocadas > ((M_Porcentaje_Ocupado * M_N_Interventions)))
		{
			copy_solution = true;
			Copy();
		}
	} while (terminado != true);
	return false;





};
bool Maintenance::ConstructiveRBias(int tipo) //Si tipo es 0 es por diferencia, si es 1 por dif de workload, si es 2 por min R, si es 3 por min W
{

	//Primera opción
	//Coger las interventions ordenadas de mayor a menor por riesgo minimo y poner donde menor riesgo tenga
	//y sea posible

//	M_I_Seasons.clear();
	LimpiarVectores();

	M_Tipo = AleatorizarIgualesListInterventions(tipo);
	CopyListInterventionsTempBias();
	//	for (vector<Intervention>::iterator it = M_Interventions.begin(); it != M_Interventions.end(); it++)
	bool colocado = true;
	int choose = -1;
	bool terminado = false;
	bool copy_solution = false;


		terminado = false;


		int choose_elegido = -1;


		for (int i = 0; i < M_Interventions.size() ; i++)
		{

			//			if (M_Iter%3==0) ielegido = M_N_Interventions - 1 - i;
			choose = M_Temp_Item[i];
			if (M_Interventions_Colocada[choose] == true) 
				continue;
			choose_elegido = choose;
			colocado = ColocarEnTiempoRValor(choose_elegido);
			//				colocado = ColocarEnTiempoRGreedy(choose_elegido);
					//			colocado = ColocarEnTiempo(choose_elegido);

			if (colocado == true)
			{
				M_Interventions_Colocada[choose_elegido] = true;
			}
			else
			{
				if (!M_Silent) cout << "No puede colocar  la intervention " << choose_elegido << endl << endl;
				if (M_Tipo != 9) M_Interventions_Out[choose_elegido]++;
				return false;
			}
			if (M_NI_Colocadas == M_N_Interventions)
				return true;
			if (copy_solution == false && M_NI_Colocadas > ((M_Porcentaje_Ocupado * M_N_Interventions)))
			{
				copy_solution = true;
				Copy();
			}

		}

	
	
	return false;





};

bool Maintenance::ConstructiveRCL(int tipo) //Si tipo es 0 es por diferencia, si es 1 por dif de workload, si es 2 por min R, si es 3 por min W
{

	//Primera opción
	//Coger las interventions ordenadas de mayor a menor por riesgo minimo y poner donde menor riesgo tenga
	//y sea posible

//	M_I_Seasons.clear();
	LimpiarVectores();

	M_Tipo = AleatorizarIgualesListInterventions(tipo);

	//	for (vector<Intervention>::iterator it = M_Interventions.begin(); it != M_Interventions.end(); it++)
	bool colocado = true;
	int choose = -1;
	bool terminado = false;
	bool copy_solution = false;

	do
	{
		terminado = false;
		bool primero = false;
		bool elegido = false;
		int choose_elegido = -1;
		int ielegido = 0;
		for (int i = 0; i < M_Interventions.size() && elegido != true; i++)
		{

			//			if (M_Iter%3==0) ielegido = M_N_Interventions - 1 - i;
			choose = IndiceTipo(M_Tipo, ielegido);
			ielegido = i;
			int alea = 0;
			if (M_Interventions_Colocada[choose] == true) continue;
			alea = get_random(0, M_Delta);
			//			if (alea ==M_Delta && primero==true) continue;
			//			if (primero == true || alea!=M_Delta) elegido = true;
			if (alea >= 1 && primero == true) continue;
			if (primero == true || alea == 0) elegido = true;

			primero = true;

			choose_elegido = choose;


		}
//		if (choose_elegido == 252)
//			int kk = 9;
		colocado = ColocarEnTiempoRValor(choose_elegido);
		//				colocado = ColocarEnTiempoRGreedy(choose_elegido);
				//			colocado = ColocarEnTiempo(choose_elegido);

		if (colocado == true)
		{
			M_Interventions_Colocada[choose_elegido] = true;
		}
		else
		{
			if (!M_Silent) cout << "No puede colocar  la intervention " << choose_elegido << endl << endl;
			if (M_Tipo != 9) M_Interventions_Out[choose_elegido]++;
			return false;
		}
		if (M_NI_Colocadas == M_N_Interventions)
			return true;
		if (copy_solution == false && M_NI_Colocadas > ((M_Porcentaje_Ocupado * M_N_Interventions)))
		{
			copy_solution = true;
			Copy();
		}
	} while (terminado != true);
	return false;





};
bool Maintenance::ConstructiveRA(int tipo) //Si tipo es 0 es por diferencia, si es 1 por dif de workload, si es 2 por min R, si es 3 por min W
{

	//Primera opción
	//Coger las interventions ordenadas de mayor a menor por riesgo minimo y poner donde menor riesgo tenga
	//y sea posible

//	M_I_Seasons.clear();
	LimpiarVectores();
	LimpiarRiskTempS();

	M_Tipo = tipo;

	//	for (vector<Intervention>::iterator it = M_Interventions.begin(); it != M_Interventions.end(); it++)
	double tmp_first = 0;

	bool terminado = false;
	bool copy_solution = false;
	bool copy_solution2 = false;
	do
	{
		terminado = false;

		int choose_elegido = -1;
		if (get_random(0,1)>=1)
		choose_elegido = ElegirMayorAleatorizado(tipo);
		else
		{
	//		if (get_random(0,1)==0)
				choose_elegido = ElegirMayorAleatorizadoSPG(tipo);
	//		else
	//			choose_elegido = ElegirMenorAleatorizadoSPG(tipo);
		}
			 //M_Tipo);


		/*
		for (int i = 0; i < M_N_Interventions && elegido != true; i++)
		{
			int ielegido = i;
			//			if (M_Iter%3==0) ielegido = M_N_Interventions - 1 - i;
			choose = IndiceTipo(tipo, ielegido);
			choose = ElegirMayorAleatorizado(M_Tipo);
			int alea = 0;
			if (M_Interventions_Colocada[choose] == true ) continue;
			if (M_Interventions[choose].Get_T_Ordenados_Risk_Temp().size() == 0) continue;
			alea = get_random(0, M_Delta);
			//			if (alea ==M_Delta && primero==true) continue;
			//			if (primero == true || alea!=M_Delta) elegido = true;
			if (alea >= 1 && primero == true) continue;
			if (primero == true || alea == 0) elegido = true;

			primero = true;

			choose_elegido = choose;


		}*/
		if (choose_elegido < 0)
			return false;
		//Coloca la intervention en la primera T disponible
//		if (M_I_T_Solution.size() == 45)
//			int kk = 9;

		if (get_random(0, 2) >= 2 )
		{
			int telegido = ElegirMayorTFuncionObjetivoAbsoluto(choose_elegido);
			ColocarIEnTiempo(choose_elegido, telegido);
/*			if (get_random(0, 1) >= 2) 
				ColocarIEnTiempoValor(choose_elegido);
			else
				ColocarIEnTiempoSPG(choose_elegido);*/
		}
		
		else
		{
			int telegido = ElegirMayorTFuncionObjetivo(choose_elegido);
			ColocarIEnTiempo(choose_elegido, telegido);
		}
		ModificarRiskTemp(M_Interventions[choose_elegido].Get_T_Temp(), M_Interventions[choose_elegido].Get_T_F_Temp());

		
		if (M_NI_Colocadas == M_N_Interventions)
			return true;
		if (copy_solution == false && M_NI_Colocadas > ((M_Porcentaje_Ocupado * M_N_Interventions)))
		{
			copy_solution = true;
			Copy();
		}
		if (copy_solution2 == false && M_NI_Colocadas > ((M_Porcentaje_Ocupado2 * M_N_Interventions)))
		{
			copy_solution2 = true;
			Copy2();
		}
		timeb t2;
		ftime(&t2);


		tmp_first = ((double)((t2.time - M_Time_Initial.time) * 1000 + t2.millitm - M_Time_Initial.millitm)) / 1000;


		

	} while (terminado != true && tmp_first < M_Total_Time);




	return false;
};
void Maintenance::ModificarRiskTempAll(int tini, int tfin)
{
	//Hay que coger y eliminar los t hasta uno de cada intervention que pueda colocarse y todos los que
	//tengan igual valor
	bool modificado = false;
	for (int i = 0; i < M_N_Interventions; i++)
	{

		if (M_Interventions[i].Get_T_Ordenados_Risk_Temp().size() == 0)
		{
			if (M_Silent != true) cout << endl << "No se puede colocar la " << i << endl;
			continue;
		}

		if (M_Interventions[i].MinEntre(tini, tfin) == true)
		{
			ModificarRiskTemp(i);
			modificado = true;

		}

	}

};
void Maintenance::ModificarRiskTemp(int tini, int tfin)
{
	//Hay que coger y eliminar los t hasta uno de cada intervention que pueda colocarse y todos los que
	//tengan igual valor
	bool modificado = false;
	for (int i = 0; i < M_N_Interventions; i++)
	{
		if (M_Interventions_Colocada[i] == true) continue;
		if (M_Interventions[i].Get_T_Ordenados_Risk_Temp().size() == 0)
		{
			if (M_Silent != true) cout << endl << "No se puede colocar la " << i << endl;
			continue;
		}

		if (M_Interventions[i].MinEntre(tini, tfin) == true)
		{
			ModificarRiskTemp(i);
			modificado = true;
				
		}

	}
/*	if (modificado == true)
	{
		sort(M_Interventions_Regret.begin(), M_Interventions_Regret.end(),
			[&](size_t i, size_t j) { return M_Interventions[i].Get_Regret() > M_Interventions[j].Get_Regret(); });
		sort(M_Interventions_Risk_Temp.begin(), M_Interventions_Risk_Temp.end(),
			[&](size_t i, size_t j) { return M_Interventions[i].Get_Min_Risk_Temp() > M_Interventions[j].Get_Min_Risk_Temp(); });

	}*/
};
void Maintenance::ModificarRiskTemp(int Int)

{
//	if (Int == 137 && M_I_T_Solution.size()==46)
//		int kk = 9;
	if (M_Interventions[Int].Get_T_Ordenados_Risk_Temp().size() == 0)
		if (M_Silent != true) cout << "No se puede colocar" << endl;

	double Regret = 0;
	double Min_Temp = __LONG_MAX__;
	bool primero = true;
	bool stop = false;
	if (M_Interventions[Int].Get_T_Ordenados_RiskQ_Temp().size() > 0)
	{
		list<int> ::iterator it = M_Interventions[Int].Get_T_Ordenados_Risk_Temp().begin();
		do
		{

			if (PuedoColocar(Int, (*it)) == false)
			{

				int borrado = (*it);
				it = M_Interventions[Int].Get_T_Ordenados_Risk_Temp().erase(it);
				M_Interventions[Int].Get_T_Ordenados_RiskQ_Temp().erase(find(M_Interventions[Int].Get_T_Ordenados_RiskQ_Temp().begin(), M_Interventions[Int].Get_T_Ordenados_RiskQ_Temp().end(), borrado));

				if (M_Interventions[Int].Get_T_Ordenados_Risk_Temp().size() == 0)
					stop = true;
				continue;
			}

			if (primero == true)
			{
				Min_Temp = M_Interventions[Int].Get_RiskC((*it));
			}
			else
			{
				Regret = M_Interventions[Int].Get_RiskC((*it)) - Min_Temp;
				stop = true;
			}
			primero = false;

			it++;

		} while (it != M_Interventions[Int].Get_T_Ordenados_Risk_Temp().end() && stop != true);
	}
	double RegretQ = 0;
	double Min_TempQ = __FLT_MAX__;
	bool primeroQ = true;
	bool stopQ = false;
	if (M_Interventions[Int].Get_T_Ordenados_RiskQ_Temp().size() > 0)
	{
		list<int> ::iterator it = M_Interventions[Int].Get_T_Ordenados_RiskQ_Temp().begin();
		do
		{
//			if (Int == 7 && (*it) == 71)
//				int kk = 9;
			if (PuedoColocar(Int, (*it)) == false)
			{
				int borrado = (*it);
//				if (M_Interventions[Int].Get_T_Ordenados_RiskQ_Temp().size() != M_Interventions[Int].Get_T_Ordenados_Risk_Temp().size())
//					int kk = 9;
				it = M_Interventions[Int].Get_T_Ordenados_RiskQ_Temp().erase(it);
				M_Interventions[Int].Get_T_Ordenados_Risk_Temp().erase(find(M_Interventions[Int].Get_T_Ordenados_Risk_Temp().begin(), M_Interventions[Int].Get_T_Ordenados_Risk_Temp().end(), borrado));
//				if (M_Interventions[Int].Get_T_Ordenados_RiskQ_Temp().size() != M_Interventions[Int].Get_T_Ordenados_Risk_Temp().size())
//					int kk = 9;
				if (M_Interventions[Int].Get_T_Ordenados_RiskQ_Temp().size() == 0)
					stopQ = true;
				continue;
			}

			if (primeroQ == true)
			{
				Min_TempQ = M_Interventions[Int].Get_RiskQuantile((*it));
			}
			else
			{
				RegretQ = M_Interventions[Int].Get_RiskQuantile((*it)) - Min_TempQ;
				stopQ = true;
			}
			primeroQ = false;

			it++;

		} while (it != M_Interventions[Int].Get_T_Ordenados_RiskQ_Temp().end() && stopQ != true);
	}
	double minwC = __FLT_MAX__;
	if (M_Tipo == 12)
	{
		for (list<int> ::iterator it= M_Interventions[Int].Get_T_Ordenados_Risk_Temp().begin();it!= M_Interventions[Int].Get_T_Ordenados_Risk_Temp().end() && minwC>M_Interventions[Int].Get_Min_Workload();it++)
		{
			if (M_Interventions[Int].Get_WorkloadC((*it)) < minwC)
				minwC = M_Interventions[Int].Get_WorkloadC((*it));
		}
		M_Interventions[Int].Set_Min_Workload_Temp(minwC);
	}

	M_Interventions[Int].Set_Min_Risk_Temp(Min_Temp);
	M_Interventions[Int].Set_Regret(Regret);
	M_Interventions[Int].Set_Min_RiskQ_Temp(Min_TempQ);
	M_Interventions[Int].Set_RegretQ(RegretQ);
//	if (M_Interventions[Int].Get_Min_Risk_Temp() < M_Interventions[Int].Get_Min_Risk())
//		int kk = 9;
	
};
bool Intervention::MinEntre(int ini, int fin)
{
	if (I_T_ordenados_Risk_Temp.size() == 1)
	{
		if ((*I_T_ordenados_Risk_Temp.begin()) > fin) return false;
		if ((*I_T_ordenados_Risk_Temp.begin()) + I_Delta[(*I_T_ordenados_Risk_Temp.begin())] < ini) return false;
		return true;
	}
	else
	{
		list<int> ::iterator itn = I_T_ordenados_Risk_Temp.begin();
		itn++;
		if ((((*I_T_ordenados_Risk_Temp.begin()) > fin) ||
			((*I_T_ordenados_Risk_Temp.begin()) + I_Delta[(*I_T_ordenados_Risk_Temp.begin())] < ini))
			&& (((*(itn)) > fin) ||
				((*(itn)) + I_Delta[(*(itn))] < ini)))
			return false;
	}
	return true;

};

void Maintenance::QuitarRecursos(int n_int)
{
	int telegido = M_Interventions[n_int].Get_T_Temp();
	for (vector<int> ::iterator it = M_Interventions[n_int].Get_Workload_Res().begin(); it != M_Interventions[n_int].Get_Workload_Res().end(); it++)
	{

//	for (map<int, map<int, map <int, double>>> ::iterator it = M_Interventions[n_int].Get_Workload().begin(); it != M_Interventions[n_int].Get_Workload().end(); it++)
//	{
		//Recursos
		for (int tprima = telegido; tprima < (telegido + M_Interventions[n_int].Get_Delta(telegido)); tprima++)
		{

			M_Resources[(*it)].Subtract(M_Interventions[n_int].Get_WorkloadN((*it),telegido,tprima), tprima);
			//			if (M_Interventions[n_int].Get_Workload()[(*it).first][tprima][t] > 0 && (*it).first == 1 && tprima==6)
			//				cout << M_I_Name[n_int] << " " << tprima << " " << t << " " << M_R_Name[(*it).first] << " " << M_Interventions[n_int].Get_Workload()[(*it).first][tprima][t] << " Temp "<< M_Resources[(*it).first].Get_Temp()[tprima-1]<<  endl;
//			if (M_Resources[(*it)].Get_Temp(tprima) < 0)
//				int kk = 9;
		}

	}
}
void Maintenance::AddRecursos(int n_int)
{
	int telegido = M_Interventions[n_int].Get_T_Temp();
//	for (map<int, map<int, map <int, double>>> ::iterator it = M_Interventions[n_int].Get_Workload().begin(); it != M_Interventions[n_int].Get_Workload().end(); it++)
//	{
		for (vector<int> ::iterator it = M_Interventions[n_int].Get_Workload_Res().begin(); it != M_Interventions[n_int].Get_Workload_Res().end(); it++)
		{
			//Recursos
		for (int tprima = telegido; tprima < (telegido + M_Interventions[n_int].Get_Delta(telegido)); tprima++)
		{

			M_Resources[(*it)].Add(M_Interventions[n_int].Get_WorkloadN((*it),telegido,tprima), tprima);
			//			if (M_Interventions[n_int].Get_Workload()[(*it).first][tprima][t] > 0 && (*it).first == 1 && tprima==6)
			//				cout << M_I_Name[n_int] << " " << tprima << " " << t << " " << M_R_Name[(*it).first] << " " << M_Interventions[n_int].Get_Workload()[(*it).first][tprima][t] << " Temp "<< M_Resources[(*it).first].Get_Temp()[tprima-1]<<  endl;

		}

	}
}
void Maintenance::ColocarIEnTiempo(int n_int, int telegido)
{
	M_NI_Colocadas++;
//	pair<int, int> p(n_int, telegido);
//	M_I_T_Solution.push_back(p);
	M_I_T[n_int] = telegido;
	//Pongo la season en la que pongo la pieza
	M_I_Seasons[n_int] = M_T_Season[telegido];
	M_Interventions[n_int].Set_T_Temp(telegido);
//	printf("Entra 21");
	AddRecursos(n_int);
	AddScenarios(n_int);
//	printf("Sale 21");
//	printf("Entra 32");

	M_Interventions_Colocada[n_int] = true;
	M_Perdida_Parcial += (M_Interventions[n_int].Get_RiskC()[telegido] - M_Interventions[n_int].Get_Min_Risk());
	M_fo_first += M_Interventions[n_int].Get_RiskC()[telegido];
	M_fo_temp += M_Interventions[n_int].Get_RiskC()[telegido];
	M_Waste += M_Interventions[n_int].Get_RiskC()[telegido] - M_Interventions[n_int].Get_Min_Risk();
	FuncionObjetivoTemp(M_Interventions[n_int].Get_T_Temp(), M_Interventions[n_int].Get_T_F_Temp());
//	printf("Sale 32");
	//Calcular el riesgo que meto para ver si es correcto
	ComprobarRiskTemp(n_int);
	//	M_Interventions[n_int].Set_Min_Risk_Temp(0);
//	VerifySolution();


}
double Maintenance::SegundaParteFObj(int numero_i,int timeini,int timefin,double val_ini)
{
	if (M_Pool == false)
		return 0;
	//parte antigua
	double fobjetivo_second = 0;

	for (int i = timeini; i < timefin; i++)
	{

		fobjetivo_second += M_vfo2_temp[i];

	}
	double fobjetivo_second_new = 0;
	if (fobjetivo_second < __FLT_EPSILON__ && M_T_Temp_Risk_S[timeini][0] < __FLT_EPSILON__ && M_T_Temp_Risk_S[timeini][M_Scenarios_number[timeini] - 1] < __FLT_EPSILON__ )
	{

		fobjetivo_second_new = M_Interventions[numero_i].Get_RiskExcess(timeini);
	}
	else
	{
		//parte nueva
//		AddScenarios(numero_i, timeini, timefin);

		//	double fobjetivo_second_new = 0;

		for (register int i = timeini; i < timefin; i++)
		{
			// __FLT_EPSILON__
			if (M_vfo2_temp[i] < __FLT_EPSILON__ && M_T_Temp_Risk_S[i][0] < __FLT_EPSILON__  && M_T_Temp_Risk_S[i][M_Scenarios_number[i]-1]< __FLT_EPSILON__)
			{
				fobjetivo_second_new+= M_Interventions[numero_i].Get_Riskexcess(timeini,i);
			
			}
			else
			{
				int pos = 0;

				//			set<double> temp2;
				//			double suma2 = 0;

				vector<double> temp(M_Scenarios_number[i]);
				double suma = 0;
				if (M_Scenarios_number[i] > 1)
				{
					pos = ceil(M_Scenarios_number[i] * M_Quantile) - 1;

					for (register int k = 0; k < M_Scenarios_number[i]; k++)
					{
						temp[k] = (M_T_Temp_Risk_S[i][k] + M_Interventions[numero_i].Get_RiskN(i, timeini, k));
						suma += temp[k];
						//					temp2.insert(temp[k]);

					}
				}
				else continue;
				//			double suma = accumulate(temp.begin(), temp.end(), 0.0);
				double media = suma / M_Scenarios_number[i];
				nth_element(temp.begin(), temp.begin() + pos, temp.end());
				//		double cuantil1= M_T_Temp_Risk_S[i][pos];
				//		 sort(M_T_Temp_Risk_S[i].begin(), M_T_Temp_Risk_S[i].end());
				double cuantil = temp[pos];
				//			double cuantil2 = *(next(temp2.begin(), pos));
				//			if ((cuantil<cuantil2 - __FLT_EPSILON__) || (cuantil > cuantil2 + __FLT_EPSILON__))
				//				Problema(123);
				double excess = max((double)0, (cuantil - media));
				//		fobjetivo_first3 += media;
				fobjetivo_second_new += excess;
			}

			if ((fobjetivo_second_new - fobjetivo_second) > (val_ini-__FLT_EPSILON__))
				return __FLT_MAX__;

		}

	}
	return (fobjetivo_second_new - fobjetivo_second);

}
int Maintenance::ElegirMayorTFuncionObjetivo(int num_int)
{
	bool stop = false;
	vector<int> vec_temp;

	double Min = __FLT_MAX__;
	int elegido = -1;
	list<int> ::iterator it = M_Interventions[num_int].Get_T_Ordenados_Risk_Temp().begin();
	do
	{
		
		double fobj1 = M_Interventions[num_int].Get_RiskC((*it));
		double fobj2 = 0;
		if (M_Alpha * fobj1 > (Min+((double)(get_random(0,2)-10000)/(double)10000)))
		{
			stop = true;
			break;
		}

		if ( PuedoColocar(num_int, (*it)))
		{
			fobj2 = SegundaParteFObj(num_int, (*it), (*it) + M_Interventions[num_int].Get_Delta((*it)),(Min-(M_Alpha * fobj1))/(1-M_Alpha));
			//		if ((M_Interventions[n_int].Get_RiskC()[(*it)] > (min + __FLT_EPSILON__)) || vec_temp.size() > 1)
			if ((M_Alpha*fobj1 + (1-M_Alpha)*fobj2 < Min))
			{
				elegido = (*it);
				Min = M_Alpha * fobj1 + (1 - M_Alpha) * fobj2;
			}
			it++;

		}
		else
		{
//			if (M_Interventions[num_int].Get_T_Ordenados_RiskQ_Temp().size() != M_Interventions[num_int].Get_T_Ordenados_Risk_Temp().size())
//				int kk = 9;
			int borrado = (*it);
			M_Interventions[num_int].Get_T_Ordenados_RiskQ_Temp().erase(find(M_Interventions[num_int].Get_T_Ordenados_RiskQ_Temp().begin(), M_Interventions[num_int].Get_T_Ordenados_RiskQ_Temp().end(), borrado));

			it = M_Interventions[num_int].Get_T_Ordenados_Risk_Temp().erase(it);
//			if (M_Interventions[num_int].Get_T_Ordenados_RiskQ_Temp().size() != M_Interventions[num_int].Get_T_Ordenados_Risk_Temp().size())
//				int kk = 9;
			if (M_Interventions[num_int].Get_T_Ordenados_Risk_Temp().size() == 0)
				stop = true;
		}


	

	} while (it != M_Interventions[num_int].Get_T_Ordenados_Risk_Temp().end() && stop != true);


	return elegido;
}
int Maintenance::ElegirMayorTFuncionObjetivoAbsoluto(int num_int)
{

	vector<int> vec_temp;
	double Min = __FLT_MAX__;
	int elegido = -1;
	int index_anterior = 0;
	int ele_cont = 0;
	int cont = 0;
	for (list<int> ::iterator it = M_Interventions[num_int].Get_T_Ordenados_Risk_I().begin(); it != M_Interventions[num_int].Get_T_Ordenados_Risk_I().end(); it++)
	{

		if (((*it) - index_anterior) > 1)
		{
//			if (((*it) - index_anterior) > 10)
//				int kk = 9;
			//			if (it == I_T_ordenados_Risk_I.begin())
			//			random_shuffle(M_Interventions_MaxDifR.begin(), M_Interventions_MaxDifR.begin() + (*it));

			random_shuffle(M_Interventions[num_int].Get_T_Ordenados_Risk().begin() + index_anterior, M_Interventions[num_int].Get_T_Ordenados_Risk().begin() + (*it));
		}

		bool stop = false;
		for (int pos = index_anterior; pos < (*it) ; pos++)
		{
			int pos_t = M_Interventions[num_int].Get_T_Ordenados_Risk()[pos];
			double fobj1 = M_Interventions[num_int].Get_RiskC(pos_t);
			double fobj2 = 0;
			if (M_Alpha*fobj1 > (Min - __FLT_EPSILON__))
			{
				stop = true;
				return elegido;

			}

			if (PuedoColocar(num_int, pos_t))
			{
				cont++;
				if (M_Iter%2==0 && elegido>=0 && get_random(0,2)<=1)
				continue;
//				if (3 * cont > M_T)
//					break;
				fobj2 = SegundaParteFObj(num_int, pos_t, pos_t + M_Interventions[num_int].Get_Delta(pos_t),  (Min - (M_Alpha * fobj1)) / (1 - M_Alpha));
					//		if ((M_Interventions[n_int].Get_RiskC()[(*it)] > (min + __FLT_EPSILON__)) || vec_temp.size() > 1)
				if ((M_Alpha*fobj1 + (1-M_Alpha)*fobj2 < (Min - __FLT_EPSILON__)))
				{
					ele_cont = cont;
					elegido = pos_t;
					Min = M_Alpha * fobj1 + (1 - M_Alpha) * fobj2;
					Min =((double)get_random(990,1010)/(double)1000) * Min;
				}
			}

		}
		index_anterior = (*it);
		
	}



	return elegido;
}
void Maintenance::ColocarIEnTiempoValor(int n_int)
{
//	if (n_int == 30)
//		int kk = 9;
	double min = M_Interventions[n_int].Get_Min_Risk_Temp();
//	double val = min*(1+((100-M_Delta)/100));

	double val = min + (max(M_Best_fo - (M_Perdida_Parcial * (M_Alpha/M_T)) -max(M_Cota, M_Alpha * (M_fo_temp / M_T)), (double)0)) * (M_T / M_Alpha);
//	double val = min + (__max((((100 - 0) / 100) * (M_Best_fo - __max(M_Cota+M_Cota_Parcial, M_Alpha * (M_fo_temp / M_T)))),0))*(M_T/M_Alpha);

//	if ((val - min) > (M_Best_fo - max(M_Cota,M_Alpha*(M_fo_temp/M_T))))
//		int kk = 9;
//	if (M_Determinista == false)
//		int kk = 9;
	if (M_Determinista) val = min;
//	val = min;
	bool stop = false;
	vector<int> vec_temp;
	list<int> ::iterator it = M_Interventions[n_int].Get_T_Ordenados_Risk_Temp().begin();
	do
	{

//		if ((M_Interventions[n_int].Get_RiskC()[(*it)] > (min + __FLT_EPSILON__)) || vec_temp.size() > 1)
		if ((M_Interventions[n_int].Get_RiskC()[(*it)] > (val + __FLT_EPSILON__)) )
		{
			stop = true;
			break;
		}
		if (vec_temp.size() <= 1)
		{
			vec_temp.push_back((*it));
			it++;
		}
		else
		{
			if (PuedoColocar(n_int, (*it)))
			{
				vec_temp.push_back((*it));
				it++;
				
			}
			else
			{
				int borrado = (*it);
				M_Interventions[n_int].Get_T_Ordenados_RiskQ_Temp().erase(find(M_Interventions[n_int].Get_T_Ordenados_RiskQ_Temp().begin(), M_Interventions[n_int].Get_T_Ordenados_RiskQ_Temp().end(), borrado));

				it = M_Interventions[n_int].Get_T_Ordenados_Risk_Temp().erase(it);
				if (M_Interventions[n_int].Get_T_Ordenados_Risk_Temp().size() == 0)
					stop = true;
			}


		}
		
	}while (it != M_Interventions[n_int].Get_T_Ordenados_Risk_Temp().end() && stop != true);


	random_shuffle(vec_temp.begin(), vec_temp.end());

	int telegido = vec_temp[0];

	ColocarIEnTiempo(n_int, telegido);
	/*
	for (map<int, map<int, map <int, double>>> ::iterator it = M_Interventions[n_int].Get_Workload().begin(); it != M_Interventions[n_int].Get_Workload().end(); it++)
	{
		//Recursos
		for (int tprima = telegido; tprima < (telegido + M_Interventions[n_int].Get_Delta(telegido)); tprima++)
		{

			M_Resources[(*it).first].Add(M_Interventions[n_int].Get_Workload()[(*it).first][tprima][telegido], tprima);
			//			if (M_Interventions[n_int].Get_Workload()[(*it).first][tprima][t] > 0 && (*it).first == 1 && tprima==6)
			//				cout << M_I_Name[n_int] << " " << tprima << " " << t << " " << M_R_Name[(*it).first] << " " << M_Interventions[n_int].Get_Workload()[(*it).first][tprima][t] << " Temp "<< M_Resources[(*it).first].Get_Temp()[tprima-1]<<  endl;

		}

	}
	pair<int, int> p(n_int, telegido);
	M_I_T_Solution.push_back(p);
	//Pongo la season en la que pongo la pieza
	M_I_Seasons[n_int] = M_T_Season[telegido];
	M_Interventions[n_int].Set_T_Temp(telegido);
	M_Interventions_T[n_int] = telegido;
	M_Interventions_Colocada[n_int] = true;
	M_fo_first += M_Interventions[n_int].Get_RiskC()[telegido];
	M_fo_temp+= M_Interventions[n_int].Get_RiskC()[telegido];*/
	//Calcular el riesgo que meto para ver si es correcto
	ComprobarRiskTemp(n_int);
//	M_Interventions[n_int].Set_Min_Risk_Temp(0);
	VerifySolution();
	
}
void Maintenance::ColocarIEnTiempoSPG(int n_int)
{


	//	val = min;

	vector<int> vec_temp;
	int telegido = -1;
	bool primero = false;
	int treserva = (-1);


	double min = M_Interventions[n_int].Get_Min_Risk_Temp();

	double val = min + (max(M_Best_fo - (M_Perdida_Parcial * (M_Alpha / M_T)) - max(M_Cota, M_Alpha * (M_fo_temp / M_T)),(double) 0)) * (M_T / M_Alpha);
	//	double val = min + (__max((((100 - 0) / 100) * (M_Best_fo - __max(M_Cota+M_Cota_Parcial, M_Alpha * (M_fo_temp / M_T)))),0))*(M_T/M_Alpha);

	if (M_Determinista) val = min;
	for (list<int> ::iterator it = M_Interventions[n_int].Get_T_Ordenados_Risk_Temp().begin();telegido<0 && it != M_Interventions[n_int].Get_T_Ordenados_Risk_Temp().end(); it++)
	{
		if ( M_Interventions[n_int].Get_RiskC((*it))>(val+__FLT_EPSILON__))
			continue;
		if (primero==false && PuedoColocar(n_int, (*it)))
		{
			treserva = (*it);
			primero = true;
		}
		if (get_random(0, 100) > M_Delta)
			continue;
		if (PuedoColocar(n_int, (*it)))
		{
			telegido = (*it);
		}
	}
	if (telegido < 0)
		telegido = treserva;

	ColocarIEnTiempo(n_int, telegido);
	/*
	for (map<int, map<int, map <int, double>>> ::iterator it = M_Interventions[n_int].Get_Workload().begin(); it != M_Interventions[n_int].Get_Workload().end(); it++)
	{
		//Recursos
		for (int tprima = telegido; tprima < (telegido + M_Interventions[n_int].Get_Delta(telegido)); tprima++)
		{

			M_Resources[(*it).first].Add(M_Interventions[n_int].Get_Workload()[(*it).first][tprima][telegido], tprima);
			//			if (M_Interventions[n_int].Get_Workload()[(*it).first][tprima][t] > 0 && (*it).first == 1 && tprima==6)
			//				cout << M_I_Name[n_int] << " " << tprima << " " << t << " " << M_R_Name[(*it).first] << " " << M_Interventions[n_int].Get_Workload()[(*it).first][tprima][t] << " Temp "<< M_Resources[(*it).first].Get_Temp()[tprima-1]<<  endl;

		}

	}
	pair<int, int> p(n_int, telegido);
	M_I_T_Solution.push_back(p);
	//Pongo la season en la que pongo la pieza
	M_I_Seasons[n_int] = M_T_Season[telegido];
	M_Interventions[n_int].Set_T_Temp(telegido);
	M_Interventions_T[n_int] = telegido;
	M_Interventions_Colocada[n_int] = true;
	M_fo_first += M_Interventions[n_int].Get_RiskC()[telegido];
	M_fo_temp+= M_Interventions[n_int].Get_RiskC()[telegido];*/
	//Calcular el riesgo que meto para ver si es correcto
	ComprobarRiskTemp(n_int);
	//	M_Interventions[n_int].Set_Min_Risk_Temp(0);
	VerifySolution();

}
void Maintenance::ColocarIEnBestTiempo(int n_int)
{

//	double min = M_Interventions[n_int].Get_Min_Risk_Temp();


	vector<int> vec_temp;
	list<int> ::iterator it2 = M_Interventions[n_int].Get_T_Ordenados_Risk_Temp().begin();
	int telegido = (*it2);
	int told = M_Interventions[n_int].Get_T_Temp();
	//Reducir el workload dónde estaba y añadir el nuevo
	for (vector<int> ::iterator it = M_Interventions[n_int].Get_Workload_Res().begin(); it != M_Interventions[n_int].Get_Workload_Res().end(); it++)
	{

//	for (map<int, map<int, map <int, double>>> ::iterator it = M_Interventions[n_int].Get_Workload().begin(); it != M_Interventions[n_int].Get_Workload().end(); it++)
//	{
		//Recursos
		for (int tprima = told; tprima < (told + M_Interventions[n_int].Get_Delta()[told]); tprima++)
		{

			M_Resources[(*it)].Subtract(M_Interventions[n_int].Get_WorkloadN((*it),told,tprima), tprima);
			//			if (M_Interventions[n_int].Get_Workload()[(*it).first][tprima][t] > 0 && (*it).first == 1 && tprima==6)
			//				cout << M_I_Name[n_int] << " " << tprima << " " << t << " " << M_R_Name[(*it).first] << " " << M_Interventions[n_int].Get_Workload()[(*it).first][tprima][t] << " Temp "<< M_Resources[(*it).first].Get_Temp()[tprima-1]<<  endl;

		}

	}
	for (vector<int> ::iterator it = M_Interventions[n_int].Get_Workload_Res().begin(); it != M_Interventions[n_int].Get_Workload_Res().end(); it++)
	{

//	for (map<int, map<int, map <int, double>>> ::iterator it = M_Interventions[n_int].Get_Workload().begin(); it != M_Interventions[n_int].Get_Workload().end(); it++)
//	{
		//Recursos
		for (int tprima = telegido; tprima < (telegido + M_Interventions[n_int].Get_Delta(telegido)); tprima++)
		{

			M_Resources[(*it)].Add(M_Interventions[n_int].Get_WorkloadN((*it), telegido, tprima), tprima); //[(*it).first] [tprima] [telegido]
			//			if (M_Interventions[n_int].Get_Workload()[(*it).first][tprima][t] > 0 && (*it).first == 1 && tprima==6)
			//				cout << M_I_Name[n_int] << " " << tprima << " " << t << " " << M_R_Name[(*it).first] << " " << M_Interventions[n_int].Get_Workload()[(*it).first][tprima][t] << " Temp "<< M_Resources[(*it).first].Get_Temp()[tprima-1]<<  endl;

		}

	}
	for (list<pair<int, int>>::iterator it = M_I_T_Solution.begin(); it != M_I_T_Solution.end(); it++)
	{
		if ((*it).first == n_int)
			(*it).second = telegido;
	}

	//Pongo la season en la que pongo la pieza
	M_I_Seasons[n_int] = M_T_Season[telegido];
	M_Interventions[n_int].Set_T_Temp(telegido);
	M_I_T[n_int] = telegido;
//	M_Interventions_T[n_int] = telegido;
	M_Interventions_Colocada[n_int] = true;
	M_fo_first += M_Interventions[n_int].Get_RiskC()[telegido];
	M_fo_temp += M_Interventions[n_int].Get_RiskC()[telegido];
	M_fo_first -= M_Interventions[n_int].Get_RiskC()[told];
	M_fo_temp -= M_Interventions[n_int].Get_RiskC()[told];
	//Calcular el riesgo que meto para ver si es correcto
	ComprobarRiskTemp(n_int);
	//	M_Interventions[n_int].Set_Min_Risk_Temp(0);
	VerifySolution();

}
//comprueba si el riesgo mínimo está bien calculado
void Maintenance::ComprobarRiskTemp(int n_int)
{
	return;
	LimpiarRiskTempS();
	double acumuladoT = 0;
	for (int t = M_Interventions[n_int].Get_T_Temp(); t < M_Interventions[n_int].Get_T_F_Temp(); t++)
	{
		double acumuladoT2 = 0;
		for (int si = 0; si < M_Interventions[n_int].Get_Risk()[t][M_Interventions[n_int].Get_T_Temp()].size(); si++)
		{
			acumuladoT2 += M_Interventions[n_int].Get_Risk()[t][M_Interventions[n_int].Get_T_Temp()][si];
			//				InsertarOrdenadoTRisk(t, M_Interventions[i].Get_Risk()[t][M_Interventions[i].Get_T_Temp()][si]);

			M_T_Temp_Risk_S[t][si] += M_Interventions[n_int].Get_Risk()[t][M_Interventions[n_int].Get_T_Temp()][si];

			//			kktot += M_Interventions[n_int].Get_Risk()[t][M_Interventions[i].Get_T_Temp()][si];
		}
		acumuladoT2 = acumuladoT2 / M_Interventions[n_int].Get_Risk()[t][M_Interventions[n_int].Get_T_Temp()].size();
		acumuladoT += acumuladoT2;
	}
	if (M_Interventions[n_int].Get_RiskC()[M_Interventions[n_int].Get_T_Temp()] != acumuladoT)
		Problema(15);
}
void Maintenance::LimpiarRiskTempS()
{
	for (int i = 0; i < M_T; i++)
	{
		for (int j = 0; j < M_Scenarios_number[i]; j++)
		{
			M_T_Temp_Risk_S[i][j] = 0;

		}
	}
}
void Maintenance::LimpiarRiskTempS2()
{
	for (int i = 0; i < M_T; i++)
	{
		for (int j = 0; j < M_Scenarios_number[i]; j++)
		{
			M_T_Temp_Risk_S2[i][j] = 0;

		}
	}
}
void Maintenance::LimpiarVectores()
{
	M_NI_Colocadas = 0;
	M_Waste = 0;
	M_fo_first = 0;
	M_fo_temp = 0;


	M_Perdida_Parcial = 0;

	for (int  i2 = 0; i2 < M_Resources.size(); i2++)
	{
		M_Resources[i2].Clean();
	}
//	printf("Size %d %d %d %d", M_I_Seasons.size(), M_I_T.size(), M_Interventions_Colocada.size(), M_vfo2_temp.size());
	for (int i = 0; i < M_N_Interventions; i++)
	{
		M_I_Seasons[i]=(-1);
		M_Interventions_Colocada[i]=false;
		M_I_T[i] = -1;

		M_Interventions[i].Original(false);

	}
	for (int i1 = 0; i1 < M_T; i1++)
	{
		M_vfo2_temp[i1] = 0;
	}
}

void Intervention::Original(bool ordenado)
{
	I_Regret_Temp = I_Regret;
	I_Min_Risk_Temp=I_Min_Risk;
	I_RegretQ_Temp = I_RegretQ;
	I_Min_RiskQ_Temp = I_Min_RiskQ;
	I_T_ordenados_Risk_Temp.clear();
	I_T_ordenados_RiskQ_Temp.clear();
	int index_anterior = 0;
	for (list<int> ::iterator it = I_T_ordenados_Risk_I.begin(); it != I_T_ordenados_Risk_I.end(); it++)
	{

		if (((*it) - index_anterior) > 1)
		{
			//			if (it == I_T_ordenados_Risk_I.begin())
			//			random_shuffle(M_Interventions_MaxDifR.begin(), M_Interventions_MaxDifR.begin() + (*it));
			if (ordenado)
				sort(I_T_ordenados_Risk.begin() + index_anterior, I_T_ordenados_Risk.begin() + (*it));
			else
				random_shuffle(I_T_ordenados_Risk.begin() + index_anterior, I_T_ordenados_Risk.begin() + (*it));
			for (int i = index_anterior; i < (*it); i++)
			{
				I_T_ordenados_Risk_Temp.push_back(I_T_ordenados_Risk[i]);
			}
		}
		else
			I_T_ordenados_Risk_Temp.push_back(I_T_ordenados_Risk[index_anterior]);
		index_anterior = (*it);

	}
	index_anterior = 0;
	for (list<int> ::iterator it = I_T_ordenados_Risk_IQ.begin(); it != I_T_ordenados_Risk_IQ.end(); it++)
	{
		if (((*it) - index_anterior) > 1)
		{
			//			if (it == I_T_ordenados_Risk_I.begin())
			//			random_shuffle(M_Interventions_MaxDifR.begin(), M_Interventions_MaxDifR.begin() + (*it));
			if (ordenado)
				sort(I_T_ordenados_RiskQ.begin() + index_anterior, I_T_ordenados_RiskQ.begin() + (*it));
			else
				random_shuffle(I_T_ordenados_RiskQ.begin() + index_anterior, I_T_ordenados_RiskQ.begin() + (*it));
			for (int i = index_anterior; i < (*it); i++)
			{
				I_T_ordenados_RiskQ_Temp.push_back(I_T_ordenados_RiskQ[i]);
			}
		}
		else
			I_T_ordenados_RiskQ_Temp.push_back(I_T_ordenados_RiskQ[index_anterior]);
		index_anterior = (*it);

	}
}
bool Maintenance::DestruirYReparar(void) //Si tipo es 0 es por diferencia, si es 1 por dif de workload, si es 2 por min R, si es 3 por min W
{

	//Primera opción
	//Coger las interventions ordenadas de mayor a menor por riesgo minimo y poner donde menor riesgo tenga
	//y sea posible

//	M_I_Seasons.clear();
	M_Silent = true;
	LimpiarVectores();
	M_Iterations = 10000;
//	srand(M_seed);

	M_Tipo = 2;
	M_Iter = 0;
	double tmp_first = 0;
	do
	{
		M_Delta = get_random(1, 20);
		bool terminado = false;
		int intentos = 0;
		do
		{

			M_Tipo = get_random(0, 8);
			LimpiarVectores();
			M_Tipo = AleatorizarIgualesListInterventions(M_Tipo);
			M_ConstruirConRecursos = false;
			//Construir solucion


			//	for (vector<Intervention>::iterator it = M_Interventions.begin(); it != M_Interventions.end(); it++)
			bool colocado = true;
			int choose = -1;

			int choose_elegido = -1;
			bool stop = false;

			terminado = false;


			do
			{
				bool primero = false;
				bool elegido = false;
				for (int i = 0; i < M_Interventions.size() && elegido != true; i++)
				{
					choose = IndiceTipo(M_Tipo, i);
					int alea = 0;
					if (M_Interventions_Colocada[choose] == true) continue;
					alea = get_random(0, M_Delta);
					//			if (alea ==M_Delta && primero==true) continue;
					//			if (primero == true || alea!=M_Delta) elegido = true;
					if (alea >= 1 && primero == true) continue;
					if (primero == true || alea == 0) elegido = true;

					primero = true;

					choose_elegido = choose;


				}

				colocado = ColocarEnTiempoRGreedy(choose_elegido);
				//			colocado = ColocarEnTiempo(choose_elegido);

				if (colocado == true)
				{
					M_Interventions_Colocada[choose_elegido] = true;
				}
				else
				{
					if (!M_Silent) cout << "No puede colocar  la intervention " << choose_elegido << endl << endl;
					if (M_Tipo!=9) M_Interventions_Out[choose_elegido] += M_T;
					return false;
				}
				if (M_NI_Colocadas == M_N_Interventions)
				{

					double val = FuncionObjetivo();
					stop = true;
					if ((val < M_Best_fo) || intentos > 2)
						terminado = true;
					else
						intentos++;
				}
			} while (stop != true);



		} while (terminado != true && intentos < 3);
	//Quitar  solucion
		random_shuffle(M_Vec_Alea_T.begin(), M_Vec_Alea_T.end());
		random_shuffle(M_Vec_Alea_I.begin(), M_Vec_Alea_I.end());
		//Tengo que elegir uno que sobrepase los recursos y cambiarlo a otro sitio
		bool quitado = false;
		//ahora quito los que no estén en su mínimo
		do
		{
			int elegido = -1;
			quitado = false;
			for (int j = 0; j < M_N_Interventions; j++)
			{
				if (M_Interventions_Colocada[j] == false) continue;
				if (M_Interventions[j].Get_Min_Risk() >= M_Interventions[j].Get_RiskC()[M_Interventions[j].Get_T_Temp()])
					continue;
				elegido = j;
				//Quito también los recursos


				for (map<int, map<int, map <int, double>>> ::iterator itt = M_Interventions[elegido].Get_Workload().begin(); itt != M_Interventions[elegido].Get_Workload().end(); itt++)
				{
					for (int tprima = M_Interventions[elegido].Get_T_Temp(); tprima < (M_Interventions[elegido].Get_T_Temp() + M_Interventions[elegido].Get_Delta()[M_Interventions[elegido].Get_T_Temp()]); tprima++)
					{
						M_Resources[(*itt).first].Subtract(M_Interventions[elegido].Get_Workload()[(*itt).first][tprima][M_Interventions[elegido].Get_T_Temp()], tprima);

					}
				}
				M_Interventions_Colocada[elegido] = false;
//				Borrar(elegido, M_Interventions_T[elegido]);
				M_I_T[elegido] = -1;
				M_Interventions[elegido].Set_T_Temp(-1);

				//Pongo la season en la que pongo la pieza
				M_I_Seasons[elegido] = (-1);
				quitado = true;




			}
		} while (quitado == true);

		do
		{
			quitado = false;
			for (size_t j = 0; j < M_Resources.size(); j++)
			{
				for (vector<int> ::iterator it = M_Vec_Alea_T.begin(); it != M_Vec_Alea_T.end(); it++)
				{

					if (M_Resources[j].Get_Upper()[(*it)] < M_Resources[j].Get_Temp()[(*it)])
					{
						int elegido = BuscarIntervention((*it), (int) j);
						//Quito también los recursos
						

						for (map<int, map<int, map <int, double>>> ::iterator itt = M_Interventions[elegido].Get_Workload().begin(); itt != M_Interventions[elegido].Get_Workload().end(); itt++)
						{
							for (int tprima = M_Interventions[elegido].Get_T_Temp(); tprima < (M_Interventions[elegido].Get_T_Temp() + M_Interventions[elegido].Get_Delta()[M_Interventions[elegido].Get_T_Temp()]); tprima++)
							{

								M_Resources[(*itt).first].Subtract(M_Interventions[elegido].Get_Workload()[(*itt).first][tprima][M_Interventions[elegido].Get_T_Temp()], tprima);

							}
						}
						M_Interventions_Colocada[elegido] = false;
//						Borrar(elegido, M_Interventions_T[elegido]);
						M_I_T[elegido] = -1;
						M_Interventions[elegido].Set_T_Temp(-1);

						//Pongo la season en la que pongo la pieza
						M_I_Seasons[elegido] = (-1);
						quitado = true;

					}

				}
			}
		} while (quitado == true);

		Copy();
		M_ConstruirConRecursos = true;
		RepararSolutionCopy();
		M_Iter++;
		struct timeb t1;
		ftime(&t1);

		tmp_first = ((double)((t1.time - M_Time_Initial.time) * 1000 + t1.millitm - M_Time_Initial.millitm)) / 1000;

		if (M_Iter%100==0) cout << "Iter " << M_Iter << endl;
	} while (M_Iter < M_Iterations && tmp_first < M_ComputionTime * 60);

}
	void Maintenance::Borrar(int nint, int t)
	{
		for (list<pair<int, int>> ::iterator itp = M_I_T_Solution.begin(); itp != M_I_T_Solution.end(); itp++)
		{
			if (((*itp).first == nint) && ((*itp).second == (t)))
			{
				M_I_T_Solution.erase(itp);
				return;
			}

		}
		Problema(100);
		
	}
int Maintenance::BuscarIntervention(int time, int resource)
{
	for (vector<int>::iterator it = M_Vec_Alea_I.begin(); it != M_Vec_Alea_I.end(); it++)
	{
		if (M_Interventions_Colocada[(*it)] == false)
			continue;
		if (M_Interventions[(*it)].Get_T_Temp() > time || M_Interventions[(*it)].Get_T_F_Temp() < time)
			continue;
		if (M_Interventions[(*it)].Get_WorkloadN(resource, M_Interventions[(*it)].Get_T_Temp(),time)>0)
		{
			return (*it);
		}


	}
	return (-1);
}
void Maintenance::CopyListInterventionsTempBias()
{
	M_Temp_Item.clear();
	M_Temp_Weight.clear();
	switch (M_Tipo)
	{
	default:
		for (int i = 0; i < M_N_Interventions; i++)
		{
			M_Temp_Item.push_back(M_Interventions_MaxDifR[i].first);
			M_Temp_Weight.push_back(M_Interventions_MaxDifR[i].second);

		}
		break;
	case 1:
		for (int i = 0; i < M_N_Interventions; i++)
		{
			M_Temp_Item.push_back(M_Interventions_MaxDifW[i].first);
			M_Temp_Weight.push_back(M_Interventions_MaxDifW[i].second);

		}
		break;
	case 2:
		for (int i = 0; i < M_N_Interventions; i++)
		{
			M_Temp_Item.push_back(M_Interventions_MinR[i].first);
			M_Temp_Weight.push_back(M_Interventions_MinR[i].second);

		}
		break;
	case 3:
		for (int i = 0; i < M_N_Interventions; i++)
		{
			M_Temp_Item.push_back(M_Interventions_MinW[i].first);
			M_Temp_Weight.push_back(M_Interventions_MinW[i].second);

		}
		break;
	case 4:
		for (int i = 0; i < M_N_Interventions; i++)
		{
			M_Temp_Item.push_back(M_Interventions_MinWxT[i].first);
			M_Temp_Weight.push_back(M_Interventions_MinWxT[i].second);

		}
		break;
	case 5:
		for (int i = 0; i < M_N_Interventions; i++)
		{
			M_Temp_Item.push_back(M_Interventions_MinRxT[i].first);
			M_Temp_Weight.push_back(M_Interventions_MinRxT[i].second);

		}

		break;
	case 6:
		for (int i = 0; i < M_N_Interventions; i++)
		{
			M_Temp_Item.push_back(M_Interventions_Min_WXR[i].first);
			M_Temp_Weight.push_back(M_Interventions_Min_WXR[i].second);

		}
		break;
	case 7:
		for (int i = 0; i < M_N_Interventions; i++)
		{
			M_Temp_Item.push_back(M_Interventions_Min_WtXRt[i].first);
			M_Temp_Weight.push_back(M_Interventions_Min_WtXRt[i].second);

		}
		break;
	case 8:
		for (int i = 0; i < M_N_Interventions; i++)
		{
			M_Temp_Item.push_back(M_Interventions_Exclusions[i].first);
			M_Temp_Weight.push_back(M_Interventions_Exclusions[i].second);

		}
				break;
	case 9:
		for (int i = 0; i < M_N_Interventions; i++)
		{
			M_Temp_Item.push_back(i);
			M_Temp_Weight.push_back(M_Interventions_Out_Ordenada[i]);

		}

		break;
	}
	random_device rd;

	//
	// Engines 
	//
	mt19937 e2(rd());
	//knuth_b e2(rd());
	//default_random_engine e2(rd()) ;

	//
	// Distribtuions
	//

	double max = M_Temp_Weight[0];
	double min = M_Temp_Weight[M_N_Interventions - 1];
	uniform_real_distribution<> dist(min, max);
	for (int i = 0; i < M_N_Interventions; i++)
	{
		M_Temp_Weight[i] = M_Temp_Weight[i] * dist(e2);
	}

	sort(M_Temp_Item.begin(), M_Temp_Item.end(),
		[&](size_t i, size_t j) { return M_Temp_Weight[i] > M_Temp_Weight[j]; });
}
void Maintenance::CopyListInterventionsTemp()
{
	switch (M_Tipo)
	{
	default:
			for (int i = 0; i < M_N_Interventions; i++)
			{
				M_Interventions_Temp.push_back(M_Interventions_MaxDifR[i]);
			}
		break;
	case 1:
			for (int i = 0; i < M_N_Interventions; i++)
			{
				M_Interventions_Temp.push_back(M_Interventions_MaxDifW[i]);
			}
		break;
	case 2:
			for (int i = 0; i < M_N_Interventions; i++)
			{
				M_Interventions_Temp.push_back(M_Interventions_MinR[i]);
			}
		break;
	case 3:
			for (int i = 0; i < M_N_Interventions; i++)
			{
				M_Interventions_Temp.push_back(M_Interventions_MinW[i]);
			}
		break;
	case 4:
			for (int i = 0; i < M_N_Interventions; i++)
			{
				M_Interventions_Temp.push_back(M_Interventions_MinWxT[i]);
			}
		break;
	case 5:
			for (int i = 0; i < M_N_Interventions; i++)
			{
				M_Interventions_Temp.push_back(M_Interventions_MinRxT[i]);
			}

		break;
	case 6:
			for (int i = 0; i < M_N_Interventions; i++)
			{
				M_Interventions_Temp.push_back(M_Interventions_Min_WXR[i]);
			}
		break;
	case 7:
			for (int i = 0; i < M_N_Interventions; i++)
			{
				M_Interventions_Temp.push_back(M_Interventions_Min_WtXRt[i]);
			}

		break;
	}

}
bool Maintenance::ConstructiveRValor(int tipo) //Si tipo es 0 es por diferencia, si es 1 por dif de workload, si es 2 por min R, si es 3 por min W
{

	//Primera opción
	//Coger las interventions ordenadas de mayor a menor por riesgo minimo y poner donde menor riesgo tenga
	//y sea posible

	LimpiarVectores();
	M_Porcentaje_Ocupado = 90;

	M_Tipo = AleatorizarIgualesListInterventions(tipo);
	CopyListInterventionsTemp();
	cout << endl << tipo << endl;
	
	//	for (vector<Intervention>::iterator it = M_Interventions.begin(); it != M_Interventions.end(); it++)
	bool colocado = true;

	bool terminado = false;
	int inicio = 0;
	int fin = 0;
	int paso = 0;
	do
	{
		inicio = fin;
		paso = get_random(0,4);
		//Primero hay que saber quién es el último
		bool elegido_rango = false;


		inicio = fin;

		//Ya no queda ninguno
		if (inicio == (M_N_Interventions))
			return false;
		double umbral = (1 - (double)(paso / (double)100)) * M_Interventions_Temp[inicio].second;
		for (int i = inicio; i < M_N_Interventions && elegido_rango != true; i++)
		{
			if (M_Interventions_Colocada[M_Interventions_Temp[i].first] == true) continue;

			if (M_Interventions_Temp[i].second < umbral - __FLT_EPSILON__)
			{
				fin = i;
				elegido_rango = true;
			}
		}
		if (elegido_rango != true && paso < 100)
		{
			fin = M_N_Interventions;
			elegido_rango = true;
		}
		if (inicio == (M_N_Interventions))
			return false;
		//elegir al azar del rango
		vector<int> random;
		for (int i = inicio; i < fin; i++)
		{
			random.push_back(i);
		}
		random_shuffle(random.begin(), random.end());
		terminado = false;
		int choose_elegido = (-1);
		for (int ik = 0; ik < random.size(); ik++)
		{
			choose_elegido = random[ik];
			if (4 *M_NI_Colocadas< M_N_Interventions)
				colocado = ColocarEnTiempoRValor(choose_elegido);
			else
				colocado = ColocarEnTiempoRGreedy(choose_elegido);
			//			colocado = ColocarEnTiempo(choose_elegido);

			if (colocado == true)
			{
				M_Interventions_Colocada[choose_elegido] = true;
				if (M_NI_Colocadas > ((double)(M_Porcentaje_Ocupado * M_N_Interventions)/(double)100))
					Copy();
			}
			else
			{
				cout << "No puede colocar  la intervention " << choose_elegido << endl << endl;
				if (M_Tipo!=9) M_Interventions_Out[choose_elegido] += M_T;
				return false;
			}
			if (M_NI_Colocadas == M_N_Interventions)
				return true;
		}
	} while (colocado!=true || M_NI_Colocadas !=M_N_Interventions);
};






int Maintenance::IndiceTipo(int type, int index)
{
	switch (type)
	{
	default:
		return M_Interventions_MaxDifR[index].first;
	case 1:
		return M_Interventions_MaxDifW[index].first;
	case 2:
		return M_Interventions_MinR[index].first;

	case 3:
		return  M_Interventions_MinW[index].first;

	case 4:
		return M_Interventions_MinWxT[index].first;

	case 5:
		return M_Interventions_MinRxT[index].first;
	case 6:
		return M_Interventions_Min_WXR[index].first;
	case 7:
		return M_Interventions_Min_WtXRt[index].first;
	case 8:
		return M_Interventions_Exclusions[index].first;
	case 9:
		return M_Interventions_Out_Ordenada[index];
	case 10:
		return M_Interventions_Risk_Temp[index];
	case 11:
		return M_Interventions_Regret[index];
	}
}
void Maintenance::Constructive(int tipo) //Si tipo es 0 es por diferencia, si es 1 por dif de workload, si es 2 por min R, si es 3 por min W
{
	//Primera opción
	//Coger las interventions ordenadas de mayor a menor por riesgo minimo y poner donde menor riesgo tenga
	//y sea posible
//	M_Interventions_Colocada.clear();
	//	M_I_Seasons.clear();

	for (int i = 0; i < M_Resources.size(); i++)
	{
		M_Resources[i].Clean();
	}

	for (int i = 0; i < M_N_Interventions; i++)
	{
		M_I_Seasons[i] = -1;
		M_Interventions_Colocada[i] = false;

	}
	//	for (vector<Intervention>::iterator it = M_Interventions.begin(); it != M_Interventions.end(); it++)
	bool colocado = true;
	int choose = -1;

	for (int i = 0; i < M_Interventions.size(); i++)
	{


		switch (tipo)
		{
		default:
			choose = M_Interventions_MaxDifR[i].first;
			break;
		case 1:
			choose = M_Interventions_MaxDifW[i].first;
			break;
		case 2:
			choose = M_Interventions_MinR[i].first;
			break;
		case 3:
			choose = M_Interventions_MinW[i].first;
			break;
		}


		if (M_Interventions_Colocada[choose] == false)
		{
			colocado = ColocarEnTiempo(choose);
			if (colocado == true)
			{
				M_Interventions_Colocada[choose] = true;
			}
			else
			{
				cout << "No puede colocar  la intervention " << choose << endl << endl;
				if (M_Tipo != 9) M_Interventions_Out[choose] += M_T;
			}

		}
	}


};
bool Maintenance::ColocarEnTiempoR(int n_int)
{
	bool puedo = false;
	vector<bool> p(M_T);
	M_T_Temp.clear();
	M_T_Temp = p;
	int choose_elegido = (-1);

	cout << n_int << "Intervention" << endl;
	do
	{
		bool primero = false;
		choose_elegido = (-1);
		bool elegido = false;
		for (int i = 0; i < M_Interventions[n_int].Get_Tmax() && elegido != true; i++)
		{
			if (M_T_Temp[M_Interventions[n_int].Get_T_Ordenados_Risk()[i]] == true) continue;
			int alea = get_random(0, 2);
			if (alea == 2 && primero == true) continue;
			//			primero = true;
			if (primero==true || alea!=2) elegido = true;
			primero = true;
			
			choose_elegido = i;

		}
		if (choose_elegido == (-1))
			return false;
		M_T_Temp[M_Interventions[n_int].Get_T_Ordenados_Risk()[choose_elegido]]=true;
		puedo = ColocarEnTiempo(n_int, M_Interventions[n_int].Get_T_Ordenados_Risk()[choose_elegido]);
//		cout << "Pruebo en " << choose_elegido<< " Y coloco " << puedo << endl;

		if (puedo == true)
			return true;

	} while (choose_elegido!=(-1));
	if (choose_elegido == (-1))
		return false;
}
void Maintenance::CotaPrimeraParte()
{
	M_Cota = 0;
	for (int i = 0; i < M_N_Interventions ; i++)
	{
		M_Cota = M_Cota+ M_Interventions[i].Get_Min_Risk();
	}
	M_Cota = M_Cota /(double) M_T;
	M_Cota = M_Alpha * M_Cota;


}

bool Maintenance::ColocarEnTiempoRValor(int n_int)
{
	bool puedo = false;
	vector<bool> p(M_T);

	M_T_Temp.clear();
	M_T_Temp = p;
	int choose_elegido = (-1);

//	cout << n_int << "Intervention" << endl;
	int inicio = 0;
	int fin = 0;
	int paso = 0;
	do
	{
		inicio = fin;
		paso = get_random(10,30);
//		paso = 10;
		//Primero hay que saber quién es el último
		bool elegido_rango = false;

		do
		{
			inicio = fin;

			//Ya no queda ninguno
			if (inicio == (M_Interventions[n_int].Get_Tmax() - 1))
				return false;
			double umbral = (1+ (double)(paso / (double)100) ) * M_Interventions[n_int].Get_RiskC()[M_Interventions[n_int].Get_T_Ordenados_Risk()[inicio]];
//			for (int i = inicio; i < M_Interventions[n_int].Get_T_Ordenados_Risk_I().size() && elegido_rango != true; i++)
			for (list<int> ::iterator it= M_Interventions[n_int].Get_T_Ordenados_Risk_I().begin(); it != M_Interventions[n_int].Get_T_Ordenados_Risk_I().end() && elegido_rango != true; it++)
			{
				if (M_T_Temp[M_Interventions[n_int].Get_T_Ordenados_Risk()[(*it)-1]] == true) continue;

				if (M_Interventions[n_int].Get_RiskC()[M_Interventions[n_int].Get_T_Ordenados_Risk()[(*it)-1]] > umbral + __FLT_EPSILON__)
				{
					fin = (*it)-1;
					elegido_rango = true;
				}
			}
			if (elegido_rango != true && paso<100)
			{
				fin = M_Interventions[n_int].Get_T_Ordenados_Risk().size() - 1;
				elegido_rango = true;
			}
			paso=paso+10;
		} while (elegido_rango != true);
		if (inicio == (M_Interventions[n_int].Get_T_Ordenados_Risk().size() - 1))
			return false;
		//elegir al azar del rango
		vector<int> random;
		for (int i = inicio; i < fin; i++)
		{
			random.push_back(M_Interventions[n_int].Get_T_Ordenados_Risk()[i]);
		}

		random_shuffle(random.begin(), random.end());
		bool terminado = false;
		choose_elegido = (-1);
		for (int ik = 0; ik < random.size(); ik++)
		{
			int i = random[ik];

			if (M_T_Temp[i] == true) 
				continue;
			choose_elegido = i;
			terminado = true;

			M_T_Temp[i] = true;
			puedo = ColocarEnTiempo(n_int, i);
//			cout << "Pruebo en " << choose_elegido << " Y coloco " << puedo << endl;

			if (puedo == true)
				return true;

		}
	} while (choose_elegido != (-1) || fin!=((M_Interventions[n_int].Get_T_Ordenados_Risk_I().size() - 1)));
	if (choose_elegido == (-1))
		return false;
}
bool Maintenance::ColocarEnTiempoRGreedy(int n_int)
{
	bool puedo = false;


	int index_anterior = 0;
	int lugar=0;
	for (list<int> ::iterator it = M_Interventions[n_int].Get_T_Ordenados_Risk_I().begin(); it != M_Interventions[n_int].Get_T_Ordenados_Risk_I().end(); it++)
	{
		lugar++;
		if (((*it) - index_anterior) > 1)
		{
//			if (it == I_T_ordenados_Risk_I.begin())
//			random_shuffle(M_Interventions_MaxDifR.begin(), M_Interventions_MaxDifR.begin() + (*it));
			if (M_T_Ordenado)
				sort(M_Interventions[n_int].Get_T_Ordenados_Risk().begin() + index_anterior, M_Interventions[n_int].Get_T_Ordenados_Risk().begin() + (*it));
			else
				random_shuffle(M_Interventions[n_int].Get_T_Ordenados_Risk().begin() + index_anterior, M_Interventions[n_int].Get_T_Ordenados_Risk().begin() + (*it));
				for (int i = index_anterior; i < (*it); i++)
				{
					puedo = ColocarEnTiempo(n_int, M_Interventions[n_int].Get_T_Ordenados_Risk()[i]);
					//			cout << "Pruebo en " << choose_elegido << " Y coloco " << puedo << endl;
					if (puedo == true)
						return true;
					else
						if (M_Tipo != 9) M_Interventions_Out[n_int] += lugar;
				}
		}

		else
		{

			puedo = ColocarEnTiempo(n_int, M_Interventions[n_int].Get_T_Ordenados_Risk()[index_anterior]);
			//			cout << "Pruebo en " << choose_elegido << " Y coloco " << puedo << endl;
			if (puedo == true)
				return true;
			else
				if (M_Tipo != 9) M_Interventions_Out[n_int] += lugar;
		}

		index_anterior = (*it);

	}
	return false;
}
bool Maintenance::ColocarEnTiempo(int n_int)
{
	bool puedo = false;

	for (int i = 0; i < M_Interventions[n_int].Get_Tmax() && puedo!=true; i++)
	{

		puedo=ColocarEnTiempo(n_int, M_Interventions[n_int].Get_T_Ordenados_Risk()[i]);
	
	}
	if (puedo == true)
		return true;
	else
		return false;
}

bool Maintenance::ColocarEnTiempo(int n_int, int t)
{

	//TODO que pasa si hay cota inferior
//	bool puedo_en_t = false;
	//Comprobar puedo por recursos
	if (!PuedoColocar(n_int, t)) 
		return false;

	//Si he llegado aquí es que puedo ponerla
	//Puedo ponerla
	ColocarIEnTiempo(n_int, t);
//	VerifySolution();
	if (!M_Silent) cout << M_I_Name[n_int] <<" Intervention " << n_int << " Tiempo Inicio " << t << " Tiempo Fin " << t+M_Interventions[n_int].Get_Delta(t) << " Season " << M_S_Name[M_T_Season[t]] << endl;
	return true;

}
/*
void Maintenance::ColocarIEnTiempo(int n_int, int t)
{
	for (map<int, map<int, map <int, double>>> ::iterator it = M_Interventions[n_int].Get_Workload().begin(); it != M_Interventions[n_int].Get_Workload().end(); it++)
	{
		//Recursos
		for (int tprima = t; tprima < (t + M_Interventions[n_int].Get_Delta(t)); tprima++)
		{

			M_Resources[(*it).first].Add(M_Interventions[n_int].Get_Workload()[(*it).first][tprima][t], tprima);
			//			if (M_Interventions[n_int].Get_Workload()[(*it).first][tprima][t] > 0 && (*it).first == 1 && tprima==6)
			//				cout << M_I_Name[n_int] << " " << tprima << " " << t << " " << M_R_Name[(*it).first] << " " << M_Interventions[n_int].Get_Workload()[(*it).first][tprima][t] << " Temp "<< M_Resources[(*it).first].Get_Temp()[tprima-1]<<  endl;

		}

	}
	pair<int, int> p(n_int, t);
	M_I_T_Solution.push_back(p);
	//Pongo la season en la que pongo la pieza
	M_I_Seasons[n_int] = M_T_Season[t];
	M_Interventions[n_int].Set_T_Temp(t);
	M_Interventions_T[n_int] = t;
	M_Interventions_Colocada[n_int] = true;

}*/
bool Maintenance::PuedoColocar(int n_int, int t)
{

	//Exclusiones
//TO DO cambiar con lo de arreglar esta exclusion
	if (Exclusion(n_int, t) == true)
		return false;
	if (t + M_Interventions[n_int].Get_Delta(t) > M_T) return false;
	/*
	bool no_se_puede = false;
	bool no_se_puede2 = false;
	for (map<int, map<int, map <int, double>>> ::iterator itm = M_Interventions[n_int].Get_Workload().begin(); itm != M_Interventions[n_int].Get_Workload().end(); itm++)
	{
		//Recursos
		if (M_ConstruirConRecursos)
		{
			for (int tprima = t; tprima < (t + M_Interventions[n_int].Get_Delta(t)); tprima++)
			{
				if ((M_Interventions[n_int].Get_Workload((*itm).first, tprima, t) + M_Resources[(*itm).first].Get_Temp()[tprima] > (M_Resources[(*itm).first].Get_Upper()[tprima] + __FLT_EPSILON__)) ||
					(M_Interventions[n_int].Get_Workload((*itm).first, tprima, t) + M_Resources[(*itm).first].Get_Temp()[tprima] < (M_Resources[(*itm).first].Get_Lower()[tprima] - __FLT_EPSILON__)))
					no_se_puede = true;
			}
		}



	}*/
	for (vector<int>::iterator it=M_Interventions[n_int].Get_Workload_Res().begin();it!= M_Interventions[n_int].Get_Workload_Res().end();it++)
	{
		int i = (*it);
//	for (int i=0;i<M_N_Resources;i++)
//	{
		//Recursos
		if (M_ConstruirConRecursos)
		{
			for (int tprima = t; tprima < (t + M_Interventions[n_int].Get_Delta(t)); tprima++)
			{
				if ((M_Interventions[n_int].Get_WorkloadN(i, t, tprima) + M_Resources[i].Get_Temp(tprima)> (M_Resources[i].Get_Upper(tprima) + __FLT_EPSILON__)))
					return false;
//					no_se_puede2 = true;
			}
		}
		if (M_Lower_ResourcesV[i] == true)
		{
			for (int tprima = t; tprima < (t + M_Interventions[n_int].Get_Delta(t)); tprima++)
			{
				if ((M_Interventions[n_int].Get_WorkloadN(i, t, tprima) + M_Resources[i].Get_Temp(tprima) < (M_Resources[i].Get_Lower(tprima) - __FLT_EPSILON__)))
					return false;
				//					no_se_puede2 = true;
			}
		}



	}
	/*
	if (no_se_puede != no_se_puede2)
		Problema(1897);
	if (no_se_puede == true)
		return false;*/


	return true;

}
bool Maintenance::PuedoQuitar(int n_int, int t)
{
	if (M_Lower_Resources==false) 
		return true;
		
	for (int i = 0; i < M_N_Resources; i++)
	{
		//Recursos
		if (M_Lower_ResourcesV[i] == false) continue;
		for (int tprima = t; tprima < (t + M_Interventions[n_int].Get_Delta(t)); tprima++)
		{
			if (M_Resources[i].Get_Lower(tprima) < __FLT_EPSILON__)
				continue;
			if (( M_Resources[i].Get_Temp(tprima)- (M_Interventions[n_int].Get_WorkloadN(i, t, tprima) )) < (M_Resources[i].Get_Lower(tprima) - __FLT_EPSILON__))
				return false;
			//					no_se_puede2 = true;
		}

	}


	return true;

}
 bool Maintenance::Exclusion( int nint, int tiempo )
{

	//Voy a colocar en intervention nint y tiempo t
	//Tengo que mirar si es posible

	for (list<pair<int, int>> ::iterator it = M_Interventions[nint].Get_Exclusions().begin(); it != M_Interventions[nint].Get_Exclusions().end(); it++)
	{
		//no está colocada la anterior
		if (M_Interventions_Colocada[(*it).first] == false || M_I_T[(*it).first] <0) continue;
		//

		for (int t = tiempo; t < tiempo + M_Interventions[nint].Get_Delta()[tiempo]; t++)
		{
			//Si el t no es de esa temporada
			if (M_T_Season[t] != (*it).second)
				continue;
			if (M_Interventions[(*it).first].Get_T_Temp() > t)
				continue;
			if (M_Interventions[(*it).first].Get_T_F_Temp() <= t)
				continue;
			return  true;
			//Si lo es no puede solapar con la otra

		}
	}


	return false;

}

//Función objetivo
/*
## Compute mean for each period
def compute_mean_risk(risk, T_max: int, scenario_numbers) :
	"""Compute mean risk values over each time period"""

	print('\tComputing mean risk...')
	# Init mean risk
	mean_risk = np.zeros(T_max)
	# compute mean
	for t in range(T_max) :
		mean_risk[t] = sum(risk[t]) / scenario_numbers[t]
		print('\tDone')

		return mean_risk

## Compute quantile for each period
		def compute_quantile(risk, T_max: int, scenario_numbers, quantile) :
		"""Compute Quantile values over each time period"""

		print('\tComputing Quantile...')
		# Init quantile
		q = np.zeros(T_max)
		for t in range(T_max) :
			risk[t].sort()
			q[t] = risk[t][int(np.ceil(scenario_numbers[t] * quantile)) - 1]
			print('\tDone')

			return q

## Compute both objectives: mean risk and quantile
			def compute_objective(Instance : dict) :
			"""Compute objectives (mean and expected excess)"""

			print('Computing objectives values...')
			# Retrieve usefull infos
			T_max = Instance[T_STR]
			scenario_numbers = Instance[SCENARIO_NUMBER]
			Interventions = Instance[INTERVENTIONS_STR]
			quantile = Instance[QUANTILE_STR]
			# Retrieve risk final distribution
			risk = compute_risk_distribution(Interventions, T_max, scenario_numbers)
			# Compute mean risk
			mean_risk = compute_mean_risk(risk, T_max, scenario_numbers)
			# Compute quantile
			q = compute_quantile(risk, T_max, scenario_numbers, quantile)
			print('Done')

			return mean_risk, q

			*/
