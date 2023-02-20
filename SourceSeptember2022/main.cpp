/*
 * Francisco Parreño Torres
 UCLM

 */

#include "Maintenance.h"
#include "definitions.h"
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <omp.h>
//#include "json.h"

using namespace std;

int Global_Best_Objective_Function = MAXIMUM_INT_ROADEF; // 11266923; //2329354
int Param = 30;

int main(int argc, char **argv)
{
//	struct json_object *jobj; //	int result = system("RTE_ChallengeROADEF2020_checker.py A_set/example1.json solution1.txt");
	int nThreads = 4;
	int LazyConst = 0;
	int CotaTira = 0; //	int CotaNB = 0;
	Global_Best_Objective_Function = MAXIMUM_INT_ROADEF;
	Param = 21;
	int Exacto = 0; //	int tiempo_heuristico = 180;//30;
	string file_to_read, file_to_write, batchPath, solutionPath, defectsPath, optParamsPath,
	    file_idx2;
	int Integer = -1;
	int Type = 0;
	int TeamId = 0, seed = 1, Seconds = 900;
	int Doble = 2;
	int Iter = 10;
	int VND = 1;
	int TipoBusqueda = 7;
	int maxSeconds = 900;
	int visible = 1;
	file_to_read = "A_set/A1"; //Procesamiento de los parametros
	if ((argc == 1))
	{
		cout << "Team 56" << endl; //		printf("Team 56");
		return 56;
	}
	if ((argc > 2)) //&& (argc % 2 == 1))
	{               //Se comprueba que haya pares de argumentos (y al menos un par)
		while (argc > 1)
		{
			if (strcmp(argv[1], "-t") == 0)
			{ //Tiempo
				sscanf(argv[2], "%d", &Seconds);
			}
			else if (strcmp(argv[1], "-p") == 0)
			{ //File to read
				file_to_read = argv[2];
			}
			else if (strcmp(argv[1], "-o") == 0)
			{
				file_to_write = argv[2];
			}
			else if (strcmp(argv[1], "-s") == 0)
			{
				sscanf(argv[2], "%d", &seed);
			}
			else if (strcmp(argv[1], "-nThreads") == 0)
			{
				sscanf(argv[2], "%d", &nThreads);
			}
			else if (strcmp(argv[1], "-name") == 0)
			{
				printf("\n\nTeam 56\n\n");
				argc -= 1;
				argv += 1;
				continue; //				sscanf(argv[2], "%d", &TeamId);
			}
			else if (strcmp(argv[1], "-iter") == 0)
			{
				sscanf(argv[2], "%d", &Iter);
			} //For the test
			else if (strcmp(argv[1], "-Param") == 0)
			{
				sscanf(argv[2], "%d", &Param);
			}
			else if (strcmp(argv[1], "-Exacto") == 0)
			{
				sscanf(argv[2], "%d", &Exacto);
			} //For the test
			else if (strcmp(argv[1], "-Lazy") == 0)
			{
				sscanf(argv[2], "%d", &LazyConst);
			}
			else if (strcmp(argv[1], "-Tira") == 0)
			{
				sscanf(argv[2], "%d", &CotaTira);
			}
			else if (strcmp(argv[1], "-TipoB") == 0)
			{
				sscanf(argv[2], "%d", &TipoBusqueda);
			}
			else if (strcmp(argv[1], "-Doble") == 0)
			{
				sscanf(argv[2], "%d", &Doble);
			}
			else if (strcmp(argv[1], "-Type") == 0)
			{
				sscanf(argv[2], "%d",
				       &Type); //Type=0 is the usual //Type=1 is the new one
			}
			else if (strcmp(argv[1], "-Integer") == 0)
			{
				sscanf(argv[2], "%d",
				       &Integer); //Type=0 is the usual //Type=1 is the new one
			}
			else if (strcmp(argv[1], "-VND") == 0)
			{
				sscanf(argv[2], "%d",
				       &VND); //Type=0 is the usual //Type=1 is the new one
			}
			else if (strcmp(argv[1], "-Visible") == 0)
			{
				sscanf(argv[2], "%d",
				       &visible); //Type=0 is the usual //Type=1 is the new one
			}
			argc -= 2;
			argv += 2;
		}
	}
	struct timeb time_final, time_initial;
	ftime(&time_initial);
	const struct timeb time_ini = time_initial; //	Doble = 0; //	DibujarOpenGL();
	vector<Maintenance> Problemas;
	srand(seed);
	Maintenance Problema(1); //		Problema.PruebaHash();
	Problema.Set_Seed(seed);
	nThreads = min(omp_get_max_threads(), nThreads);
	Problema.SetNumberOfThreads(nThreads); //	Problema.leer_problem(file_to_read);
	Problema.Set_Time_Initial(time_ini);
	double tmp_first2 = 0; //	Problema.leer_problem_wojson(file_to_read);
	Problema.leer_problem(
	    file_to_read); //	Maintenance Problema2(1); //	Problema2.leer_problem_wojson(file_to_read); //		jobj = json_tokener_parse(argv[2]);
	ftime(&time_final);
	tmp_first2 = ((double)((time_final.time - Problema.Get_Time_Initial().time) * 1000 +
	                       time_final.millitm - Problema.Get_Time_Initial().millitm)) /
	             1000;
	Problema.Set_Time_Leer(tmp_first2);
	if (!visible) printf("\n Time Reading %.3f", tmp_first2);
	Problema.VectoresOrdenados();
	Problema.CotaPrimeraParte();
	Problema.SetVND(VND);
	Problema.Set_Pool_Solutions(true);
	Problema.Set_Mixed_Risk(false);
	Problema.Set_Formulacion_Cuantiles(false);
	Problema.Set_Max_Formulation(false);
	Problema.Set_Min_Formulation(false);
	Problema.CalcularMaximosEscenario();
	Problema.Set_Mixed_Risk(false);
	Problema.SetTotalTime(Seconds - 20);
	Problema.Set_ChallengeMode(visible);
	maxSeconds = Seconds - 20;
	if (Integer >= 0)
	{
		Problema.Formulations(Integer);
		ftime(&time_final);
		tmp_first2 = ((double)((time_final.time - Problema.Get_Time_Initial().time) * 1000 +
		                       time_final.millitm - Problema.Get_Time_Initial().millitm)) /
		             1000;
	}
	Problema.Set_Time_Grasp(15); //		Problema.VerificarRiskMap();
	Problema.Set_TipoBusqueda(TipoBusqueda);
	Problema.Set_NThreads(nThreads);
	Problema.Set_MGapPool(10); //		Problema.MejorarPoolSolutions2(0);
	for (int NHilos = 0; NHilos < nThreads; NHilos++)
	{
		Problemas.push_back(Problema);
		Problemas[NHilos].Set_Seed(seed + NHilos);
		Problemas[NHilos].Set_Hilo(NHilos);
		Problemas[NHilos].Set_Iter_Formulation(1);
	}
//	Problema.~Maintenance();
	int M_Best_Hilo = 0;
	if (nThreads < 2)
	{
		if (TipoBusqueda == 7)
		{
			Problemas[0].GRASP();
			Problemas[0].MejorarPoolSolutions();
		}
		else
		{
			Problemas[0].Set_Pool_Solutions(false);
		}
	}
	else
	{ //GRASP Inicial
		int TiempoGrasp = min((maxSeconds / 10),60)+tmp_first2; //		TiempoGrasp = 10; //PARA EL GRASP //PAra hacerlo con fopenmp
		if (TiempoGrasp>maxSeconds)
			TiempoGrasp=maxSeconds-tmp_first2;
#pragma omp parallel num_threads(nThreads)
		{ //#pragma omp for schedule(static,1) nowait
#pragma omp for schedule(dynamic, 1)  nowait 
			for (int Global_Iter = 0; Global_Iter < nThreads; Global_Iter++)
			{
				struct timeb time_ini2;
				ftime(&time_ini2);
				double tmp_first3 =
				    ((double)((time_ini2.time -
				               Problemas[Global_Iter].Get_Time_Initial().time) *
				                  1000 +
				              time_ini2.millitm -
				              Problemas[Global_Iter].Get_Time_Initial().millitm)) /
				    1000;
				if (tmp_first3 < maxSeconds)
				{
					struct timeb time_ini21;
					ftime(&time_ini21);
					Problemas[Global_Iter].SetTotalTime(TiempoGrasp);
					Problemas[Global_Iter].GRASP();
				}
			}
		}
		M_Best_Hilo = 0;
		for (int cont = 0; cont < nThreads; cont++)
		{
			if (!visible) printf("\n Sol Hilo %d %.5f Iter %d\n", cont,
			       Problemas[cont].Get_Best_Obj(), Problemas[cont].Get_Iter());
			if (cont == 0)
				continue;
			if (Problemas[cont].Get_Best_Obj() < Problemas[M_Best_Hilo].Get_Best_Obj())
			{
				M_Best_Hilo = cont;
			}
		}
		if (!visible) printf("\n Time Grasp %.3f", tmp_first2);
		Problemas[M_Best_Hilo].PrintSolution(file_to_write);
		Problemas[0].Set_Best_Solution(Problemas[M_Best_Hilo].Get_Best_Solution());
		ftime(
		    &time_final); //	double tmp_first = ((double)((time_final.time - Problemas[M_Best_Hilo].Get_Time_Initial().time) * 1000 + time_final.millitm - Problemas[M_Best_Hilo].Get_Time_Initial().millitm)) / 1000;
		double tmp_first4 = ((double)((time_final.time - time_ini.time) * 1000 +time_final.millitm - time_ini.millitm)) /1000;
		int max_Iter = 1;
		int Iter_Grasp = 0;
		for (int cont = 0; cont < nThreads; cont++)
		{
			Iter_Grasp+=Problemas[cont].Get_Iter()-Problemas[cont].Get_No_Sol();
			Problemas[cont].Set_MGapPool(100);


		}
		Problemas[0].Set_Iter(Iter_Grasp);
		int Total_Time = maxSeconds - tmp_first4;
/*		max_Iter = 800 / tmp_first2;
		if (max_Iter > 8)
			max_Iter = 8;
		if (max_Iter < 1)
			max_Iter = 1;
		if (Seconds > 950)
		{
			max_Iter = 3 * max_Iter;
			
			Problemas[0].Set_MGapPool(1);
		}
		if (max_Iter>1 && Problemas[0].Get_Iter()+Problemas[1].Get_Iter() <= 4)
		{
			max_Iter / 2;
			Problemas[0].Set_MGapPool(10);
		}

		//Una sola vez
		max_Iter = 1;
		maxSeconds = Total_Time / max_Iter;
		maxSeconds = maxSeconds - 3 * max(1, 4 - max_Iter);*/
		if (!visible) printf("\nIteraciones %d Tiempo por cada una %d\n", max_Iter,
		       maxSeconds); //		maxSeconds = 10;
		//Dos veces una corto y uno largo
		if (Total_Time>1000) 
		{
			max_Iter = 2;

		}
if (Total_Time>36000) max_Iter=3;
		if (!visible) printf("\nIteraciones %d Tiempo por cada una %d\n", max_Iter,
		       maxSeconds); //		maxSeconds = 10;

		for (int k = 0; k < nThreads; k++)
		{
			Problemas[k].SetTotalTime(maxSeconds); 
			Problemas[k].Set_Iter_Grasp(Iter_Grasp);
		}
		for (int Iter_Global = 1; Iter_Global <= max_Iter; Iter_Global++)
		{
			if (max_Iter == 2) 
			{
				if (Iter_Global==1) 
				{
					
					Total_Time=900-tmp_first4;
					Total_Time = Total_Time - 10;
					Problemas[0].Set_MGapPool(100);

		

				}
				else
				{
					Total_Time=maxSeconds-900;
					Total_Time = Total_Time - 10;
					Problemas[0].Set_MGapPool(100);
					Problemas[0].Set_Iter(Iter_Grasp);
					//Quitar de Integer_Sol_Distintas las no visitadas

	//				Problemas[0].Get_Sol_Pool();
					list<pair<vector<int>, double>> tempk = Problemas[0].GetPoolSolutions();
					Problemas[0].ClearPool();
					Problemas[0].Insertar(tempk);
					for (int ki1 = 1; ki1 < nThreads; ki1++)
					{
						Problemas[0].Insertar(Problemas[ki1].GetPoolSolutions());
					}
				}
				for (int k = 0; k < nThreads; k++)
				{
					Problemas[k].SetTotalTime(Total_Time);
				}
			}
			if (max_Iter == 3) 
			{
				if (Iter_Global==1) 
				{
					Total_Time=1400-tmp_first4;
					Total_Time = Total_Time - 10;
					Problemas[0].Set_MGapPool(100);
					Problemas[0].Set_Iter(Iter_Grasp);		

				}
				if (Iter_Global==2) 
				{
					
					Total_Time = 600 - 10;	
					Problemas[0].Set_MGapPool(1000);
					Problemas[0].Set_Iter(Iter_Grasp);					
					list<pair<vector<int>, double>> tempk = Problemas[0].GetPoolSolutions();
					Problemas[0].ClearPool();
					Problemas[0].Insertar(tempk);
					for (int ki1 = 1; ki1 < nThreads; ki1++)
					{
						Problemas[0].Insertar(Problemas[ki1].GetPoolSolutions());
					}

				}
				if (Iter_Global==3)
				{
					Total_Time=maxSeconds-2000;
					Total_Time = Total_Time - 10;
					Problemas[0].Set_MGapPool(1000);
					Problemas[0].Set_Iter(Iter_Grasp);
					list<pair<vector<int>, double>> tempk = Problemas[0].GetPoolSolutions();
					Problemas[0].ClearPool();
					Problemas[0].Insertar(tempk);
					for (int ki1 = 1; ki1 < nThreads; ki1++)
					{
						Problemas[0].Insertar(Problemas[ki1].GetPoolSolutions());
					}
	

				}
				for (int k = 0; k < nThreads; k++)
				{
					Problemas[k].SetTotalTime(Total_Time);
				}
			}

			ftime(
			    &time_final); //	double tmp_first = ((double)((time_final.time - Problemas[M_Best_Hilo].Get_Time_Initial().time) * 1000 + time_final.millitm - Problemas[M_Best_Hilo].Get_Time_Initial().millitm)) / 1000;
			double tmp_first5 = ((double)((time_final.time - time_ini.time) * 1000 +
			                              time_final.millitm - time_ini.millitm)) /
			                    1000;
			printf("\nTiempo %.3f Iter %d \n", tmp_first5, Iter_Global);
			struct timeb time_initial2;
			ftime(&time_initial2);
			const struct timeb time_ini2 = time_initial2;
			for (int k = 0; k < nThreads; k++)
			{
				Problemas[k].Set_Time_Initial(time_ini2);
				Problemas[k].Set_Iter_Formulation(Iter_Global);
			}

			if (nThreads > 1 && TipoBusqueda == 7)
				Problemas[0].CrearListasFormulations();
			printf("Tiempo %d", Total_Time);
			if (Problemas[0].EncuentraSolutions())
			{
				for (int i = 1; i < nThreads; i++)
				{
					Problemas[i].SetListaSolucionesInteger(
					    Problemas[0].GetListaSolucionesInteger(i));
				}
			} /*		for (int i = 1; i < nThreads; i++) 
			{
				Problemas[0].Insertar_Distintos(Problemas[i].GetDistintos());
			}
			for (int i = 1; i < nThreads; i++) 
			{
				Problemas[i].Insertar_Distintos(Problemas[0].GetDistintos());
			}*/ //PAra hacerlo con fopenmp
#pragma omp parallel num_threads(nThreads)
			{
#pragma omp for schedule(static, 1)   nowait //#pragma omp for schedule(dynamic, 1)
				for (int Global_Iter = 0; Global_Iter < nThreads; Global_Iter++)
				{
					struct timeb time_ini_3;
					ftime(&time_ini_3);
					double tmp_first3 =  ((double)((time_ini_3.time - Problemas[0].Get_Time_Initial().time) * 1000 + time_ini_3.millitm - Problemas[0].Get_Time_Initial().millitm)) / 1000;
					if (tmp_first3 < maxSeconds)
					{
						struct timeb time_ini21;
						ftime(&time_ini21);
						Problemas[Global_Iter].SetTotalTime(Total_Time);
						Problemas[Global_Iter].SetIntensification(true);
						Problemas[Global_Iter].MejorarPoolSolutions2(
						    Global_Iter);
					}
				}
			}
			M_Best_Hilo = 0;
			for (int cont = 0; cont < nThreads; cont++)
			{
				if (!visible) printf("\n Sol Hilo %d %.5f Iter %d\n", cont,
				       Problemas[cont].Get_Best_Obj(), Problemas[cont].Get_Iter());
				if (cont == 0)
					continue;
				if (Problemas[cont].Get_Best_Obj() <
				    Problemas[M_Best_Hilo].Get_Best_Obj())
				{
					M_Best_Hilo = cont;
				}
			}
			printf("Mejor %d %.5f\n", M_Best_Hilo,
			       Problemas[M_Best_Hilo].Get_Best_Obj());
			Problemas[M_Best_Hilo].PrintSolution(file_to_write);
		}
	}



	//	string name2 = file_to_read+"Resultados.txt";
#ifdef __linux__
	//linux code goes here
	string name2 = file_to_read.c_str() + std::to_string(Seconds) +
	               "_sol.txt"; //"RHeur_" + kk +"_"+ std::to_string(Problem.Getm())+".txt";
	FILE *file = fopen(name2.c_str(), "a+");

	//	file << endl;
	ftime(&time_final);
	//	double tmp_first = ((double)((time_final.time - Problemas[M_Best_Hilo].Get_Time_Initial().time) * 1000 + time_final.millitm - Problemas[M_Best_Hilo].Get_Time_Initial().millitm)) / 1000;
	double tmp_first = ((double)((time_final.time - time_ini.time) * 1000 + time_final.millitm -
	                             time_ini.millitm)) /
	                   1000;

	fprintf(file,
	        "%s FSI %.5f FTI %.3f FO1 %.5f FO2 %.5f T2 %f T3 %f FO %.5f FO1 %.5f FO2 %.5f T %f",
	        file_to_read.c_str(), Problemas[M_Best_Hilo].Get_Sol_Formulation(),
	        Problemas[M_Best_Hilo].Get_Time_Formulation1(),
	        Problemas[M_Best_Hilo].Get_Obj_Formulation1(),
	        Problemas[M_Best_Hilo].Get_Obj_Formulation2(),
	        Problemas[M_Best_Hilo].Get_Time_Formulation2(), tmp_first2,
	        Problemas[M_Best_Hilo].Get_Best_Obj(), Problemas[M_Best_Hilo].Get_BestFO1(),
	        Problemas[M_Best_Hilo].Get_BestFO2(), tmp_first);
	fprintf(
	    file,
	    " BI %d TI %d C %.5f E %d I %d R %d T %d S %d MM %.5f VND %d TB %d Hilo %d  M %d #P %d "
	    "#Q %d #M %d NSol %d I %.5f Q %.5f Max  %.5f  Alpha %.5f Quantile %.5f\n",
	    Problemas[M_Best_Hilo].Get_Best_Iter(), Problemas[M_Best_Hilo].Get_Iter(),
	    Problemas[M_Best_Hilo].Get_Cota(), Problemas[M_Best_Hilo].Get_NExclusions(),
	    Problemas[M_Best_Hilo].Get_NInterventions(), Problemas[M_Best_Hilo].Get_NResources(),
	    Problema.Get_T(), Problemas[M_Best_Hilo].Get_Total_Scenarios(),
	    Problemas[M_Best_Hilo].Get_Max_Min_Risk() / (double)Problemas[M_Best_Hilo].Get_T(), VND,
	    TipoBusqueda, M_Best_Hilo, Problemas[M_Best_Hilo].Get_Lugar(),
	    Problemas[0].Get_Sol_Pool(), Problemas[0].Get_Sol_PoolQ(), Problemas[0].Get_Sol_PoolM(),
	    Problemas[M_Best_Hilo].Get_No_Sol(), Problemas[0].Get_Sol_I(), Problemas[0].Get_Sol_Q(),
	    Problemas[M_Best_Hilo].Get_Sol_M(), Problemas[M_Best_Hilo].Get_Alpha(),
	    Problemas[0].Get_Quantile());

	//	file << " BI " << Problemas[M_Best_Hilo].Get_Best_Iter()<< " TI "<<Problemas[M_Best_Hilo].Get_Iter() <<" C "<<Problemas[M_Best_Hilo].Get_Cota() <<" E  "<< Problemas[M_Best_Hilo].Get_NExclusions()<<" I  "<<Problemas[M_Best_Hilo].Get_NInterventions()<<" R "<<Problemas[M_Best_Hilo].Get_NResources()<<" T "<<Problema.Get_T()<<" S "<<Problemas[M_Best_Hilo].Get_Total_Scenarios()<< " Mm "<< Problemas[M_Best_Hilo].Get_Max_Min_Risk()/(double)Problemas[M_Best_Hilo].Get_T() << " VND "<< VND<< " TB " <<TipoBusqueda<< "Dnd"<<M_Best_Hilo <<endl;
	//	file <<  " " << Problema.Get_Best_Obj()<< " "<< tmp_first << " " << Problema.Get_Best_Iter() <<" "<<Problema.Get_Cota() <<"E  "<< Problema.Get_NExclusions()<<"I  "<<Problema.Get_NInterventions()<<"R "<<Problema.Get_NResources()<<"T "<<Problema.Get_T()<<"S "<<Problema.Get_Total_Scenarios()<<endl;

	fclose(file);

#else
	string name2 = "Resultados.txt";
	FILE *file = fopen(name2.c_str(), "a+");
	//	file << endl;
	ftime(&time_final);
	//	double tmp_first = ((double)((time_final.time - Problemas[M_Best_Hilo].Get_Time_Initial().time) * 1000 + time_final.millitm - Problemas[M_Best_Hilo].Get_Time_Initial().millitm)) / 1000;
	double tmp_first = ((double)((time_final.time - time_ini.time) * 1000 + time_final.millitm -
	                             time_ini.millitm)) /
	                   1000;

	fprintf(file,
	        "%s FSI %.5f FTI %.3f FO1 %.5f FO2 %.5f T2 %f T3 %f FO %.5f FO1 %.5f FO2 %.5f T %f",
	        file_to_read.c_str(), Problemas[M_Best_Hilo].Get_Sol_Formulation(),
	        Problemas[M_Best_Hilo].Get_Time_Formulation1(),
	        Problemas[M_Best_Hilo].Get_Obj_Formulation1(),
	        Problemas[M_Best_Hilo].Get_Obj_Formulation2(),
	        Problemas[M_Best_Hilo].Get_Time_Formulation2(), tmp_first2,
	        Problemas[M_Best_Hilo].Get_Best_Obj(), Problemas[M_Best_Hilo].Get_BestFO1(),
	        Problemas[M_Best_Hilo].Get_BestFO2(), tmp_first);
	fprintf(
	    file,
	    " BI %d TI %d C %.5f E %d I %d R %d T %d S %d MM %.5f VND %d TB %d Hilo %d  M %d #P %d "
	    "#Q %d #M %d NSol %d I %.5f Q %.5f Max  %.5f  Alpha %.5f Quantile %.5f\n",
	    Problemas[M_Best_Hilo].Get_Best_Iter(), Problemas[M_Best_Hilo].Get_Iter(),
	    Problemas[M_Best_Hilo].Get_Cota(), Problemas[M_Best_Hilo].Get_NExclusions(),
	    Problemas[M_Best_Hilo].Get_NInterventions(), Problemas[M_Best_Hilo].Get_NResources(),
	    Problema.Get_T(), Problemas[M_Best_Hilo].Get_Total_Scenarios(),
	    Problemas[M_Best_Hilo].Get_Max_Min_Risk() / (double)Problemas[M_Best_Hilo].Get_T(), VND,
	    TipoBusqueda, M_Best_Hilo, Problemas[M_Best_Hilo].Get_Lugar(),
	    Problemas[0].Get_Sol_Pool(), Problemas[0].Get_Sol_PoolQ(), Problemas[0].Get_Sol_PoolM(),
	    Problemas[M_Best_Hilo].Get_No_Sol(), Problemas[0].Get_Sol_I(), Problemas[0].Get_Sol_Q(),
	    Problemas[M_Best_Hilo].Get_Sol_M(), Problemas[M_Best_Hilo].Get_Alpha(),
	    Problemas[0].Get_Quantile());

	//	file << " BI " << Problemas[M_Best_Hilo].Get_Best_Iter()<< " TI "<<Problemas[M_Best_Hilo].Get_Iter() <<" C "<<Problemas[M_Best_Hilo].Get_Cota() <<" E  "<< Problemas[M_Best_Hilo].Get_NExclusions()<<" I  "<<Problemas[M_Best_Hilo].Get_NInterventions()<<" R "<<Problemas[M_Best_Hilo].Get_NResources()<<" T "<<Problema.Get_T()<<" S "<<Problemas[M_Best_Hilo].Get_Total_Scenarios()<< " Mm "<< Problemas[M_Best_Hilo].Get_Max_Min_Risk()/(double)Problemas[M_Best_Hilo].Get_T() << " VND "<< VND<< " TB " <<TipoBusqueda<< "Dnd"<<M_Best_Hilo <<endl;
	//	file <<  " " << Problema.Get_Best_Obj()<< " "<< tmp_first << " " << Problema.Get_Best_Iter() <<" "<<Problema.Get_Cota() <<"E  "<< Problema.Get_NExclusions()<<"I  "<<Problema.Get_NInterventions()<<"R "<<Problema.Get_NResources()<<"T "<<Problema.Get_T()<<"S "<<Problema.Get_Total_Scenarios()<<endl;

	fclose(file);
	bool revisa = true;
	if (!Problemas[M_Best_Hilo].Get_ChallengeMode() || revisa == true)
	{
		if (Problemas[M_Best_Hilo].Get_Best_Solution().size() == 0)
			return 0;
		string system_ins =
		    "RTE_ChallengeROADEF2020_checker.py " + file_to_read + ".json " + file_to_write;
		cout << endl << system_ins << endl;
		//	int result = system("RTE_ChallengeROADEF2020_checker.py A_set/example1.json solution1.txt");
		system(system_ins.c_str());

		//	Problema.PrintResources();
		//	Problema.PrintResourcesT(7);

		//	Problema.Get_Interventions()[M_Best_Hilo].Get_Risk(2, 1, 1);
		//	Problema.Get_Interventions()[M_Best_Hilo].Get_RiskC(2, 1);
		Problemas[M_Best_Hilo].PrintPSolutionStatistics(
		    Problemas[M_Best_Hilo].Get_Best_Solution());
		//	int result = system("RTE_ChallengeROADEF2020_checker.py");
	}

#endif
	return 0;

	/*	if (!Problemas[0].opened_file)
		return EXIT_FAILURE;
	return EXIT_SUCCESS;
	*/
}
/*
//PAra hacerlo con fopenmp
#pragma omp parallel  num_threads(nThreads)
		{



		//#pragma omp for schedule(static,1) nowait

#pragma omp for schedule(dynamic,1)


		for (int Global_Iter = 0; Global_Iter <= Number_Of_Iterations; Global_Iter++)
		{




			struct timeb  time_ini2;

			ftime(&time_ini2);
			double tmp_first2 = ((double)((time_ini2.time - time_ini.time) * 1000 + time_ini2.millitm - time_ini.millitm)) / 1000;
			if (tmp_first2 < maxSeconds)
			{

				struct timeb  time_ini21;
				ftime(&time_ini21);




				if (Problemas[Global_Iter].active_log_error)
					printf("\nTipo Nivel %d Thread %d Iter \t%d\n", Problemas[Global_Iter].G_type_strip, omp_get_thread_num(), Global_Iter);
				//					else
					//					printf("\nTipo Nivel %d Thread NO Error %d Iter \t%d\n", Problemas[Global_Iter].G_type_strip, omp_get_thread_num(), Global_Iter);


					Problemas[Global_Iter].BeamSearch(omp_get_thread_num(), seedId);

				ftime(&time_ini2);
				tmp_first2 = ((double)((time_ini2.time - time_ini21.time) * 1000 + time_ini2.millitm - time_ini21.millitm)) / 1000;
				if (Problemas[Global_Iter].active_log_error) printf("\nTiempo %f Thread %d Iter \t%d\n", tmp_first2, omp_get_thread_num(), Global_Iter);
				if (Problemas[Global_Iter].active_log_error) printf("\nTiempo2 %f Thread %d Iter \t%d\n", tmp_first2, omp_get_thread_num(), Global_Iter);

			}

		}
		}

*/
