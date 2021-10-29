#include "Maintenance.h"

#pragma warning(disable:4786)


#include <assert.h>
#include <sys/types.h>
#include <sys/timeb.h>

#include <ilcplex/ilocplex.h>
#include <ilconcert/ilomodel.h>
ILOSTLBEGIN




using namespace std;
/*
	IloIntVarArray  variable entera
	IloNumVarArray  variable real
*/
typedef IloIntVarArray NumVector;
typedef IloArray<IloNumVarArray> NumVarMatrixF;
typedef IloArray<IloIntVarArray> NumVarMatrix;
typedef IloArray<IloNumVarArray> NumVarMatrix2F;
typedef IloArray<IloIntVarArray> NumVarMatrix2;
typedef IloArray<IloArray<IloNumVarArray> > NumVarMatrix3F;
typedef IloArray<IloArray<IloIntVarArray> > NumVarMatrix3;
typedef IloArray<IloArray<IloArray<IloIntVarArray> > > NumVarMatrix4;
typedef IloArray < IloArray<IloArray<IloArray<IloIntVarArray> > > > NumVarMatrix5;
typedef IloArray < IloArray < IloArray<IloArray<IloArray<IloIntVarArray> > > > > NumVarMatrix6;

typedef IloBoolVarArray BoolVector;
typedef IloArray<IloBoolVarArray> BoolVarMatrix;
typedef IloArray<IloBoolVarArray> BoolVarMatrix2;
typedef IloArray<IloArray<IloBoolVarArray> > BoolVarMatrix3;
typedef IloArray<IloArray<IloArray<IloBoolVarArray> > > BoolVarMatrix4;
typedef IloArray < IloArray<IloArray<IloArray<IloBoolVarArray> > > > BoolVarMatrix5;
typedef IloArray < IloArray < IloArray<IloArray<IloArray<IloBoolVarArray> > > > > BoolVarMatrix6;



//#include "GraspCPlex3Indices.cpp"


//extern  "C" int  bouknap(int n, int* p, int* w, int* m, int* x, int c);



void Maintenance::CalcularMaximosEscenario()
{
	M_Cotas_Variables.clear();
	for (int j = 0; j < M_T; j++)
	{
		M_Cotas_Variables.push_back(0.0);
	}
	for (int j = 0; j < M_T; j++)
	{
		if (M_Scenarios_number[j] == 1) continue;
		double max_j = 0;
		for (int l = 0; l < M_Scenarios_number[j]; l++)
		{

			double temp_l = 0;

			for (size_t i=0; i < M_N_Interventions; i++)
			{
				if (M_Interventions[i].Get_RiskC(j) > M_Max_Riesgo) continue;
				for (int tprima = 0; tprima <= j; tprima++)
				{
					if (tprima >= M_Interventions[i].Get_Tmax()) continue;
					if ((tprima + M_Interventions[i].Get_Delta(tprima)) <= j) continue;
					if (tprima > j) continue;
					temp_l+=M_Interventions[i].Get_RiskN(j,tprima,l);
				}
			}
			if (temp_l > max_j)
				max_j = temp_l;
		}
//		cout << j << endl;
		M_Cotas_Variables[j]=max_j;
	}
//	cout << "10" << endl;
}
double Maintenance::IntegerFormulationGRB(int tipo_pool)
{
	// gurobi environment object, we make it global because we only need one
	if (M_Reparar == false)
	{
		for (int i = 0; i < M_N_Interventions; i++)
			M_I_T[i] = -1;
	}
	GRBEnv envGRB = GRBEnv();
//	printf("Entra Gurobi\n");
	int nmbvar = 0;
	std::vector<std::vector<int>> key(M_N_Interventions, std::vector<int>(M_T, -1));
	int K = 0;
	for (int i = 0; i < M_N_Interventions; i++)
	{
		for (int t = 0; t < M_T; t++)
		{
			if (M_Interventions[i].Get_RiskC()[t] > M_Max_Riesgo)
				continue;
			if (M_I_T[i] >= 0) continue;
			key[i][t] = K++;

			nmbvar++;
		}
	}
	try
	{
		GRBModel model = GRBModel(envGRB);
		GRBVar *X = 0;
		X = model.addVars(nmbvar, GRB_BINARY);

		// each intervention is planned
		for (int i = 0; i < M_N_Interventions; i++)
		{
			if (M_I_T[i] >= 0) continue;
			GRBLinExpr expr = 0;
			for (int j = 0; j < M_Interventions[i].Get_Tmax(); j++)
			{
				if (key[i][j] >= 0)
					expr += X[key[i][j]];
			}
			model.addConstr(expr == 1, "i_" + to_string(i));
		}

		// resources consumption
		for (int j = 0; j < M_T; j++)
		{
			for (int k = 0; k < M_Resources.size(); k++)
			{
				double cantidad = 0;
				GRBLinExpr expr = 0;

				for (size_t i = 0; i < M_N_Interventions; i++)
				{
					if (find(M_Interventions[i].Get_Workload_Res().begin(),
					         M_Interventions[i].Get_Workload_Res().end(),
					         k) != M_Interventions[i].Get_Workload_Res().end())
						for (int tprima = 0; tprima <= j; tprima++)
						{
							if (tprima >= M_Interventions[i].Get_Tmax())
								continue;
							if ((tprima + M_Interventions[i].Get_Delta(
							                  tprima)) <= j)
								continue;
							if (tprima > j)
								continue;
							if (M_Interventions[i].Get_RiskC(tprima) >
							    M_Max_Riesgo)
								continue;
							
							if (M_I_T[i] < 0 && (key[i][tprima]>=0))
							{
								if ((key[i][tprima]) >= 0)
									expr += X[key[i][tprima]] *
									M_Interventions[i].Get_WorkloadN(
										k, tprima, j);
								else
									int kk = 9;
							}
							if (M_I_T[i]==tprima)
							expr +=  M_Interventions[i].Get_WorkloadN(
							            k, tprima, j);
							

						}
				}
				model.addConstr(expr >=( M_Resources[k].Get_Lower()[j] - 9e-6 -cantidad),
				                "min_" + to_string(k) + to_string(j));
				model.addConstr(expr <= (M_Resources[k].Get_Upper()[j] + 9e-6 -cantidad),
				                "max_" + to_string(k) + to_string(j));
			}
		}

		// exclusion

		for (list<vector<int>>::iterator it = M_Exclusions.begin();
		     it != M_Exclusions.end(); it++)
		{

			for (int j = 0; j < M_T; j++)
			{
				if (M_T_Season[j] != (*it)[2])
					continue;
				GRBLinExpr expr = 0;
				bool algo1 = false;
				bool algo2 = false;
				for (auto t1 = 0;
				     t1 < M_Interventions[(*it)[0]].Get_Tmax() && t1 <= j; t1++)
				{

					if ((t1 + M_Interventions[(*it)[0]].Get_Delta()[t1]) <= j)
						continue;
					if (M_Interventions[(*it)[0]].Get_RiskC(t1) > M_Max_Riesgo)
						continue;
					if (M_I_T[(*it)[0]] < 0 && (key[(*it)[0]][t1]>=0))
					{ 
						if (key[(*it)[0]][t1] >= 0)
							expr += X[key[(*it)[0]][t1]];
						else
							int kk = 9;
					}
					if (M_I_T[(*it)[0]] == t1)
						expr += 1;					
					algo1 = true;

				}
				for (auto t2 = 0;
				     t2 < M_Interventions[(*it)[1]].Get_Tmax() && t2 <= j; t2++)
				{

					if ((t2 + M_Interventions[(*it)[1]].Get_Delta()[t2]) <= j)
						continue;
					if (M_Interventions[(*it)[1]].Get_RiskC(t2) > M_Max_Riesgo)
						continue;
//					expr += X[(*it)[1]][t2];
//					if (key[(*it)[1]][t2]>=0)
					if (M_I_T[(*it)[1]] < 0 && (key[(*it)[1]][t2]>=0))
					{ 

						if (key[(*it)[1]][t2] >= 0)
							expr += X[key[(*it)[1]][t2]];
						else
							int kk = 9;
					}
					if (M_I_T[(*it)[1]] == t2)
						expr += 1;
						algo2 = true;					
				}
				if (algo1 == true && algo2 == true)
				{
					model.addConstr(expr <= 1);
				}
			}
		}
		
				//Reparar sol
/*		if (M_Reparar == true)
		{
			model.set(GRB_IntParam_Threads, 1);
			for (int i = 0; i < M_N_Interventions; i++)
			{
				for (int j = 0; j  < M_Interventions[i].Get_Tmax(); j++)
				{
					if (M_I_T[i] == j)
					{
						X[key[i][j]].set(GRB_DoubleAttr_LB, 1);
						X[key[i][j]].set(GRB_DoubleAttr_UB,1);

					}
				}
			}
		}
		*/
		// OBJECTIVE
		GRBLinExpr obj = 0;

		int NO_OBJECTIVE = false;
		if (M_Best_I_T.size()==0 || M_Reparar)
			NO_OBJECTIVE = true;
		double Alpha = M_Alpha;
		if (NO_OBJECTIVE == false)
		{
			for (int i = 0; i < M_N_Interventions; i++)
				for (int j = 0; j < M_T; j++)
					if (key[i][j] >= 0)
						obj +=
						    (X[key[i][j]] *
						         (M_Interventions[i].Get_RiskQuantile(j) *
						          (1 - Alpha)) +
						     ((Alpha)*M_Interventions[i].Get_RiskC(j)));
		}
		//obj = data->alpha * obj;
		model.setObjective(obj, GRB_MINIMIZE);


//					if (key[i][j] >= 0)
//					{
//						if (solinitvector[i] == j)
//							X[key[i][j]].set(GRB_DoubleAttr_Start, 1);

//			for (int i = 0; i < M_N_Interventions; i++)
//				for (int j = 0; j < M_T; j++)
//					if (key[i][j] >= 0)
//					{
//						if (solinitvector[i] == j)
//							X[key[i][j]].set(GRB_DoubleAttr_Start, 1);
//						else
//							X[key[i][j]].set(GRB_DoubleAttr_Start, 0);
//					}




		double gap = 0.00;
		model.set(GRB_IntParam_Threads, M_NThreads);
		model.set(GRB_DoubleParam_MIPGap, gap);
		if (M_Reparar)
		model.set(GRB_DoubleParam_TimeLimit,1);
		else
		model.set(GRB_DoubleParam_TimeLimit,150);
		model.set(GRB_IntParam_OutputFlag, 0);
		model.set(GRB_IntParam_Presolve, 1);  // 1 - conservative, 0 - off, 2 - aggressive
		model.set(GRB_IntParam_PrePasses, 1); // do not spend too much time in pre-solve
		if (!M_Reparar)
		model.set(GRB_IntParam_OutputFlag, 1);
//				model.set(GRB_IntParam_OutputFlag, 1);
//		model.write("kk.lp");
		model.optimize();

		

	int optimstatus = model.get(GRB_IntAttr_Status);
	if (optimstatus != GRB_OPTIMAL)
	{
		delete[] X;
//		printf("Sale Gurobi\n");
		return -1;
	}
		double objValue = model.get(GRB_DoubleAttr_ObjVal);
		int Nsol = model.get(GRB_IntAttr_SolCount);
		double val = -1;
		//Create solution
		if (Nsol > 0)
		{
			cout << "Gurobi find solution";

			for (size_t i = 0; i < M_N_Interventions; i++)
			{
				
				for (int l = 0; l < M_Interventions[i].Get_Tmax(); l++)
				{
					if (M_Interventions[i].Get_RiskC()[l] > M_Max_Riesgo)
						continue;
					if (key[i][l] >= 0 && X[key[i][l]].get(GRB_DoubleAttr_X) > 0.99)
					{
						//							cout << M_Interventions[i].Get_Name() << " ";
						//						printf("id %d time %d  \n", i, l);
						if (M_Reparar)
						{
							if (M_I_T[i] < 0)
							{
								M_I_T[i] = l;
								M_Interventions[i].Set_T_Temp(l);
								//				M_I_T_Solution.push_back(p);
								M_Interventions_Colocada[i] = true;
								M_NI_Colocadas++;
								AddScenarios(i);
								AddRecursos(i);
								M_fo_temp += M_Interventions[i].Get_RiskC(l);
								M_Min_fo_temp -=
								    M_Interventions[i]
								        .Get_Min_Risk();

							
							}

						}
						else
						{
							M_I_T[i] = l;
							M_Interventions_Colocada[i] = true;
							M_Interventions[i].Set_T_Temp(l);
						}

						//						fprintf(f, "%s %d w %d h %d s %d id %d\n", x_l[i].getName(), 1, Temp_Conjuntos[i].item_id, Temp_Conjuntos[i].stack_nbr, Temp_Conjuntos[i].number, i);
					}

				}
			}
			val = FuncionObjetivo();
//			InsertarOrdenadoPool(val, M_I_T, M_Pool_Solutions_Integer);
			ActualizarMejorSolution(val, 99);
		}
		/*		for (int i = 0; i < data->nmbInterventions; i++)
		{
			for (int j = 0; j < data->T; j++)
			{
				if (key[i][j] >= 0 && X[key[i][j]].get(GRB_DoubleAttr_X) > 0.99)
				{
					sol->start[i] = j;
				}
			}
		}
*/
		delete[] X;
		printf("Sale Gurobi\n");
		//		return pair<double, double>(objValue, model.get(GRB_DoubleAttr_ObjBound));
		return val;
	}
	catch (GRBException e)
	{
		cout << " Error code = " << e.getErrorCode() << endl << e.getMessage() << endl;
		return 1e15;
	}
	catch (...)
	{
		cout << " Exception during optimization " << endl;
		return -1;
	}

	return 1e15;

	return true;
	//	printf("Aqui666\n");

	
}
int Maintenance::IntegerFormulationFullGRB(int tipo_pool)
{

			M_Time_Optimo = M_Time_Modelo;
	M_Time_Populate = 0;
	if (M_Pool_Solutions)
	{
		M_Time_Optimo = M_Time_Modelo / 2;
		M_Time_Populate = M_Time_Optimo;
		//		M_Time_Populate = 0;
	}
	// gurobi environment object, we make it global because we only need one
	GRBEnv envGRB = GRBEnv();
	int nmbvar = 0;
	std::vector<std::vector<int>> key(M_N_Interventions, std::vector<int>(M_T, -1));
	int K = 0;
	for (int i = 0; i < M_N_Interventions; i++)
		for (int t = 0; t <= M_Interventions[i].Get_Tmax(); t++)
		{
			if (M_Interventions[i].Get_RiskC()[t] > M_Max_Riesgo)
				continue;
			key[i][t] = K++;

			nmbvar++;
		}

	try
	{
		GRBModel model = GRBModel(envGRB);
		GRBVar *X = 0;
		X = model.addVars(nmbvar, GRB_BINARY);

		// each intervention is planned
		for (int i = 0; i < M_N_Interventions; i++)
		{
			GRBLinExpr expr = 0;
			for (int j = 0; j < M_Interventions[i].Get_Tmax(); j++)
			{
				if (key[i][j] >= 0)
					expr += X[key[i][j]];
			}
			model.addConstr(expr == 1, "i_" + to_string(i));
		}

		// resources consumption
		for (int j = 0; j < M_T; j++)
		{
			for (int k = 0; k < M_Resources.size(); k++)
			{

				GRBLinExpr expr = 0;

				for (size_t i = 0; i < M_N_Interventions; i++)
				{
					if (find(M_Interventions[i].Get_Workload_Res().begin(),
					         M_Interventions[i].Get_Workload_Res().end(),
					         k) != M_Interventions[i].Get_Workload_Res().end())
						for (int tprima = 0; tprima <= j; tprima++)
						{
							if (tprima >= M_Interventions[i].Get_Tmax())
								continue;
							if ((tprima + M_Interventions[i].Get_Delta(
							                  tprima)) <= j)
								continue;
							if (tprima > j)
								continue;
							if (M_Interventions[i].Get_RiskC(tprima) >
							    M_Max_Riesgo)
								continue;
							expr += X[key[i][tprima]] *
							        M_Interventions[i].Get_WorkloadN(
							            k, tprima, j);
							/*					if (M_Interventions[i].Get_Workload()[k][j][tprima] > )
										{
											cout << x_ij[i][tprima].getName() << " " << k << " " << j << " " << tprima << " " << M_Interventions[i].Get_Workload()[k][j][tprima];
											cout << endl;
										}*/
						}
				}
				model.addConstr(expr >= M_Resources[k].Get_Lower()[j] - 9e-6,
				                "min_" + to_string(k) + to_string(j));
				model.addConstr(expr <= M_Resources[k].Get_Upper()[j] + 9e-6,
				                "max_" + to_string(k) + to_string(j));
			}
		}

		// exclusion

		for (list<vector<int>>::iterator it = M_Exclusions.begin();
		     it != M_Exclusions.end(); it++)
		{

			for (int j = 0; j < M_T; j++)
			{
				if (M_T_Season[j] != (*it)[2])
					continue;
				GRBLinExpr expr = 0;
				bool algo1 = false;
				bool algo2 = false;
				for (auto t1 = 0;
				     t1 < M_Interventions[(*it)[0]].Get_Tmax() && t1 <= j; t1++)
				{

					if ((t1 + M_Interventions[(*it)[0]].Get_Delta()[t1]) <= j)
						continue;
					if (M_Interventions[(*it)[0]].Get_RiskC(t1) > M_Max_Riesgo)
						continue;
					if (key[(*it)[0]][t1]>=0)
					expr += X[key[(*it)[0]][t1]];
					algo1 = true;
				}
				for (auto t2 = 0;
				     t2 < M_Interventions[(*it)[1]].Get_Tmax() && t2 <= j; t2++)
				{

					if ((t2 + M_Interventions[(*it)[1]].Get_Delta()[t2]) <= j)
						continue;
					if (M_Interventions[(*it)[1]].Get_RiskC(t2) > M_Max_Riesgo)
						continue;
//					expr += X[(*it)[1]][t2];
					if (key[(*it)[1]][t2]>=0)
					expr += X[key[(*it)[1]][t2]];
					algo2 = true;
				}
				if (algo1 == true && algo2 == true)
				{
					model.addConstr(expr <= 1);
				}
			}
		}
				//Reparar sol
		if (M_Reparar == true)
		{
			model.set(GRB_IntParam_Threads, 1);
			for (int i = 0; i < M_N_Interventions; i++)
			{
				for (int j = 0; j < M_T; j++)
				{
					if (M_I_T[i] == j) X[key[i][j]].set(GRB_DoubleAttr_Start, 1);
				}
			}
		}
		// OBJECTIVE
		GRBLinExpr obj = 0;

		int NO_OBJECTIVE = false;
						if (M_Best_I_T.size()==0 || M_Reparar)
					NO_OBJECTIVE = true;
				else
					NO_OBJECTIVE =false;
					int Max_Objectives = Numero_Objectives();
			M_Time_Populate = M_Time_Populate / Max_Objectives;
			

			double gap = 0.1;
			if (M_Reparar)
			model.set(GRB_IntParam_Threads, 1);
			else
			model.set(GRB_IntParam_Threads, 4);
			model.set(GRB_DoubleParam_MIPGap, gap);
			model.set(GRB_DoubleParam_TimeLimit, M_Time_Populate);
			model.set(GRB_IntParam_OutputFlag, 0);
			model.set(GRB_IntParam_Presolve,1); // 1 - conservative, 0 - off, 2 - aggressive
			model.set(GRB_IntParam_PrePasses,1); // do not spend too much time in pre-solve
			model.set(GRB_IntParam_OutputFlag, 1);


			if (!M_Challenge_Mode)
				printf("\nTiempo %d Iteraciones %d Cuantas %d\n", M_Time_Populate,
				       M_Iter, Max_Objectives);
			double Cambio_fobj = 1 / (double)(Max_Objectives + 1);
			int Numero_sol = M_Iter;
			if (M_Iter <= 2)
				Numero_sol = 3;
			Numero_sol = Numero_sol * 2;
			//Fobjetivo
			double Alpha = M_Alpha;

			for (int kiter = 0; kiter < Max_Objectives; kiter++)
			{

				model.set(GRB_IntParam_PoolSolutions, 5 * Numero_sol);
				model.set(GRB_IntParam_PoolSearchMode, 1);
				model.set(GRB_DoubleParam_PoolGap,
				          (double)M_GapPool / (double)100000);
				Alpha = (kiter)*Cambio_fobj;

				for (int i = 0; i < M_N_Interventions; i++)
					for (int j = 0; j < M_T; j++)
						if (key[i][j] >= 0)
							obj +=(X[key[i][j]] *(M_Interventions[i].Get_RiskQuantile(j) *
							          (1 - Alpha)) +((Alpha)*M_Interventions[i].Get_RiskC(j)));
//				if (NO_OBJECTIVE==false)
				model.setObjective(obj, GRB_MINIMIZE);
				//Pool
				model.optimize();
				int Nsol = model.get(GRB_IntAttr_SolCount);
				int ndistintos = M_Integer_Sol_Distintas.size();
				for (auto i = 0; i < Nsol; i++)
				{
					model.set(GRB_IntParam_SolutionNumber, i);
					CrearSolution(key, X);
					double val = FuncionObjetivo();
//					cout << model.get(GRB_DoubleAttr_PoolObjVal) << " " << val   << "";

					int ndistintos1 = M_Integer_Sol_Distintas.size();
					M_Integer_Sol_Distintas.insert(hash_operator(M_I_T));	
					if (ndistintos1 != M_Integer_Sol_Distintas.size())
					{
						InsertarOrdenadoPool(val, M_I_T,
						                     M_Pool_Solutions_Integer);

						ActualizarMejorSolution(val, 99);
					}
				}
				if (M_Best_I_T.size()==0 || M_Reparar)
					NO_OBJECTIVE = true;
				else
					NO_OBJECTIVE =false;
				if (Nsol < (4 * Numero_sol) || (M_Integer_Sol_Distintas.size()-ndistintos)<(4*Numero_sol))
				{
					if (M_GapPool < 10000)
						M_GapPool += (M_GapPool * 50) / 100;
					Numero_sol = Numero_sol * 2;
				}
				else
				{

					Numero_sol = Numero_sol * 1.5;
					if (M_GapPool > 10)
						M_GapPool -= (M_GapPool * 50) / 100;

				}
			}

		





		delete[] X;
		
		//		return pair<double, double>(objValue, model.get(GRB_DoubleAttr_ObjBound));
		return true;
	}
	catch (GRBException e)
	{
		cout << " Error code = " << e.getErrorCode() << endl << e.getMessage() << endl;
		return 1e15;
	}
	catch (...)
	{
		cout << " Exception during optimization " << endl;
		return 1e15;
	}

	return 1e15;

	return true;
	//	printf("Aqui666\n");

	
}
void Maintenance::CrearSolution(std::vector<std::vector<int>> &key1, GRBVar *Xsol)
{
	for (size_t i = 0; i < M_N_Interventions; i++)
	{
		M_Interventions_Colocada[i] = false;
		for (int l = 0; l < M_T; l++)
		{

			if (M_Interventions[i].Get_RiskC(l) > M_Max_Riesgo)
				continue;
			if (key1[i][l] >= 0 && Xsol[key1[i][l]].get(GRB_DoubleAttr_Xn) > 0.99)
			{
				//							cout << M_Interventions[i].Get_Name() << " ";
				//						printf("id %d time %d  \n", i, l);
				if (M_Reparar)
				{
					if (M_I_T[i] < 0)
					{
						M_I_T[i] = l;
						M_Interventions[i].Set_T_Temp(l);
						//				M_I_T_Solution.push_back(p);
						M_Interventions_Colocada[i] = true;
						M_NI_Colocadas++;
						AddScenarios(i);
						AddRecursos(i);
						M_fo_temp += M_Interventions[i].Get_RiskC(l);
					}
				}
				else
				{
					M_I_T[i] = l;
					M_Interventions_Colocada[i] = true;
					M_Interventions[i].Set_T_Temp(l);
				}

				//						fprintf(f, "%s %d w %d h %d s %d id %d\n", x_l[i].getName(), 1, Temp_Conjuntos[i].item_id, Temp_Conjuntos[i].stack_nbr, Temp_Conjuntos[i].number, i);
			}
		}
	}
}
int Maintenance::IntegerFormulation(int tipo_pool)
{


	//	M_Max_Riesgo =2*( M_T) * M_Cota;
	//for (size_t i=0; i < 100; i++)
	cout << "kk" << endl;

			M_Time_Optimo = M_Time_Modelo;
	M_Time_Populate = 0;
	if (M_Pool_Solutions)
	{
		M_Time_Optimo = M_Time_Modelo / 2;
		M_Time_Populate = M_Time_Optimo*2;
		//		M_Time_Populate = 0;
	}

	IloEnv env;
	IloNumVarArray vars(env);
	IloNumArray vals(env);
	//	CPXENVptr env;
	//	double value2;
	IloModel model = IloModel(env);
	IloRangeArray restr(env);
	NumVarMatrix2 x_ij(env, M_N_Interventions);
	IloIntVarArray m_j(env, M_T);
	IloIntVarArray M_j(env, M_T);

	//***************** INICIALIZO VARIABLES ****************

	//inicializo las variable y_l
	//	printf("Aqui21\n");
	char nombre[20];

	M_Max_Riesgo = M_Max_Min_Riesgo * 2;
	int num_var = 0;
	for (int i = 0; i < M_N_Interventions; i++)
	{
		x_ij[i] = IloIntVarArray(env, M_T);
		for (int l = 0; l < M_T; l++)
		{
			if (M_Interventions[i].Get_RiskC()[l] > M_Max_Riesgo)
				continue;

			num_var++;
			x_ij[i][l] = IloIntVar(env, 0, 1);

			sprintf(nombre, "X_%d_%d", i, l);
			x_ij[i][l].setName(nombre);
		}
	}
	printf("num_var %d", num_var);
	if (M_Max_Formulation && M_Pool_Solutions)
	{
		for (int i = 0; i < M_T; i++)
		{

			M_j[i] = IloIntVar(env, 0, M_Cotas_Variables[i]);
			sprintf(nombre, "M_%d", i);
			M_j[i].setName(nombre);
		}
	}
	if (M_Min_Formulation && M_Pool_Solutions)
	{
		for (int i = 0; i < M_T; i++)
		{

			M_j[i] = IloIntVar(env, 0, M_Cotas_Variables[i]);
			sprintf(nombre, "m_%d", i);
			M_j[i].setName(nombre);
		}
	}
	//*********************************************************
	//***************** RESTRICCIONES *************************
	//*********************************************************
	//	printf("Aqui22 Temp_Conjuntos %d\n", Temp_Conjuntos.size());

	//Todas tienen que estar asignadas

	for (size_t i = 0; i < M_N_Interventions; i++)
	{
		IloExpr v1(env);
		for (int l = 0; l < M_Interventions[i].Get_Tmax(); l++)
		{
			if (M_Interventions[i].Get_RiskC(l) > M_Max_Riesgo)
				continue;
			v1 += x_ij[i][l];
		}

		restr.add(IloRange(env, 1, v1, 1));
		v1.end();
	}

	//Recursos
	int kk = 9;

	for (int j = 0; j < M_T; j++)
	{
		for (int k = 0; k < M_Resources.size(); k++)
		{

			IloExpr vlower(env);

			for (size_t i = 0; i < M_N_Interventions; i++)
			{
				if (find(M_Interventions[i].Get_Workload_Res().begin(),
				         M_Interventions[i].Get_Workload_Res().end(),
				         k) != M_Interventions[i].Get_Workload_Res().end())
					for (int tprima = 0; tprima <= j; tprima++)
					{
						if (tprima >= M_Interventions[i].Get_Tmax())
							continue;
						if ((tprima +
						     M_Interventions[i].Get_Delta(tprima)) <= j)
							continue;
						if (tprima > j)
							continue;
						if (M_Interventions[i].Get_RiskC(tprima) >
						    M_Max_Riesgo)
							continue;
						vlower +=
						    x_ij[i][tprima] *
						    M_Interventions[i].Get_WorkloadN(k, tprima, j);
					}
			}

			restr.add(IloRange(env, M_Resources[k].Get_Lower()[j], vlower,
			                   M_Resources[k].Get_Upper()[j]));
			vlower.end();
		}
	}
	//Exclusions

	for (list<vector<int>>::iterator it = M_Exclusions.begin(); it != M_Exclusions.end(); it++)
	{

		for (int j = 0; j < M_T; j++)
		{
			if (M_T_Season[j] != (*it)[2])
				continue;
			IloExpr v1(env);
			bool algo1 = false;
			bool algo2 = false;
			for (auto t1 = 0; t1 < M_Interventions[(*it)[0]].Get_Tmax() && t1 <= j;
			     t1++)
			{

				if ((t1 + M_Interventions[(*it)[0]].Get_Delta()[t1]) <= j)
					continue;
				if (M_Interventions[(*it)[0]].Get_RiskC(t1) > M_Max_Riesgo)
					continue;
				v1 += x_ij[(*it)[0]][t1];
				algo1 = true;
			}
			for (auto t2 = 0; t2 < M_Interventions[(*it)[1]].Get_Tmax() && t2 <= j;
			     t2++)
			{

				if ((t2 + M_Interventions[(*it)[1]].Get_Delta()[t2]) <= j)
					continue;
				if (M_Interventions[(*it)[1]].Get_RiskC(t2) > M_Max_Riesgo)
					continue;
				v1 += x_ij[(*it)[1]][t2];
				algo2 = true;
			}
			if (algo1 == true && algo2 == true)
			{
				restr.add(IloRange(env, -IloInfinity, v1, 1));
				v1.end();
			}
		}
	}
	if (M_Max_Formulation && M_Pool_Solutions)
	{
		for (int j = 0; j < M_T; j++)
		{

			for (int l = 0; l < M_Scenarios_number[j]; l++)
			{
				if (M_Scenarios_number[j] == 1)
					continue;
				IloExpr vlower(env);
				for (size_t i = 0; i < M_N_Interventions; i++)
				{

					for (int tprima = 0; tprima <= j; tprima++)
					{
						if (tprima >= M_Interventions[i].Get_Tmax())
							continue;
						if ((tprima +
						     M_Interventions[i].Get_Delta()[tprima]) <= j)
							continue;
						if (tprima > j)
							continue;
						if (M_Interventions[i].Get_RiskC(tprima) >
						    M_Max_Riesgo)
							continue;
						if (M_Interventions[i].Get_RiskN(j, tprima, l) >
						    __FLT_EPSILON__)
						{
							if (tipo_pool != 4)
								vlower +=
								    x_ij[i][tprima] *
								    M_Interventions[i].Get_RiskN(
								        j, tprima, l);
							else
								vlower +=
								    x_ij[i][tprima] *
								    (M_Interventions[i]
								         .Get_RiskQuantile(j) -
								     M_Interventions[i].Get_RiskN(
								         j, tprima, l));
						}
					}
				}
				vlower += (-1) * M_j[j];
				restr.add(IloRange(env, -IloInfinity, vlower, 0));
				vlower.end();
			}
		}
	}
	if (tipo_pool == 4)
		tipo_pool = 3;

	if (M_Min_Formulation && M_Pool_Solutions)
	{
		for (int j = 0; j < M_T; j++)
		{

			for (int l = 0; l < M_Scenarios_number[j]; l++)
			{
				if (M_Scenarios_number[j] == 1)
					continue;
				IloExpr vlower(env);
				for (size_t i = 0; i < M_N_Interventions; i++)
				{

					for (int tprima = 0; tprima <= j; tprima++)
					{
						if (tprima >= M_Interventions[i].Get_Tmax())
							continue;
						if ((tprima +
						     M_Interventions[i].Get_Delta()[tprima]) <= j)
							continue;
						if (tprima > j)
							continue;
						if (M_Interventions[i].Get_RiskC(tprima) >
						    M_Max_Riesgo)
							continue;
						if (M_Interventions[i].Get_RiskN(j, tprima, l) >
						    __FLT_EPSILON__)
							vlower += x_ij[i][tprima] *
							          M_Interventions[i].Get_RiskN(
							              j, tprima, l);
					}
				}
				vlower += (-1) * M_j[j];
				restr.add(IloRange(env, 0, vlower, IloInfinity));
				vlower.end();
			}
		}
	}

	//Fijar variables

	//Restricciones
	//AÑADIMOS LAS RESTRICCIONES AL MODELO
	model.add(restr);

	//******************************************************
	//***************FUNCION OBJETIVO **********************
	//******************************************************

	double Alpha = M_Alpha;
	IloExpr Expression_fobj(env);
	for (size_t i = 0; i < M_N_Interventions; i++)
	{
		for (int l = 0; l < M_T; l++)
		{

			if (M_Interventions[i].Get_RiskC(l) > M_Max_Riesgo)
				continue;
			if (l >= M_Interventions[i].Get_Tmax())
				continue;
			if (M_Mixed_Risk)
				Expression_fobj +=
				    x_ij[i][l] *
				    ((M_Interventions[i].Get_RiskC(l) * M_Alpha) +
				     (M_Interventions[i].Get_RiskExcess(l) * (1 - M_Alpha)));
			else
			{
				if (M_Formulacion_Cuantiles)
				{
					Expression_fobj +=
					    ((x_ij[i][l] * M_Interventions[i].Get_RiskQuantile(l) *
					      (1 - M_Alpha)) +
					     ((2 * Alpha - 1) * x_ij[i][l] *
					      M_Interventions[i].Get_RiskC(l)));
				}
				else
				{
					Expression_fobj +=
					    x_ij[i][l] * M_Interventions[i].Get_RiskC(l) * M_Alpha;
				}
			}
		}
	}
	if (M_Max_Formulation && M_Pool_Solutions)
	{
		double Val = 1;
		for (int j = 0; j < M_T; j++)
		{
			if (M_Scenarios_number[j] == 1)
				continue;
			Expression_fobj += M_j[j] * (1 - M_Alpha) * Val;
			if (M_Min_Formulation)
				Expression_fobj += M_j[j] * (1 - M_Alpha) * (-1);
			{
				for (size_t i = 0; i < M_N_Interventions; i++)
				{
					if (M_Interventions[i].Get_RiskC(j) > M_Max_Riesgo)
						continue;
					if (j >= M_Interventions[i].Get_Tmax())
						continue;
					Expression_fobj += x_ij[i][j] *
					                   M_Interventions[i].Get_RiskC(j) *
					                   (M_Alpha)*Val;
				}
			}
		}
	}

	IloObjective fobj = IloMinimize(env, Expression_fobj);
	model.add(fobj);
	Expression_fobj.end();
	//	printf("Aqui66\n");

	//**************************************************************
	//*************** EJECUTAMOS EL MODELO *************************
	IloCplex cplex(model);
	struct timeb t1;
	struct timeb time_ini;
	double time = 0;
	ftime(&time_ini);
	double value2 = 0;

	try
	{
		int Max_Objectives = Numero_Objectives();
		M_Time_Populate = M_Time_Populate / Max_Objectives;
		if (!M_Challenge_Mode)
			printf("\nTiempo %d Iteraciones %d Cuantas %d TiempoT %d\n", M_Time_Populate, M_Iter,
			       Max_Objectives,M_Time_Optimo/2);
		double Cambio_fobj =1 / (double)(Max_Objectives + 1);
		int Numero_sol = M_Iter;
		if (M_Iter <= 2)
			Numero_sol = 3;
		Numero_sol = Numero_sol * 2;
		double Tiempo_Total_Cplex = (Max_Objectives) * M_Time_Populate  ;
		for (int kiter = 0; kiter < Max_Objectives; kiter++)
		{

			Alpha = (kiter)*Cambio_fobj;
			if (Alpha > 1)
				Alpha = Alpha - ((int)Alpha);
			if (!M_Challenge_Mode)
				printf("Iter %d Num_Sol %d\n", kiter,
				       M_Pool_Solutions_Integer.size());
			IloNumVarArray vars_obj(env);
			IloNumArray vals_obj(env);

			for (size_t i = 0; i < M_N_Interventions; i++)
			{
				for (int l = 0; l < M_T; l++)
				{

					if (M_Interventions[i].Get_RiskC(l) > M_Max_Riesgo)
						continue;
					if (l >= M_Interventions[i].Get_Tmax())
						continue;
					vars_obj.add(x_ij[i][l]);

//					if (kiter%3!=0)
					vals_obj.add((M_Interventions[i].Get_RiskQuantile(l) * (1 - Alpha)) + ((Alpha)*M_Interventions[i].Get_RiskC(l)));

//					vals_obj.add(round((((M_Interventions[i].Get_RiskQuantile(l) * (1 - Alpha)) + ((Alpha)*M_Interventions[i].Get_RiskC(l)))/100)));
//					else
//					vals_obj.add(((1+Alpha)*M_Interventions[i].Get_RiskCxT(l)));

				}
			}

			fobj.setLinearCoefs(vars_obj, vals_obj);
			if (M_Challenge_Mode)
				cplex.setOut(env.getNullStream());
			cplex.setParam(IloCplex::Threads, M_Number_Threads);
			cplex.setParam(IloCplex::Threads, 4);
//			cplex.setParam(IloCplex::Probe,-1);
			if (M_Best_I_T.size() != 0 && kiter!=0)
			{
//				cplex.setParam(IloCplex::HeuristicEffort, 10);

				cplex.setParam(IloCplex::WorkMem, 64);
				if (!M_Pool_Solutions)
				{
					cplex.setParam(IloCplex::EpGap, 0);
					cplex.setParam(IloCplex::EpAGap, 0);
				}
				else
				{
					cplex.setParam(IloCplex::EpGap, 0.00001);
				}
				cplex.setParam(IloCplex::EpGap, 0);
				cplex.setParam(IloCplex::EpAGap, 0);
			}

			if (M_Pool_Solutions == true && M_Best_I_T.size() != 0)
			{
				cplex.setParam(IloCplex::SolnPoolGap, 0.1);

				cplex.setParam(IloCplex::PopulateLim,
				               ((M_Total_Time / 400) + 1) * 5 * Numero_sol);
			}
			//limite de tiempo
			cplex.setParam(IloCplex::TiLim, M_Time_Optimo/2); //TIEMPO EN SEGUNDOS

			double best_fo = __FLT_MAX__;

			IloNum inicio, fin;
			clock_t t_ini, t_fin;

			inicio = cplex.getCplexTime();
			t_ini = clock();
			//Solucion inicial
			if (M_Best_I_T.size() != 0 || M_I_T_ForQ.size() != 0)
			{
				if (!M_Challenge_Mode)
					printf("Pongo Sol");

				if (M_I_T_ForQ.size() == 0)
				{

					for (
					    int i = 0; i < M_N_Interventions;
					    i++) //list<pair<int, int>>::iterator it = M_Best_I_T_Solution.begin(); it != M_Best_I_T_Solution.end(); it++)
					{
						vars.add(x_ij[i][M_Best_I_T[i]]);
						vals.add(1.0);
					}
				}
				else
				{
					if (tipo_pool == 1 && M_I_T_For.size() != 0)
					{
						for (
						    int i = 0; i < M_N_Interventions;
						    i++) //list<pair<int, int>>::iterator it = M_Best_I_T_Solution.begin(); it != M_Best_I_T_Solution.end(); it++)
						{
							vars.add(x_ij[i][M_I_T_For[i]]);
							vals.add(1.0);
						}
					}
					else
					{
						if (M_I_T_ForQ.size() != 0)
						{
							for (
							    int i = 0; i < M_N_Interventions;
							    i++) //list<pair<int, int>>::iterator it = M_Best_I_T_Solution.begin(); it != M_Best_I_T_Solution.end(); it++)
							{
								vars.add(x_ij[i][M_I_T_ForQ[i]]);
								vals.add(1.0);
							}
						}
					}
				}
				cplex.addMIPStart(vars, vals, cplex.MIPStartRepair);
				vars.clear();
			}
			int pp = 9;
			IloBool result = 0;
			if (M_Best_I_T.size() == 0 || !M_Pool_Solutions)
			{
				result = cplex.solve();
			}

			//Estado del algoritmo	al terminar
			IloCplex::Status estado = cplex.getCplexStatus();
			if (!M_Challenge_Mode)
				cout << estado << endl;
			IloCplex::Status sub_estado = cplex.getCplexSubStatus();
			if (!M_Challenge_Mode)
				cout << sub_estado << endl;
			int numsol2 = cplex.getSolnPoolNsolns();
			if (numsol2 > 0)
			{

				for (int sol = 0; sol < numsol2; sol++)
				{
					for (size_t i = 0; i < M_N_Interventions; i++)
					{
						M_Interventions_Colocada[i] = false;
						for (int l = 0; l < M_T; l++)
						{

							if (M_Interventions[i].Get_RiskC(l) >
							    M_Max_Riesgo)
								continue;
							if (cplex.isExtracted(x_ij[i][l]) &&
							    cplex.getValue(x_ij[i][l], sol) > 0.001)
							{
								M_I_T[i] = l;
								M_Interventions_Colocada[i] = true;
								M_Interventions[i].Set_T_Temp(l);
							}
						}
					}
					int ndistintos = M_Integer_Sol_Distintas.size();
					M_Integer_Sol_Distintas.insert(hash_operator(M_I_T));

					if (ndistintos == M_Integer_Sol_Distintas.size())
					{
						if (!M_Challenge_Mode)
							printf("Ya estaba\n");
						continue;
					}
					else
					{
						double val = 0;
						switch (tipo_pool)
						{
						case 1:
							val = FuncionObjetivo();
							if (val < M_Best_Integer_For)
							{
								M_Best_Integer_For = val;
								M_I_T_For = M_I_T;
							}
							InsertarOrdenadoPool(
							    val, M_I_T, M_Pool_Solutions_Integer);
							break;
						case 2:
							val = FuncionObjetivo();
							if (val < M_Best_Integer_ForQ)
							{

								M_Best_Integer_ForQ = val;
								M_I_T_ForQ = M_I_T;
							}
							InsertarOrdenadoPool(
							    val, M_I_T, M_Pool_Solutions_Integer);
							break;
						case 3:
							val = FuncionObjetivo();
							if (val < M_Best_Integer_ForMax)
								M_Best_Integer_ForMax = val;
							InsertarOrdenadoPool(
							    val, M_I_T, M_Pool_Solutions_Integer);
							break;
						default:
							val = FuncionObjetivo();
							if (val < M_Best_Integer_For)
								M_Best_Integer_For = val;
							InsertarOrdenadoPool(
							    val, M_I_T, M_Pool_Solutions_Integer);
							break;
						}
					}
					if (tipo_pool == 0)
					{
						double val = FuncionObjetivo();
						VerifySolution2();
						if (val < best_fo)
							best_fo = val;
					}
				}
			}

			t_fin = clock();

			fin = cplex.getCplexTime();
			if ((2 * (fin - inicio)) > M_Time_Optimo)
				M_GapPool = M_GapPool + 200;
			if (M_Pool_Solutions == true && M_Time_Populate > 0)
			{
				cplex.setParam(IloCplex::SolnPoolReplace, 2);
				cplex.setParam(IloCplex::SolnPoolCapacity, 5 * Numero_sol);
				cplex.setParam(IloCplex::PopulateLim, 5 * Numero_sol);
				cplex.setParam(IloCplex::SolnPoolGap,
				               (double)M_GapPool / (double)100000);
				cplex.setParam(IloCplex::EpGap, (double)M_GapPool / (double)100000);
				cplex.setParam(IloCplex::EpAGap,
				               (double)M_GapPool / (double)100000);
			}
			double secs = (double)(fin - inicio);
			cplex.setParam(IloCplex::TiLim, M_Time_Populate );
			if (M_Pool_Solutions)
			{

				result = cplex.populate();

				if (!M_Challenge_Mode)
					env.out()
					    << "Solution status = " << cplex.getStatus() << endl;
				if (!M_Challenge_Mode)
					env.out()
					    << "Incumbent objective value = " << cplex.getObjValue()
					    << endl;
			}

			int numsol = cplex.getSolnPoolNsolns();
			if (!M_Challenge_Mode)
				printf("Creadas Numsol %d", numsol);

			if (!M_Challenge_Mode)
				env.out() << "The solution pool contains " << numsol
				          << " solutions." << endl;
			int NumSol1 = M_Pool_Solutions_Integer.size();
			if (numsol > 0)
			{
				// Some solutions are deleted from the pool because of the solution 				   pool relative gap parameter

				int numsolreplaced = cplex.getSolnPoolNreplaced();
				if (!M_Challenge_Mode)
					env.out() << numsolreplaced
					          << " solutions were removed due to the "
					             "solution pool relative gap parameter."
					          << endl;

				if (!M_Challenge_Mode)
					env.out() << "In total, " << numsol + numsolreplaced
					          << " solutions were generated." << endl;

				// Get the average objective value of solutions in the solution 				   pool

				if (!M_Challenge_Mode)
					env.out() << "The average objective value of the "
					             "solutions is "
					          << cplex.getSolnPoolMeanObjValue() << "." << endl
					          << endl;
				fin = cplex.getCplexTime();

				//		double secs = (double)(fin - inicio);
				M_Time_Modelo2 = (double)(fin - inicio);
				// Write out the objective value of each solution and its   difference to the incumbent
				vector<int> IDtemp(numsol);
				if (M_Interventions_Colocada.size() == 0)
				{
					for (size_t i = 0; i < M_N_Interventions; i++)
					{

						M_Interventions_Colocada.push_back(false);
					}
				}
				else
				{
					for (size_t i = 0; i < M_N_Interventions; i++)
					{

						M_Interventions_Colocada[i]=false;
					}
	
				}
				for (size_t i = 0; i < numsol; i++)
					IDtemp[i] = i;
				random_shuffle(IDtemp.begin(), IDtemp.end());

				for (int s1 = 0; s1 < numsol; s1++)
				{
					int sol = IDtemp[s1];
					for (size_t i = 0; i < M_N_Interventions; i++)
					{
						M_Interventions_Colocada[i] = false;
						for (int l = 0; l < M_T; l++)
						{

							if (M_Interventions[i].Get_RiskC(l) >
							    M_Max_Riesgo)
								continue;
							if (cplex.isExtracted(x_ij[i][l]) &&
							    cplex.getValue(x_ij[i][l], sol) > 0.001)
							{
								M_I_T[i] = l;
								M_Interventions_Colocada[i] = true;
								M_Interventions[i].Set_T_Temp(l);
							}
						}
					}
					int ndistintos = M_Integer_Sol_Distintas.size();
					M_Integer_Sol_Distintas.insert(hash_operator(M_I_T));
					if (ndistintos == M_Integer_Sol_Distintas.size())
					{
						if (!M_Challenge_Mode)
							printf("Ya estaba\n");
						continue;
					}
					else
					{
						double val = 0;
						double val21 = 0;
						switch (tipo_pool)
						{
						case 1:
							val = FuncionObjetivo();
							if (val < M_Best_Integer_For)
								M_Best_Integer_For = val;
							InsertarOrdenadoPool(
							    val, M_I_T, M_Pool_Solutions_Integer);
							break;
						case 2:
							val = FuncionObjetivo();
							if (val < M_Best_Integer_ForQ)
								M_Best_Integer_ForQ = val;
							InsertarOrdenadoPool(
							    val, M_I_T, M_Pool_Solutions_Integer);
	/*
							CrearSolucion(M_I_T);
								for (register int i = 0; i < M_T; i++)
	{
		M_T_M1_Suma[i] = 0;
		M_T_M2_Suma[i] = 0;
		M_T_M3_Suma[i] = 0;
	}
								M_Busqueda_Local = true;
							val21 = MejoraIntercambiosMejorT();
							M_Busqueda_Local = false;
							printf("Ini %.2f Fin %.2f\n", val, val21);
							if ( val>val21 + __FLT_EPSILON__)
							{
								val = val21;
								ndistintos = M_Integer_Sol_Distintas.size();
								M_Integer_Sol_Distintas.insert(hash_operator(M_I_T));
								if (ndistintos == M_Integer_Sol_Distintas.size())
								{
									if (!M_Challenge_Mode)
										printf("Ya estaba\n");
									continue;
								}
															if (val < M_Best_Integer_ForQ)
								M_Best_Integer_ForQ = val;
							InsertarOrdenadoPool(
							    val, M_I_T, M_Pool_Solutions_Integer);

							}
		*/					
							break;
						case 3:
							val = FuncionObjetivo();
							if (val < M_Best_Integer_ForMax)
								M_Best_Integer_ForMax = val;
							InsertarOrdenadoPool(
							    val, M_I_T, M_Pool_Solutions_Integer);
							break;
						default:
							val = FuncionObjetivo();
							if (val < M_Best_Integer_For)
								M_Best_Integer_For = val;
							InsertarOrdenadoPool(
							    val, M_I_T, M_Pool_Solutions_Integer);
							break;
						}
					}
					if (tipo_pool == 0)
					{
						double val = FuncionObjetivo();
						VerifySolution2();
						if (val < best_fo)
							best_fo = val;
					}
				}
				t_fin = clock();
				M_Time_Modelo3 = (double)(t_fin - t_ini) / CLOCKS_PER_SEC;
			}
			if (!M_Challenge_Mode)
				printf("Metidas Numsol %d",
				       M_Pool_Solutions_Integer.size() - NumSol1);
			int salto = (M_GapPool * 50) / 100;
			if (numsol < 4 * Numero_sol)
			{
				if (M_GapPool < 10000)
					M_GapPool += salto;
			}
			else
			{
				if ((M_Pool_Solutions_Integer.size() - NumSol1) < 2 * Numero_sol)
					Numero_sol = Numero_sol * 1.5;
				if (M_GapPool > 10)
					M_GapPool -= salto;
			}
			t_fin = clock();
			fin = cplex.getCplexTime();

			secs = (double)(fin - inicio);
			//Estado del algoritmo	al terminar
			estado = cplex.getCplexStatus();
			sub_estado = cplex.getCplexSubStatus();
			if (result == false)
			{

				if (cplex.getStatus() == IloAlgorithm::Infeasible)
				{
					printf("Infeasible\n");
					//			cplex.exportModel("Roadef.lp");
				}
				else
				{
					if (cplex.getStatus() == IloAlgorithm::Unbounded)
						printf("Unbounded\n");
					//			printf("\n%s \tnosol;%.2f;%.2f;MaxTiras %d\t MaxPilas %d\t  Bins %d Tiras %d Pilas %d\n", name_instance.c_str(), cplex.getNnodes(), cplex.getTime(), maxTiras, maxPilas, numbins, numtiras, numpilas);
				}
			}
			else
			{
				value2 = cplex.getObjValue();
				//				cout << ((value2) / (double)M_T) << endl;
				M_sol_formulation = ((value2) / (double)M_T);
				//			double LB_Best_Value_Formulation = cplex.getBestObjValue();
				//Si ha encontrado alguna solucion posible

				if (cplex.getSolnPoolNsolns() > 0)
				{

					for (size_t i = 0; i < M_N_Interventions; i++)
					{
						for (int l = 0; l < M_T; l++)
						{

							if (M_Interventions[i].Get_RiskC(l) >
							    M_Max_Riesgo)
								continue;

							if (cplex.isExtracted(x_ij[i][l]) &&
							    cplex.getValue(x_ij[i][l]) > 0.001)
							{
								//							cout << M_Interventions[i].Get_Name() << " ";
								//							printf("id %d time %d  \n",  i, l);

								M_I_T[i] = l;
								//							M_Interventions_Colocada[i] = true;
								M_Interventions[i].Set_T_Temp(l);
								//						fprintf(f, "%s %d w %d h %d s %d id %d\n", x_l[i].getName(), 1, Temp_Conjuntos[i].item_id, Temp_Conjuntos[i].stack_nbr, Temp_Conjuntos[i].number, i);
							}
						}
						//				printf("Aqui27\n");
					}
				}
				//			printf("Aqui288");
				//}
			}

			//		printf("Aqui299");

			double val = 0;
			switch (tipo_pool)
			{
			case 1:
				val = FuncionObjetivo();
				if (val < M_Best_Integer_For)
					M_Best_Integer_For = val;
				InsertarOrdenadoPool(val, M_I_T, M_Pool_Solutions_Integer);
				break;
			case 2:
				val = FuncionObjetivo();
				if (val < M_Best_Integer_ForQ)
					M_Best_Integer_ForQ = val;
				InsertarOrdenadoPool(val, M_I_T, M_Pool_Solutions_Integer);
				break;
			case 3:
				val = FuncionObjetivo();
				if (val < M_Best_Integer_ForMax)
					M_Best_Integer_ForMax = val;
				InsertarOrdenadoPool(val, M_I_T, M_Pool_Solutions_Integer);
				break;
			default:
				val = FuncionObjetivo();
				if (val < M_Best_Integer_For)
					M_Best_Integer_For = val;
				InsertarOrdenadoPool(val, M_I_T, M_Pool_Solutions_Integer);
				break;
			}

			if (tipo_pool == 0)
			{
				double kk = FuncionObjetivo();
				VerifySolution(M_I_T_Solution_Best);

				M_fo_formulation1 = kk;
				EjecutarValidadorPython(M_Best_I_T);
				if (kk < best_fo)
					best_fo = kk;
				M_fo_formulation2 = best_fo;
			}
			ftime(&t1);
			time = ((double)((t1.time - time_ini.time) * 1000 + t1.millitm - time_ini.millitm)) / 1000;
			if (!M_Challenge_Mode) printf("\nTiempo lleva Cplex %.2f Tiempo total %.2f\n", time, Tiempo_Total_Cplex);
			if ((kiter<30) && (kiter == Max_Objectives - 1) && time < (Tiempo_Total_Cplex-10  ))
			{
				M_Time_Populate = Tiempo_Total_Cplex - time;
				Max_Objectives++;
				Cambio_fobj = ((double)get_random(10,90)/(double)(100*(kiter+1)));

			}

		}
		//	Expression_fobj.end();
		Expression_fobj.end();

		cplex.end();
		model.end();
		env.end();
		return value2 * M_Alpha;
	}
	catch (IloException &e)
	{
		Expression_fobj.end();

		printf("EXCEPCION");
		cerr << "IloException: " << e << "Status" << e;
		cplex.end();
		model.end();
		env.end();
	}

	return true;
	//	printf("Aqui666\n");
}
int Maintenance::Numero_Objectives()
{
//	M_Iter = 2 * M_Iter;
			int Max_Objectives = 10;
		if (M_Iter <= 16 )
			Max_Objectives = 8;
		if (M_Iter <= 12 )
			Max_Objectives = 6;
		if (M_Iter <= 8 )
			Max_Objectives = 4;
		if (M_Iter <= 4 )
			Max_Objectives = 1;
		if (Max_Objectives == 2 && M_Time_Populate < 50)
			Max_Objectives = 1;
		if (M_Time_Populate > 200)
			Max_Objectives = 2 * Max_Objectives;
		if (M_Time_Populate > 400)
			Max_Objectives = 4 * Max_Objectives;
		if (Max_Objectives == 1)
			Max_Objectives = 2;
		return Max_Objectives;
}
double Maintenance::IntegerFormulationPartial(int Val)
{

	//	M_Max_Riesgo =2*( M_T) * M_Cota;
	//for (size_t i=0; i < 100; i++)
//	cout << "kk" << endl;
	M_Pool_Solutions = false;
	vector<int> Temp_I_T = M_I_T;



	IloEnv env;
	//	CPXENVptr env;
	//	double value2;
	IloModel model = IloModel(env);
	IloRangeArray restr(env);
	NumVarMatrix2 x_ij(env, M_N_Interventions);
	IloIntVarArray m_j(env, M_T);
	IloIntVarArray M_j(env, M_T);


	/***************** INICIALIZO VARIABLES *******************/
/**********************************************************/
	//inicializo las variable y_l
//	printf("Aqui21\n");
	char nombre[20];




	for (int i=0; i < M_N_Interventions; i++)
	{
		if (M_I_T[i] < 0) {
		x_ij[i] = IloIntVarArray(env, M_T);
		//if (M_I_T[i] >= 0)
		//{
		//	x_ij[i][M_I_T[i]] = IloIntVar(env, 1, 1);
		//	sprintf(nombre, "X_%d_%d", i, M_I_T[i]);
		//	x_ij[i][M_I_T[i]].setName(nombre);
		//}
		//else
//		{
			for (int l = 0; l < M_T; l++)
			{
				if (M_Interventions[i].Get_RiskC()[l] > M_Max_Riesgo)
					continue;

				x_ij[i][l] = IloIntVar(env, 0, 1);
				sprintf(nombre, "X_%d_%d", i, l);
				x_ij[i][l].setName(nombre);


			}
		}


	}
	if (M_Max_Formulation && M_Pool_Solutions)
	{
		for (int i = 0; i < M_T; i++)
		{

			M_j[i] = IloIntVar(env, 0, M_Cotas_Variables[i]);
			sprintf(nombre, "M_%d", i);
			M_j[i].setName(nombre);

		}

	}
	if (M_Min_Formulation && M_Pool_Solutions)
	{
		for (int i = 0; i < M_T; i++)
		{

			M_j[i] = IloIntVar(env, 0, M_Cotas_Variables[i]);
			sprintf(nombre, "m_%d", i);
			M_j[i].setName(nombre);

		}

	}
	/**********************************************************/
	/***************** RESTRICCIONES **************************/
	/**********************************************************/
//	printf("Aqui22 Temp_Conjuntos %d\n", Temp_Conjuntos.size());

	//Todas tienen que estar asignadas



	for (size_t i=0; i < M_N_Interventions; i++)
	{
		if (M_I_T[i] < 0)
		{
			IloExpr v1(env);
			for (int l = 0; l < M_Interventions[i].Get_Tmax(); l++)
			{
				if (M_Interventions[i].Get_RiskC(l) > M_Max_Riesgo)
					continue;
				v1 += x_ij[i][l];
			}
			restr.add(IloRange(env, 1, v1, 1));
			v1.end();
		}




	}
	//Recursos

	for (int j = 0; j < M_T; j++)
	{
		for (int k = 0; k < M_Resources.size(); k++)
		{

			IloExpr vlower(env);

			for (size_t i=0; i < M_N_Interventions; i++)
			{
				if (find(M_Interventions[i].Get_Workload_Res().begin(), M_Interventions[i].Get_Workload_Res().end(), k) !=
					M_Interventions[i].Get_Workload_Res().end())

				for (int tprima = 0; tprima <= j; tprima++)
				{
					if (tprima >= M_Interventions[i].Get_Tmax()) continue;
					if ((tprima + M_Interventions[i].Get_Delta(tprima)) <= j) continue;
					if (tprima > j) continue;
					if (M_Interventions[i].Get_RiskC(tprima) > M_Max_Riesgo) continue;
					if (M_I_T[i] < 0 )
					vlower += x_ij[i][tprima] * M_Interventions[i].Get_WorkloadN(k,tprima,j);
					if (M_I_T[i] == tprima)
						vlower +=M_Interventions[i].Get_WorkloadN(k, tprima, j);
			
					/*					if (M_Interventions[i].Get_Workload()[k][j][tprima] > )
										{
											cout << x_ij[i][tprima].getName() << " " << k << " " << j << " " << tprima << " " << M_Interventions[i].Get_Workload()[k][j][tprima];
											cout << endl;
										}*/
				}
			}

//			restr.add(IloRange(env, M_Resources[k].Get_Lower()[j]- 9e-6, vlower, M_Resources[k].Get_Upper()[j] + 9e-6));
			restr.add(IloRange(env,0, vlower, M_Resources[k].Get_Upper()[j] + 9e-6));
			vlower.end();
		}
	}
	//Exclusions

	for (list<vector<int>> ::iterator it = M_Exclusions.begin(); it != M_Exclusions.end(); it++)
	{

		for (int j = 0; j < M_T; j++)
		{
			if (M_T_Season[j] != (*it)[2]) continue;
			IloExpr v1(env);
			bool algo1 = false;
			bool algo2 = false;
			for (auto t1 = 0; t1 < M_Interventions[(*it)[0]].Get_Tmax() && t1 <= j; t1++)
			{

				if ((t1 + M_Interventions[(*it)[0]].Get_Delta()[t1]) <= j) continue;
				if (M_Interventions[(*it)[0]].Get_RiskC(t1) > M_Max_Riesgo) continue;
				if (M_I_T[(*it)[0]] < 0)
				v1 += x_ij[(*it)[0]][t1];
				if (M_I_T[(*it)[0]] == t1)
					v1 += 1;
				algo1 = true;
			}
			for (auto t2 = 0; t2 < M_Interventions[(*it)[1]].Get_Tmax() && t2 <= j; t2++)
			{


				if ((t2 + M_Interventions[(*it)[1]].Get_Delta()[t2]) <= j) continue;
				if (M_Interventions[(*it)[1]].Get_RiskC(t2) > M_Max_Riesgo) continue;
				if (M_I_T[(*it)[1]] < 0)
				v1 += x_ij[(*it)[1]][t2];
				if ( M_I_T[(*it)[1]] == t2)
					v1 += 1;
				algo2 = true;

			}
			if (algo1 == true && algo2 == true)
			{
				restr.add(IloRange(env, -IloInfinity, v1, 1));
				v1.end();
			}


		}
	}

	if (M_Max_Formulation && M_Pool_Solutions)
	{
		for (int j = 0; j < M_T; j++)
		{

			for (int l = 0; l < M_Scenarios_number[j]; l++)
			{
				if (M_Scenarios_number[j] == 1) continue;
				IloExpr vlower(env);
				for (size_t i=0; i < M_N_Interventions; i++)
				{

					for (int tprima = 0; tprima <= j; tprima++)
					{
						if (tprima >= M_Interventions[i].Get_Tmax()) continue;
						if ((tprima + M_Interventions[i].Get_Delta()[tprima]) <= j) continue;
						if (tprima > j) continue;
						if (M_Interventions[i].Get_RiskC(tprima) > M_Max_Riesgo) continue;
						if (M_Interventions[i].Get_RiskN(j,tprima,l) > __FLT_EPSILON__)
						{
							if (M_I_T[i] < 0 || M_I_T[i] == tprima)
							vlower += x_ij[i][tprima] * M_Interventions[i].Get_RiskN(j, tprima, l);
						}
					}
				}
				vlower += (-1) * M_j[j];
				restr.add(IloRange(env, -IloInfinity, vlower, 0));
				vlower.end();
			}

		}

	}
	if (M_Min_Formulation && M_Pool_Solutions)
	{
		for (int j = 0; j < M_T; j++)
		{

			for (int l = 0; l < M_Scenarios_number[j]; l++)
			{
				if (M_Scenarios_number[j] == 1) continue;
				IloExpr vlower(env);
				for (size_t i=0; i < M_N_Interventions; i++)
				{

					for (int tprima = 0; tprima <= j; tprima++)
					{
						if (tprima >= M_Interventions[i].Get_Tmax()) continue;
						if ((tprima + M_Interventions[i].Get_Delta()[tprima]) <= j) continue;
						if (tprima > j) continue;
						if (M_Interventions[i].Get_RiskC(tprima) > M_Max_Riesgo) continue;
						if (M_Interventions[i].Get_RiskN(j, tprima, l) > __FLT_EPSILON__)
						{
							if (M_I_T[i] < 0 || M_I_T[i] == tprima)
								vlower += x_ij[i][tprima] * M_Interventions[i].Get_RiskN(j, tprima, l);
						}
					}
				}
				vlower += (-1) * M_j[j];
				restr.add(IloRange(env, 0, vlower, IloInfinity));
				vlower.end();
			}

		}

	}
	//Fijar variables

	//Restricciones
	//AÑADIMOS LAS RESTRICCIONES AL MODELO 
	model.add(restr);

	/**********************************************************/
	/***************FUNCION OBJETIVO **************************/
	/**********************************************************/
//	M_Alpha = 1;

	IloExpr Expression_fobj(env);
	for (size_t i = 0; i < M_N_Interventions && M_Reparar==false; i++)
	{
		for (int l = 0; l < M_T; l++)
		{

			if (M_Interventions[i].Get_RiskC(l) > M_Max_Riesgo)
				continue;
			if (M_I_T[i] >= 0) continue;
			if (l >= M_Interventions[i].Get_Tmax()) continue;
			//			Expression_fobj += x_ij[i][l]*M_Interventions[i].Get_RiskC(l)*M_Alpha;
			if (M_Mixed_Risk)
			{
				if (M_I_T[i] < 0 || M_I_T[i] == l)
					Expression_fobj += x_ij[i][l] * ((M_Interventions[i].Get_RiskC(l) * M_Alpha) + (M_Interventions[i].Get_RiskExcess(l) * (1 - M_Alpha)));
			}
			else
			{
				if (M_Formulacion_Cuantiles)
				{
					if (M_I_T[i] < 0 || M_I_T[i] == l)
						Expression_fobj += ((x_ij[i][l] * M_Interventions[i].Get_RiskQuantile(l) * (1 - M_Alpha)) + ((M_Alpha - (1 - M_Alpha)) * x_ij[i][l] * M_Interventions[i].Get_RiskC(l)));

				}
				else
				{
					if (M_I_T[i] < 0 || M_I_T[i] == l)
						Expression_fobj += x_ij[i][l] * M_Interventions[i].Get_RiskC(l) * M_Alpha;
				}

			}

		}
	}
	if (M_Max_Formulation && M_Pool_Solutions)
	{
		Val = 1;
		for (int j = 0; j < M_T; j++)
		{

			if (M_Scenarios_number[j] == 1) continue;
			Expression_fobj += M_j[j] * (1 - M_Alpha) * Val;
			if (M_Min_Formulation)
				Expression_fobj += M_j[j] * (1 - M_Alpha) * (-1);
			//			for (int l = 0; l < M_Scenarios_number[j]; l++)
			//			{
			for (size_t i=0; i < M_N_Interventions; i++)
			{
				if (M_Interventions[i].Get_RiskC(j) > M_Max_Riesgo) continue;
				if (j >= M_Interventions[i].Get_Tmax()) continue;
				if (M_I_T[i] < 0 || M_I_T[i] == j)
				Expression_fobj += x_ij[i][j] * M_Interventions[i].Get_RiskC(j) * (-1) * (1 - M_Alpha) * Val;
				/*
					//				if (j >= M_Interventions[i].Get_Tmax()) continue;
									for (int tprima = 0; tprima <= j; tprima++)
									{
										if (tprima >= M_Interventions[i].Get_Tmax()) continue;
										if ((tprima + M_Interventions[i].Get_Delta()[tprima]) <= j) continue;
										if (tprima > j) continue;
										double kk =(-1)*((double)(M_Interventions[i].Get_Risk()[j][tprima][l]) / ((double)M_Scenarios_number[j]));
										kk = kk * (1 - M_Alpha);
										if (kk < __FLT_EPSILON__)
										{
											if ((i == 1) && (tprima == 46))
											{
												kk2 += kk;
											}
											Expression_fobj += kk * x_ij[i][tprima];
										}
									}
									*/
									//				}
			}
		}
	}

	//Feasibility problem
	IloExpr Expression_fobj1(env);
	Expression_fobj1 += 10;

	if (!M_Reparar)
	model.add(IloMinimize(env, Expression_fobj));
//	Expression_fobj.end();
	//	printf("Aqui66\n");

		/***************************************************************/
		/*************** EJECUTAMOS EL MODELO **************************/
		/***************************************************************/
		double value2 = 0;
	try {
		//		printf("Aqui67\n");

		IloCplex cplex(model);
//		printf("Aqui687\n");

		//Si queremos las lazy y tenemos alguna
//		if (Lazy==true && stack_nbr<batch_items)

	//	cplex.addLazyConstraints(restrLazy);
		//No hace el probing
		//cplex.setParam(IloCplex::Probe, -1);
		//1 CORE
		cplex.setParam(IloCplex::Threads, 1);

/*		if (!M_Pool_Solutions)
		{
			cplex.setParam(IloCplex::EpGap, 0);
			cplex.setParam(IloCplex::EpAGap, 0);
		}
		else
		{*/
		cplex.setParam(IloCplex::IntSolLim, 1);
//		cplex.setParam(IloCplex::EpGap, 0.8);
		cplex.setParam(IloCplex::MIPEmphasis, 1);
//		cplex.setParam(IloCplex::MIPSearch, 2);
//		cplex.setParam(IloCplex::HeurFreq,1);
		cplex.setParam(IloCplex::FPHeur, 1);
		if (!M_Pool)
			cplex.setParam(IloCplex::EpGap, 0.0001);

	//	}
		//para que utilice memoria de disco para trasferir nodos de la mem principal a disco.
		/*cplex.setParam(IloCplex::NodeFileInd, 3);*/
		//limite de tiempo
		cplex.setParam(IloCplex::TiLim, M_Time_Modelo); //TIEMPO EN SEGUNDOS

//		cplex.exportModel("Prueba.lp");
		cplex.setOut(env.getNullStream());
//		if (M_Challenge_Mode) cplex.setOut(env.getNullStream());

		//CPXsetintparam(env,CPX_PARAM_THREADS,1);

		//numero de filas y columnas del modelo
//		int nr = cplex.getNrows();
//		int nc = cplex.getNcols();
		double best_fo = __FLT_MAX__;

		//resolver el modelo y tomamos tiempos
		IloNum inicio, fin;
		clock_t t_ini, t_fin;

		inicio = cplex.getCplexTime();
		t_ini = clock();
		//		printf("Aqui68");
//		IloBool result = false;
//		if (M_Pool==false)
		IloBool result = cplex.solve();
		t_fin = clock();
		fin = cplex.getCplexTime();

		double secs = (double)(fin - inicio);
		M_Time_Modelo1 = secs;
//		printf("%2f \n", secs);
//		cplex.setParam(IloCplex::TiLim, max(M_Time_Modelo + (M_Time_Modelo - secs),(double)1));

		t_fin = clock();
		fin = cplex.getCplexTime();

		secs = (double)(fin - inicio);
//		printf("%2f \n", secs);


		//		int minvol = (*contenedor).Get_X() * (*contenedor).Get_Y() * (*contenedor).Get_Z();
//		int candidato = 0;
		//		printf("Aqui666\n");

				//Estado del algoritmo	al terminar
		IloCplex::Status estado = cplex.getCplexStatus();
		IloCplex::Status sub_estado = cplex.getCplexSubStatus();
		if ((estado == 11 && result==false) || estado==3)
		{
			cplex.end();
			model.end();
			env.end();
			return -1;
		}
		if (result == false)
		{


			//			FILE* f = fopen("./estadomodelo.txt", "a+");
						//fprintf(f, "%s ", nom);
//			IloAlgorithm::Status est = cplex.getStatus();
			if (cplex.getStatus() == IloAlgorithm::Infeasible)
			{
				printf("Infeasible\n");
				//			cplex.exportModel("Roadef.lp");
			}
			else
			{
				if (cplex.getStatus() == IloAlgorithm::Unbounded) printf("Unbounded\n");
				//			printf("\n%s \tnosol;%.2f;%.2f;MaxTiras %d\t MaxPilas %d\t  Bins %d Tiras %d Pilas %d\n", name_instance.c_str(), cplex.getNnodes(), cplex.getTime(), maxTiras, maxPilas, numbins, numtiras, numpilas);
			}

		}
		else
		{
			//			FILE* f = fopen("./estadomodelo.txt", "a+");
			//			fprintf(f, "\n%s\t%.2f;%.2f;%.2f;MaxTiras %d\t MaxPilas %d\t Bins %d Tiras %d Pilas %d\n", name_instance.c_str(), cplex.getObjValue(), cplex.getBestObjValue(), cplex.getTime(), maxTiras, maxPilas, numbins, numtiras, numpilas);
//			printf("\n%s\t%.2f;%.2f;%.2f;MaxTiras %d\t MaxPilas %d\t Bins %d Tiras %d Pilas %d\n", name_instance.c_str(), cplex.getObjValue(), cplex.getBestObjValue(), cplex.getTime(), maxTiras, maxPilas, numbins, numtiras, numpilas);
			value2 = cplex.getObjValue();
//			cout << ((value2) / (double)M_T) << endl;
			M_sol_formulation = ((value2) / (double)M_T);
//			double LB_Best_Value_Formulation = cplex.getBestObjValue();
			//Si ha encontrado alguna solucion posible

			if (cplex.getSolnPoolNsolns() > 0)
			{

				//|bestnode-bestinteger|/(1e-10+|bestinteger|)
//				double relativeGap = abs(cplex.getObjValue() - cplex.getBestObjValue()) / (abs(cplex.getObjValue()) + 1e-10);

				//Aqui escribir la solucion en un fichero externo
	//Pinto las variables x_ijkl, xr_ijkl
//				printf("Aqui26\n");
				for (size_t i=0; i < M_N_Interventions; i++)
				{
					for (int l = 0; l < M_T; l++)
					{
						if(Temp_I_T[i] != l && Temp_I_T[i] >= 0)
							continue;
						if (M_Interventions[i].Get_RiskC(l) > M_Max_Riesgo) continue;
						if (M_I_T[i] >= 0) continue;
						if (cplex.isExtracted(x_ij[i][l]) && cplex.getValue(x_ij[i][l]) > 0.001)
						{
//							cout << M_Interventions[i].Get_Name() << " ";
//							printf("id %d time %d  \n", i, l);
							if (Temp_I_T[i] < 0)
							{
								M_I_T[i] = l;
								M_Interventions[i].Set_T_Temp(l);
								//				M_I_T_Solution.push_back(p);
								M_Interventions_Colocada[i] = true;
								M_NI_Colocadas++;
								AddScenarios(i);
								AddRecursos(i);
								M_fo_temp += M_Interventions[i].Get_RiskC(l);
								M_Min_fo_temp -=
								    M_Interventions[i]
								        .Get_Min_Risk();
								//								ColocarIEnTiempo(i, l);

							}
							//						fprintf(f, "%s %d w %d h %d s %d id %d\n", x_l[i].getName(), 1, Temp_Conjuntos[i].item_id, Temp_Conjuntos[i].stack_nbr, Temp_Conjuntos[i].number, i);
						}


					}
					//				printf("Aqui27\n");






				}
			}
			//			printf("Aqui288");
						//}
		}
//		printf("Aqui299");
		cplex.end();
		model.end();
		env.end();
//		M_Pool_Solutions_Integer.push_back(M_I_T);
		double kk = FuncionObjetivoTemp();
		VerifySolution();

		M_fo_formulation1 = kk;
//		EjecutarValidadorPython(M_I_T_Solution_Best);
		if (kk < best_fo)
			best_fo = kk;
		M_fo_formulation2 = best_fo;

		return best_fo;
	}
	catch (IloException& e)
	{
		printf("EXCEPCION");
		cerr << "IloException: " << e << "Status" << e;
	}
//	printf("Aqui666\n");
	return 0;
}

