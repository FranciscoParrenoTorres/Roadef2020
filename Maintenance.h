#ifndef MAINTENANCE_H
#define MAINTENANCE_H

#include "definitions.h"


using namespace std;

extern int Global_Best_Objective_Function;
extern int Param;
#ifdef _WIN32 
extern  int DibujarOpenGL();
#define __FLT_EPSILON__ FLT_EPSILON
#define __LONG_MAX__ LONG_MAX
#define __FLT_MAX__ FLT_MAX
#define __DBL_MAX__ DBL_MAX
#endif

class Item_orden;
class MaintenancePlate;
 class MaintenanceRectangle;
 class MaintenanceStack;
 class MaintenanceItem;
class MaintenanceDefect;
class Maintenance_Corte;


typedef pair <int, int> ParInt;

typedef  list< MaintenanceDefect > LMaintenanceDefect;

typedef pair< pair< int, int >, pair< int, int > >  Coordinates;

typedef list< MaintenanceRectangle > LRectangles;

using namespace std;

enum Seasons_Name {full=0, winter=1,summer=2,is=3 };
class Resource
{

	string R_Name;
	int R_Id;
	vector<double> R_Lower;
	vector<double> R_Upper;
	vector<double> R_Temp;
	vector<double> R_Copy;
	vector<double> R_Copy2;
	vector<double> R_Copy3;
	vector<double> R_Best;
public:
	Resource() {
		R_Id = -1;
	};
	Resource(int a)
	{
		R_Id = a;
	};
	virtual ~Resource() {};
	//Set
	void Set_Id(int a) {  R_Id = a; };
	void Set_Name(string a) { R_Name = a; };
	void Set_Lower(vector<double>& a) { R_Lower = a; };
	void Set_Upper(vector<double>& a) { R_Upper = a; };
	void Set_Temp(vector<double>& a) { R_Temp = a; };
	void Set_Copy(vector<double>& a) { R_Copy = a; R_Copy2 = a; R_Copy3 = a; R_Best = a; };
	//Get
	int Get_Id(void) { return R_Id; };
	string  Get_Name(void) { return R_Name; };
	vector<double>& Get_Lower(void) { return R_Lower; };
	vector<double>& Get_Upper(void) { return R_Upper; };
	vector<double>& Get_Temp(void) { return R_Temp; };
	inline double Get_Lower(int a) { return R_Lower[a]; };
	inline double Get_Upper(int a) { return R_Upper[a]; };
	inline double Get_Temp(int a) { return R_Temp[a]; };
	inline void Add(double a, int t) { 
		R_Temp[t]= R_Temp[t]+a; 
	};
	inline void Subtract(double a, int t) {
		R_Temp[t] = R_Temp[t] - a;
	};
	inline void Clean()
	{
		for (size_t i = 0; i < R_Temp.size(); i++)
			R_Temp[i] = 0;

	}
	inline void Copy()
	{
		for (size_t i = 0; i < R_Temp.size(); i++)
			R_Copy[i] = R_Temp[i];

	}
	inline void CopyBack()
	{
		for (size_t i = 0; i < R_Temp.size(); i++)
			R_Temp[i] = R_Copy[i];

	}
	inline void Copy2()
	{
		for (size_t i = 0; i < R_Temp.size(); i++)
			R_Copy2[i] = R_Temp[i];

	}
	inline void CopyBack2()
	{
		for (size_t i = 0; i < R_Temp.size(); i++)
			R_Temp[i] = R_Copy2[i];

	}
	inline void Copy3()
	{
		for (size_t i = 0; i < R_Temp.size(); i++)
			R_Copy3[i] = R_Temp[i];

	}
	inline void CopyBack3()
	{
		for (size_t i = 0; i < R_Temp.size(); i++)
			R_Temp[i] = R_Copy3[i];

	}
	inline void CopyBest()
	{
		for (size_t i = 0; i < R_Temp.size(); i++)
			R_Best[i] = R_Temp[i];

	}
	inline void CopyBackBest()
	{
		for (size_t i = 0; i < R_Temp.size(); i++)
			R_Temp[i] = R_Best[i];

	}

};
class Season
{
	int S_Id;
	string S_Name;
	vector<int> S_Numbers;
public:
	Season() { S_Id = -1; };
	Season(int a)
	{
		S_Id = a;
	};
	virtual ~Season() {};
	//Set
	void Set_Id(int a) {  S_Id = a; };
	void Set_Name(string a) { S_Name=a; };
	void Set_Numbers(vector<int>& a) { S_Numbers = a; };
	//Get
	int Get_Id(void) { return S_Id; };
	string  Get_Name(void) { return S_Name; };
	vector<int>& Get_Numbers(void) { return S_Numbers; };



};
class Nodo
{
	int N_id;
	int N_level;
//	int N_Id_Intervention;
	vector<int> N_T;//vector con las piezas colocadas
//	int N_T;
	vector<vector<double>> N_T_Temp_Risk_S;
	list<int> N_T_Ordenados_Risk;
	double N_fo_temp;
	vector<double> N_vfo2_temp;
	vector< int> N_I_Seasons;
public:
	Nodo(int a  )
	{
		N_id = a;
//		N_Id_Intervention = inter;
//		N_T = t;
	};
	virtual ~Nodo() {};
};
class Intervention
{

	int I_Id;
	int I_tmax;
	int I_T_Temp;
	int I_T_F_Temp;
	double I_Min_WorkloadxT;
	double I_Min_Workload;
	double I_Min_Risk;
	double I_Min_RiskQ;
	double I_Min_Risk_Temp;
	double I_Min_RiskQ_Temp;
	double I_Min_RiskQ_Temp_Copy;
	double I_Min_RiskQ_Temp_Copy2;
	double I_Min_RiskQ_Temp_Copy3;
	double I_Min_Risk_Temp_Copy;
	double I_Min_Risk_Temp_Copy2;
	double I_Min_Risk_Temp_Copy3;
	double I_Min_Risk_Temp_Best;
	double I_Min_RiskxT;

	double I_Max_Workload;
	double I_Max_WorkloadxT;
	double I_Max_Risk;
	double I_Max_RiskQ;

	double I_Max_RiskxT;
	double I_Regret;
	double I_RegretQ;
	double I_Regret_Temp;
	double I_RegretQ_Temp;
	double I_Regret_Temp_Copy;
	double I_RegretQ_Temp_Copy;
	double I_RegretQ_Temp_Copy2;
	double I_RegretQ_Temp_Copy3;
	double I_Regret_Temp_Copy2;
	double I_Regret_Temp_Copy3;
	double I_Regret_Temp_Best;
	double I_Excess_temp;
	vector<int> I_Workload_Res;
	vector<vector<double>> I_T_T_RiskExcess;
	string I_Name;
	vector<int> I_Delta;
	vector<int> I_T_ordenados_Risk;
	vector<int> I_T_ordenados_RiskQ;
	list<int> I_T_ordenados_Risk_Temp;
	list<int> I_T_ordenados_RiskQ_Temp;
	list<int> I_T_ordenados_Risk_Temp_Copy;
	list<int> I_T_ordenados_RiskQ_Temp_Copy;
	list<int> I_T_ordenados_RiskQ_Temp_Copy2;
	list<int> I_T_ordenados_RiskQ_Temp_Copy3;
	list<int> I_T_ordenados_Risk_Temp_Copy2;
	list<int> I_T_ordenados_Risk_Temp_Copy3;
	list<int> I_T_ordenados_Risk_Temp_Best;
	list<pair<int, int> > I_Exclusions;
	list<int> I_T_ordenados_Risk_I;
	list<int> I_T_ordenados_Risk_IQ;
	int I_Min_Delta;
//	vector<int,int> I_Workload_R;
	vector<double > I_WorkloadC;
	vector<double > I_WorkloadCxT;
	map<int,map<int,map<int, double >>> I_Workload;
	vector<vector<vector<double>>> I_Workload_N;
	 map<int, map<int,vector< double > >> I_Risk;
	vector<vector<vector<double>>> I_RiskN;
	vector<double> I_T_RiskC;
	vector<double> I_T_RiskCQuantile;
	vector<double> I_T_RiskCxT;
	vector<double> I_T_RiskExcess;
	double I_WC_Temp = 0.0;


//	vector<vector < vector <int> > > I_Workload;
//	vector<vector < vector <int> > > I_Risk;
//	vector<int> I_T_RiskC;
public:
	Intervention() {
		I_Id=(-1);
		I_tmax= (-1);
		I_Min_Workload= (-1);
		I_Min_Risk = (-1.0);
		I_Max_Workload = (-1.0);
		I_Max_Risk = (-1.0);
		I_Min_Delta= (-1);
		I_T_Temp = (-1);
		I_T_F_Temp = (-1);
		I_Exclusions.clear();
		I_Min_RiskQ_Temp = 0.0;
		I_Min_RiskQ_Temp_Copy = 0.0;

		I_Regret = 0.0;
		I_Max_Risk = 0.0;
		I_Max_RiskQ = 0.0;
		I_Excess_temp = 0.0;


	};
	Intervention(int a)
	{
		I_Id = a;
	};

	virtual ~Intervention() {};
	//Set
	void Set_Id(int a) { I_Id = a; };
	void Set_Tmax(int a) { I_tmax = a; };
	void Set_Min_Risk(double a) { I_Min_Risk = a; I_Min_Risk_Temp = a;  };
	void Set_Min_RiskQ(double a) { I_Min_RiskQ = a; I_Min_RiskQ_Temp = a; };
	void Set_Min_Risk_Temp(double a) {  I_Min_Risk_Temp = a; };
	void Set_Min_RiskQ_Temp(double a) { I_Min_RiskQ_Temp = a; };
	void Set_Min_RiskxT(double a) { I_Min_RiskxT = a; };
	void Set_Min_Workload(double a) { I_Min_Workload = a; I_WC_Temp = a; };
	void Set_Min_WorkloadxT(double a) { I_Min_WorkloadxT = a; };
	void Set_Max_Risk(double a) { I_Max_Risk = a; };
	void Set_Max_RiskQ(double a) { I_Max_RiskQ = a; };
	void Set_Max_RiskxT(double a) { I_Max_RiskxT = a; };
	void Set_Regret(double a) { I_Regret = a; I_Regret_Temp = a;};
	void Set_Regret_Temp(double a) { I_Regret_Temp = a; };
	void Set_RegretQ(double a) { I_RegretQ = a; I_RegretQ_Temp = a; };
	void Set_RegretQ_Temp(double a) { I_RegretQ_Temp = a; };
	void Set_Max_Workload(double a) { I_Max_Workload = a; };
	void Set_Max_WorkloadxT(double a) { I_Max_WorkloadxT = a; };
	void Set_Delta(vector<int> &a) { I_Delta = a; };
	void Set_Min_Delta(int a) { I_Min_Delta = a; };
	void Set_Name(string a) { I_Name = a; };
	void Set_Excess_temp(double a) { I_Excess_temp = a; };

	void Set_T_Temp(int a) { 
		I_T_Temp = a; 
		I_T_F_Temp = (I_T_Temp + I_Delta[I_T_Temp]); };
	void Set_Workload(map<int, map<int, map<int, double >> > & a) { I_Workload = a; };
	void Set_Workload_Res(vector<int> & a) { I_Workload_Res = a; };
	void Set_WorkloadN(vector<vector<vector<double>>>& a) { I_Workload_N = a; };
//	void Set_WorkloadR(map<int, int>& a) { I_Workload_R = a; };
	void Set_WorkloadC(vector<double >& a) { I_WorkloadC = a; };
	void Set_WorkloadCxT(vector<double >& a) { I_WorkloadCxT = a; };
	void Set_Risk( map<int, map<int, vector<double >>>& a) { I_Risk = a; };
	void Set_RiskN(vector<vector<vector<double >>>& a) { I_RiskN = a; };
	void Set_RiskC(vector<double>& a) { I_T_RiskC = a; };
	void Set_RiskExcess(vector<double>& a) { I_T_RiskExcess = a; };
	void Set_RiskExcessXt(vector<vector<double>>& a) { I_T_T_RiskExcess = a; };
	void Set_RiskCQuantile(vector<double>& a) { I_T_RiskCQuantile = a; };
	void Set_RiskCxT(vector<double>& a) { I_T_RiskCxT = a; };
	void Set_T_ordenados_Risk(vector<int>& a) 
	{ 
		I_T_ordenados_Risk = a; 
		for (int k=0;k<a.size();k++)
			I_T_ordenados_Risk_Temp.push_back(a[k]);
	};
	void Set_T_ordenados_Risk_I(list<int>& a) { I_T_ordenados_Risk_I = a; };
	void Set_T_ordenados_RiskQ(vector<int>& a)
	{
		I_T_ordenados_RiskQ = a;
		for (int k = 0; k < a.size(); k++)
			I_T_ordenados_RiskQ_Temp.push_back(a[k]);
	};
	void Set_T_ordenados_Risk_IQ(list<int>& a) { I_T_ordenados_Risk_IQ = a; };
	void Set_Exclusions(list<pair<int, int> >& a) { I_Exclusions = a; };
	//Get
	int Get_Id(void) { return I_Id; };
	int Get_Tmax(void) { return I_tmax; };
	int Get_T_Temp(void) { return I_T_Temp; };
	double Get_Excess_temp(void) { return I_Excess_temp; };
	double Get_Regret(void) { return I_Regret; };
	double Get_Regret_Temp(void) { return I_Regret_Temp; };
	double Get_RegretQ(void) { return I_RegretQ; };
	double Get_RegretQ_Temp(void) { return I_RegretQ_Temp; };
	int Get_T_F_Temp(void) { return I_T_F_Temp; };
	double Get_Min_Workload(void) { return I_Min_Workload; };
	void Set_Min_Workload_Temp(double a) { I_WC_Temp = a; };
	double Get_Min_Risk(void) { return I_Min_Risk; };
	double Get_Min_RiskQ(void) { return I_Min_RiskQ; };
	double Get_Min_RiskxT(void) { return I_Min_RiskxT; };
	double Get_Min_WorkloadxT(void) { return I_Min_WorkloadxT; };
	double Get_Max_Dif_Risk(void) { return (I_Max_Risk - I_Min_Risk); };
	double Get_Max_Workload(void) { return I_Max_Workload; };
	double Get_Max_Risk(void) { return I_Max_Risk; };
	double Get_Min_Risk_Temp(void) { return I_Min_Risk_Temp; };
	double Get_Min_RiskQ_Temp(void) { return I_Min_RiskQ_Temp; };
	vector<int>& Get_Delta(void) { return I_Delta; };
	int  Get_Delta(int a) { return I_Delta[a]; };
	int Get_Min_Delta(void) { return I_Min_Delta; };
	inline double Get_Riskexcess(int tinicio, int tactual)
	{
		if (tinicio <= tactual)
			return I_T_T_RiskExcess[tinicio][tactual - tinicio];
//		return I_T_T_RiskExcess[tinicio][tactual - tinicio];
		return 0;
	}

	inline double Get_Risk(int tactual, int tinicio, int nEscenario)
	{
		if (tinicio<=tactual)
			return I_Risk[tactual][tinicio][nEscenario];
//			return I_Risk[tactual][tinicio][nEscenario];
		for (int i = 0;; i++)
			cout << "Problema";
		return 0;

	};
	inline int position(int tactual, int tinicio)
	{
		return ( I_RiskN[tactual].size() - 1) - ( min(tactual, I_tmax - 1) - tinicio);
	}
	inline bool EstaTs(vector<int> &lista)
	{
		for (register int i = 0; i < lista.size(); i++)
		{
			if (I_T_Temp <= lista[i] && I_T_F_Temp > lista[i]) 
				return true;

		}
		return false;
	}
	inline bool EstaT(int t)
	{

			if (I_T_Temp <= t && I_T_F_Temp > t)
				return true;

		return false;
	}
	inline bool QuiereT(int t,int cuantos)
	{
		for (int k = 0; k < cuantos; k++)
		{
			if (I_T_ordenados_Risk[k] == t)
				return true;
		}

		return false;
	}
	inline double Get_RiskN(int tactual, int tinicio, int nEscenario)
	{

//		int pos = position(tactual, tinicio);
		if (tinicio <= tactual)
			//return I_RiskN[tinicio][tactual-tinicio][nEscenario];
			return I_RiskN[tinicio][tactual - tinicio][nEscenario];
		for (int i = 0;; i++)
			cout << "Problema";
		return 0;

	};
	inline double Get_WorkloadN(int resource_id, int tinicio, int tactual)
	{
//		if (I_Workload_N[resource_id].size() < 1) 
//			return 0;
		return I_Workload_N[resource_id][tinicio][tactual-tinicio];
	}
	inline double Get_Workload(int resource_id, int tactual, int tinicio)
	{
		return I_Workload[resource_id][tactual][tinicio];
	}
	vector<double> &Get_RiskC(void)
	{
		return I_T_RiskC;
	}
	double  Get_RiskC(int a)
	{
		return I_T_RiskC[a];
	}
	double  Get_RiskExcess(int a)
	{
		return I_T_RiskExcess[a];
	}
	double  Get_RiskQuantile(int a)
	{
		return I_T_RiskCQuantile[a];
	}
	double Get_RiskC(int tactual, int tinicio)
	{ 
		if (tinicio<tactual)
		return I_Risk[tactual][tinicio][I_Risk[tactual][tinicio].size()];
		
	};

	string  Get_Name(void) { return I_Name; };
	vector<int>& Get_T_Ordenados_Risk() { return I_T_ordenados_Risk; };
	list<int>& Get_T_Ordenados_Risk_Temp() { return I_T_ordenados_Risk_Temp; };
	vector<int>& Get_T_Ordenados_RiskQ() { return I_T_ordenados_RiskQ; };
	list<int>& Get_T_Ordenados_RiskQ_Temp() { return I_T_ordenados_RiskQ_Temp; };
	double Get_WorkloadC_Temp() { return I_WC_Temp; };

	list<int>& Get_T_Ordenados_Risk_I() { return I_T_ordenados_Risk_I; };
	vector<int> & Get_Workload_Res(void) { return I_Workload_Res; };
	map<int, map<int, map<int, double >>>& Get_Workload(void) { return I_Workload; };
	 map<int, map<int, vector< double > >>& Get_Risk(void) { return I_Risk; };
//	map<int, int> & Get_WorkloadR(void) { return I_Workload_R; };
	vector <double > & Get_WorkloadC(void) { return I_WorkloadC; };
	list<pair<int, int> >& Get_Exclusions(void) { return I_Exclusions; };
	double  Get_WorkloadC(int a) { return I_WorkloadC[a]; };
	void Add_Exclusion(int i, int S)
	{
		I_Exclusions.push_back(pair<int, int>(i, S));
	};
	void Copy(bool lista_temp)
	{
		if (lista_temp)
		{
			I_T_ordenados_Risk_Temp_Copy = I_T_ordenados_Risk_Temp;
			I_T_ordenados_RiskQ_Temp_Copy = I_T_ordenados_RiskQ_Temp;
		}
		I_Regret_Temp_Copy = I_Regret_Temp;
		I_Min_Risk_Temp_Copy = I_Min_Risk_Temp;

		I_RegretQ_Temp_Copy = I_RegretQ_Temp;
		I_Min_RiskQ_Temp_Copy = I_Min_RiskQ_Temp;
	};
	void CopyBack(bool lista_temp)
	{
		if (lista_temp)
		{
			I_T_ordenados_Risk_Temp = I_T_ordenados_Risk_Temp_Copy;
			I_T_ordenados_RiskQ_Temp = I_T_ordenados_RiskQ_Temp_Copy;
		}
		I_Regret_Temp = I_Regret_Temp_Copy;
		I_Min_Risk_Temp = I_Min_Risk_Temp_Copy;

		I_RegretQ_Temp = I_RegretQ_Temp_Copy;
		I_Min_RiskQ_Temp = I_Min_RiskQ_Temp_Copy;

	};

	void Copy2(bool lista_temp)
	{
		if (lista_temp)
		{
			I_T_ordenados_Risk_Temp_Copy2 = I_T_ordenados_Risk_Temp;
			I_T_ordenados_RiskQ_Temp_Copy2 = I_T_ordenados_RiskQ_Temp;
		}
		I_Regret_Temp_Copy2 = I_Regret_Temp;
		I_Min_Risk_Temp_Copy2 = I_Min_Risk_Temp;

		I_RegretQ_Temp_Copy2 = I_RegretQ_Temp;
		I_Min_RiskQ_Temp_Copy2 = I_Min_RiskQ_Temp;
	};
	void CopyBack2(bool lista_temp)
	{
		if (lista_temp)
		{
			I_T_ordenados_Risk_Temp = I_T_ordenados_Risk_Temp_Copy2;
			I_T_ordenados_RiskQ_Temp = I_T_ordenados_RiskQ_Temp_Copy2;
		}
		I_Regret_Temp = I_Regret_Temp_Copy2;
		I_Min_Risk_Temp = I_Min_Risk_Temp_Copy2;

		I_RegretQ_Temp = I_RegretQ_Temp_Copy2;
		I_Min_RiskQ_Temp = I_Min_RiskQ_Temp_Copy2;

	};
	void Copy3(bool lista_temp)
	{
		if (lista_temp)
		{
			I_T_ordenados_Risk_Temp_Copy3 = I_T_ordenados_Risk_Temp;
			I_T_ordenados_RiskQ_Temp_Copy3 = I_T_ordenados_RiskQ_Temp;
		}
		I_Regret_Temp_Copy3 = I_Regret_Temp;
		I_Min_Risk_Temp_Copy3 = I_Min_Risk_Temp;

		I_RegretQ_Temp_Copy3 = I_RegretQ_Temp;
		I_Min_RiskQ_Temp_Copy3 = I_Min_RiskQ_Temp;
	};
	void CopyBack3(bool lista_temp)
	{
		if (lista_temp)
		{
			I_T_ordenados_Risk_Temp = I_T_ordenados_Risk_Temp_Copy3;
			I_T_ordenados_RiskQ_Temp = I_T_ordenados_RiskQ_Temp_Copy3;
		}
		I_Regret_Temp = I_Regret_Temp_Copy3;
		I_Min_Risk_Temp = I_Min_Risk_Temp_Copy3;

		I_RegretQ_Temp = I_RegretQ_Temp_Copy3;
		I_Min_RiskQ_Temp = I_Min_RiskQ_Temp_Copy3;

	};
	void CopyBest()
	{
		I_T_ordenados_Risk_Temp_Best = I_T_ordenados_Risk_Temp;
		I_Regret_Temp_Best = I_Regret_Temp;
		I_Min_Risk_Temp_Best = I_Min_Risk_Temp;
	};
	void CopyBackBest()
	{
		I_T_ordenados_Risk_Temp = I_T_ordenados_Risk_Temp_Best;
		I_Regret_Temp = I_Regret_Temp_Best;
		I_Min_Risk_Temp = I_Min_Risk_Temp_Best;


	};
	/*	bool  operator <(const Item_orden& p2) const
		{
			if (altura < p2.altura)
				return false;
			else
				return true;
		}
		*/

		//bool  operator simple(const Item_orden& p2) const
		//{
		//	if (min(altura,anchura) < min(p2.altura,p2.anchura))
		//		return false;
		//	else
		//		return true;
		//}
	bool MinEntre(int ini, int fin);
	void Original(bool);

};



//bool PIECE::operator <(const PIECE& p2) const



class Maintenance
{
	int M_Hilo;
	int M_Id;
	int M_T;
	int M_seed;
	map<string, int> M_I_Id;
	map<string, int> M_R_Id;
	map<string, int> M_S_Id;
	double M_Delta;
	double M_Quantile;
	double M_Alpha;
	int M_Tipo;
	int M_ComputionTime;
	int M_Total_Time;
	double M_Cota;
	double M_Best_fo;
	double M_fo_temp;
	double M_fo2_temp;
	vector<double> M_vfo2_temp;
	vector<double> M_vfo2_temp_Copy;

	vector<double> M_vfo2_temp_Copy2;
	vector<double> M_vfo2_temp_Copy3;

	vector<int> M_Ivfo2_temp;
	double M_fo_temp_Copy;
	double M_fo_temp_Copy2;
	double M_fo_temp_Copy3;
	double M_fo_temp_Best;
	vector<Resource> M_Resources;
	vector<Intervention> M_Interventions;
	list<pair<vector<int>, double>> M_Pool_Solutions_Integer;
	list<pair<vector<int>, double>> M_Pool_Solutions_IntegerQuantiles;
	list<pair<vector<int>, double>> M_Pool_Solutions_IntegerMax;
	vector<list<pair<vector<int>, double>>> M_Pool_Solutions_Lista;
	double M_Best_Integer_For;
	double M_Best_Integer_ForQ;
	double M_Best_Integer_ForMax;

	vector<double> M_T_M1_Suma;
	vector<double> M_T_M2_Suma;
	vector<double> M_T_M3_Suma;
	vector<double> M_T_M4_Suma;
	vector<bool> M_T_M1;
	vector<bool> M_T_M2;
	vector<bool> M_T_M3;
	vector<bool> M_T_M4;
	vector<double> M_BestT_Suma;
	bool M_Reparar;

	list<vector<int>> M_Pool_Solutions_lista1;
	list<vector<int>> M_Pool_Solutions_lista2;
	vector<bool> M_Interventions_Colocada;//Intervencions colocadas
	vector<bool> M_Interventions_Colocada_Copy;//Intervencions colocadas copia
	vector<bool> M_Interventions_Colocada_Copy2;//Intervencions colocadas copia
	vector<bool> M_Interventions_Colocada_Copy3;//Intervencions colocadas copia
	vector<bool> M_Interventions_Colocada_Best;//Intervencions colocadas copia
	vector< Season > M_Seasons;
	vector<int> M_Scenarios_number;
	vector<list<double>> M_T_Temp_Risk;
	vector<vector<double>> M_T_Temp_Risk_S;
	vector<vector<double>> M_T_Temp_Risk_FO;
	vector<double > M_T_Temp_Risk_FO_Suma;
	vector<vector<double>> M_T_Temp_Risk_S2;
	vector<vector<double>> M_T_Temp_Risk_S_Copy;
	vector<vector<double>> M_T_Temp_Risk_S_Copy2;
	vector<vector<double>> M_T_Temp_Risk_S_Copy3;
	vector<pair<int, double >> M_Interventions_MinWxT;
	vector<pair<int, double >> M_Interventions_MinRxT;
	vector<pair<int, double >> M_Interventions_MinW;
	vector<pair<int, double >> M_Interventions_MinR;
	vector<pair<int, double >> M_Interventions_MaxDifR;
	vector<pair<int, double >> M_Interventions_MaxDifW;
	//Ranking de Workload y Riesgo
	vector<pair<int, double >> M_Interventions_Min_WXR;
	//Ranking de Workload y Riesto por T
	vector<pair<int, double >> M_Interventions_Min_WtXRt;
	vector<int> M_Interventions_Regret;
	vector<pair<int, double>> M_Interventions_Temp;
	vector<pair<int, double>> M_Interventions_Exclusions;
	list<int> M_Interventions_MinWxT_I;
	list<int> M_Interventions_MinRxT_I;
	list<int> M_Interventions_MinW_I;
	list<int> M_Interventions_MinR_I;
	list<int> M_Interventions_Min_WXR_I;
	list<int> M_Interventions_Min_WtXRt_I;
	list<int> M_Interventions_MaxDifR_I;
	list<int> M_Interventions_MaxDifW_I;
	list<int> M_Interventions_Temp_I;
	list<int> M_Interventions_Exclusions_I;
	vector<int> M_Interventions_Risk_Temp;
	list< vector< int> > M_Exclusions;
	vector< int> M_I_Seasons; //vector con temporadas en las que está colocada una intervention, en otro caso -1
	vector< int> M_I_Seasons_Copy; //vector con temporadas en las que está colocada una intervention, en otro caso -1
	vector< int> M_I_Seasons_Copy2; //vector con temporadas en las que está colocada una intervention, en otro caso -1
	vector< int> M_I_Seasons_Copy3; //vector con temporadas en las que está colocada una intervention, en otro caso -1
	vector< int> M_I_Seasons_Best; //vector con temporadas en las que está colocada una intervention, en otro caso -1

	list< vector< string > > M_Exclusions_Originals;
	int M_N_Resources = 0;
	int M_N_Interventions = 0;
	int M_N_Exclusions = 0;
	timeb M_Time_Initial;


	vector<int> M_T_Season;
	//	vector< set<int> > M_T_I; //lista de intervenciones colocadas por el tiempo en el que están

	vector<string> M_R_Name;
	vector<string> M_S_Name;
	vector<string> M_I_Name;
	vector<int> M_I_T;
	vector<int> M_I_T_For;
	vector<int> M_I_T_ForQ;
	vector<int> M_Best_I_T;
	vector<int> M_I_T_Copy;
	vector<int> M_I_T_Copy2;
	vector<int> M_I_T_Copy3;
	list<pair<int, int>> M_I_T_Solution;
	list<pair<int, int>> M_I_T_Solution_Copy;
	list<pair<int, int>> M_I_T_Solution_Copy2;
	list<pair<int, int>> M_I_T_Solution_Copy3;
	list<pair<int, int>> M_I_T_Solution_Best;
	list<pair<int, int>> M_Best_I_T_Solution;
	vector<int> M_Interventions_T;
	vector<int> M_Interventions_T_Copy;
	vector<int> M_Interventions_T_Copy2;
	vector<int> M_Interventions_T_Copy3;
	vector<int> M_Interventions_T_Best;
	vector<int> M_Temp_Item;
	vector<double> M_Temp_Weight;
	//PAra los Algoritmos
	double M_Max_Riesgo;
	bool M_Mixed_Risk;
	int M_Random_Constructive; //0 es determinista //1 aleatorizado
	int M_Iterations;
	int M_Iter;
	int M_Best_Iter;
	vector<bool> M_T_Temp;
	vector<int> M_Vec_Alea_T;
	vector<int> M_Vec_Alea_I;
	//Porcentaje Ocupado para determinista
	double M_fo2_temp_Copy;
	double M_fo2_temp_Copy2;
	double M_Porcentaje_Ocupado;
	double M_Porcentaje_Ocupado2;
	bool M_Silent = true;
	int M_Total_Scenarios;
	bool M_ConstruirConRecursos;
	vector<int> M_Interventions_Out;
	vector<int> M_Interventions_Out_Ordenada;
	//Booleano que me dice si estoy en la fase de intensification
	bool M_Fase_Intensification;
	bool M_T_Ordenado;
	double M_fo_first;
	string M_Name_file;
	double M_FC_Umbral;
	bool M_Determinista;
	vector<int> M_Veces_Tipo;
	vector<double> M_FO_Tipo;
	vector<int> M_Sin_Terminar_Tipo;
	bool M_Max_Formulation;
	bool M_Min_Formulation;

	bool M_Pool;
	double M_fo_formulation1 = 0.0;
	double M_fo_formulation2 = 0.0;
	double M_fo_formulation3 = 0.0;
	double M_sol_formulation = 0.0;
	double M_Time_Modelo1 = 0.0;
	double M_Time_Modelo2 = 0.0;
	double M_Time_Modelo3 = 0.0;
	int M_Time_Modelo;
	vector<double> M_Cotas_Variables;
	int M_ComputationTime_Grasp;
	double M_Max_Min_Riesgo;

	//
	list<Nodo> M_ListaNodosBeam;
	list<Nodo> M_ListaNodosBeam_children;
	int M_iter_Beam;
	int M_level;
	int M_children;
	int M_Max_List_Nodos_Beam;
	double M_Waste;
	double M_Waste_Copy;
	double M_Waste_Copy2;
	double M_Waste_Copy3;
	double M_Perdida_Parcial;
	vector<int> M_Interventions_Dif;
	int M_Mejora1;
	int M_Mejora2;
	int M_Mejora3;
	int M_Mejora4;
	int M_Mejora5;
	int M_VMejora1;
	int M_VMejora2;
	int M_VMejora3;
	int M_VMejora4;
	int M_VMejora5;
	bool M_VND;
	bool M_Pool_Solutions;
	bool M_Formulacion_Cuantiles;
	double M_Min_Improve;
	bool M_Busqueda_Local;
	double M_Best_fo_1;//Primera parte función objetivo
	double M_Best_fo_2;//Segunda parte función objetivo
	//0 es ninguna
	//1 es VND
	//2 es la quitar
	//3 es mejorT
	//4 swap 
	//5 ejection chain
	//6 Integer
	int M_Tipo_Busqueda;
	int M_Time_Optimo;
	int M_Time_Populate;
	set<int> M_Integer_Sol_Distintas;
	set<int> M_No_Visitadas;
	int M_NThreads;
	int M_GapPool;
	list<pair<vector<int>, double>> M_PoolGoodSolutions;
	double M_MinPoolSolutions;
	int M_SizePoolSolutions;
	set<int> M_PoolGoodSolutionsD;
	bool M_Intensification;
	int M_Time_Intensification;
	double M_Time_Leer;
	bool M_Challenge_Mode;
	int M_NI_Colocadas;
	int M_NI_Colocadas_Copy3;
	int M_NI_Colocadas_Copy2;
	int M_NI_Colocadas_Copy;
	bool M_Lower_Resources;
	vector<bool> M_Lower_ResourcesV;
	size_t M_Number_Threads;
	bool M_Encuentra_Solutions;
	int M_Iter_Grasp;
	int M_No_Sol = 0;
	int M_Lugar;
	vector<int> M_T_Excess;
	set<int> M_distintos;
	vector<double> M_Cuanto_Mejora;
	double M_Mediana_Mejora;
	set<int> M_distintosM;
	//Iter de la formulation
	int M_Iter_Formulation;

      public:
	Maintenance() {};

	Maintenance(int a)
	{
		M_Iter_Formulation = 1;
		M_Hilo = 0;
		M_NThreads = 0;
		M_Min_Improve = 0.1;
		M_Silent = true;
		M_T = 0;
		M_Time_Leer = 0;
		M_Quantile = 0.0;
		M_Iter_Grasp = 0;
		M_Id = a;
		M_I_Name.clear();
		M_Tipo = 0;
		M_seed = 0;
		M_Best_fo = __LONG_MAX__;
		M_ConstruirConRecursos = true;
		M_T_Ordenado = false;
		M_Pool = false;
		M_Time_Modelo = 0;
		M_Max_Formulation = false;
		M_Min_Formulation = false;
		M_Lower_Resources = false;
		M_Fase_Intensification = false;
		M_Perdida_Parcial = 0;
		M_Best_Integer_For = 0;
		M_Best_Integer_ForQ = 0;
		M_Best_Integer_ForMax = 0;

		M_Mejora1 = 0;
		M_Mejora2 = 0;
		M_Mejora3 = 0;
		M_Mejora4 = 0;
		M_Mejora5 = 0;
		M_VMejora1 = 0;
		M_VMejora2 = 0;
		M_VMejora3 = 0;
		M_VMejora4 = 0;
		M_VMejora5 = 0;
		M_VND = false;
		M_Mixed_Risk = false;
		M_Formulacion_Cuantiles = false;
		M_Iter = 0;
		M_Time_Optimo = 0;
		M_Time_Populate = 0;
		M_GapPool = 100;
		M_SizePoolSolutions = 20;
		M_MinPoolSolutions = __FLT_MAX__;
		M_Intensification = false;
		M_Time_Intensification = 0;
		M_Challenge_Mode = false;
		M_NI_Colocadas = 0;
		M_Encuentra_Solutions = false;
		M_No_Sol = 0;
		M_Best_Integer_ForMax = __FLT_MAX__;
		M_Best_Integer_ForQ = __FLT_MAX__;
		M_Best_Integer_For = __FLT_MAX__;
		M_Mediana_Mejora =9999999;

	};


	virtual ~Maintenance() {};
	double DestruirCplex(double fobjAntiguo);
	double Get_Sol_I() { return M_Best_Integer_For; };
	double Get_Sol_Q() { return M_Best_Integer_ForQ; };
	double Get_Sol_M() { return M_Best_Integer_ForMax; };
	bool EncuentraSolutions() { return M_Encuentra_Solutions; };
	list<pair<vector<int>, double>>& GetPoolSolutions() { return M_Pool_Solutions_Integer; };
	list<pair<vector<int>, double>>& GetListaSolucionesInteger(int i) {
		return M_Pool_Solutions_Lista[i];
	};
	set<int> &GetDistintos(void)
	{
		return M_distintos;
	};
	set<int> &GetNoVisitadas(void)
	{
		return M_No_Visitadas;
	};
	int GetNDistintos(void)
	{
		return M_distintos.size();
	};
	void SetListaSolucionesInteger( list<pair<vector<int>, double>>& lista )
	{
		M_Pool_Solutions_Integer = lista;
	}

	void Set_Best_Solution(vector<int> &lista)
	{
		M_Best_I_T = lista;
	};
	int Get_Hilo(void) { return M_Hilo; };
	void Set_Hilo(int a) { M_Hilo = a; };
	void SetTotalTime(int a) { M_Total_Time = a; };
	void SetNumberOfThreads(int a) { M_Number_Threads = a; };
	void Set_Seed(int a) { M_seed = a; };
	void Set_MGapPool(int a) { M_GapPool = a; };
	void Set_ChallengeMode(bool a) { M_Challenge_Mode = a; };
	bool Get_ChallengeMode(void) { return M_Challenge_Mode; };
	double Get_BestFO1() { return M_Best_fo_1; };
	double Get_BestFO2() { return M_Best_fo_2; };
	int Get_No_Sol() { return M_No_Sol; };
	double QuitarT(double fobjAntiguo);
	double TerminarSolutionPrimeraCopy(double val);
	double VNS(double);
	double VND2(double& val);
	//Set
	bool CompararDosSumasT(vector<bool>& Cambios, vector<double>& SumaT1, vector<double>& SumaT2);

	void Set_NThreads(int a) { M_NThreads = a; };
	void Set_Id(int a) { M_Id = a; };
	void Set_T(int a) { M_T = a; };
	void Set_TipoBusqueda(int a) { M_Tipo_Busqueda = a; };
	void Set_Mixed_Risk(bool a) { M_Mixed_Risk = a; };
	void Set_Formulacion_Cuantiles(bool a) { M_Formulacion_Cuantiles = a; };
	void Set_Quantile(double a) { M_Quantile = a; };
	void Set_Alpha(double a) { M_Alpha = a; };
	void Set_NResources(int a) { M_N_Resources = a; };
	void Set_NInterventions(int a) { M_N_Interventions = a; };
	void Set_NExclusions(int a) { M_N_Exclusions = a; };
	void Set_Resources(vector<Resource>& a) { M_Resources = a; };
	void Set_Interventions(vector<Intervention>& a) { M_Interventions = a; };
	void Set_Seasons(vector<Season>& a) { M_Seasons = a; };
	void Set_Scenarios_Number(vector<int>& a) { M_Scenarios_number = a; };
	void Set_Exclusions(list< vector< int> >& a) { M_Exclusions = a; };
	void Set_Time_Formulation(int a) { M_Time_Modelo = a; };
	void Set_Time_Leer(double a) { M_Time_Leer = a; };
	void Set_Max_Riesgo(double a) { M_Max_Riesgo = a; };
	void Set_Max_Formulation(bool a) { M_Max_Formulation = a; };
	void Set_Min_Formulation(bool a) { M_Min_Formulation = a; };
	void Set_Time_Grasp(int a) { M_ComputationTime_Grasp = a; };
	void Set_Pool_Solutions(bool a) { M_Pool_Solutions = a; };
	void Insertar(list<pair<vector<int>, double>>& lista);
	void ClearPool() { M_Pool_Solutions_Integer.clear(); };
	inline void CopyFO2()
	{
		M_vfo2_temp_Copy = M_vfo2_temp;
		M_fo2_temp_Copy = M_fo2_temp;
	};
	inline void CopyBackFO2()
	{
		M_vfo2_temp = M_vfo2_temp_Copy;
		M_fo2_temp = M_fo2_temp_Copy;
	}
	inline void CopyFO2_2()
	{
		M_vfo2_temp_Copy2 = M_vfo2_temp;
		M_fo2_temp_Copy2 = M_fo2_temp;
	};
	void Insertar_Distintos(set<int> &lista) 
	{
		M_distintos.insert(lista.begin(), lista.end());
	}
	inline void CopyBackFO2_2()
	{
		M_vfo2_temp = M_vfo2_temp_Copy2;
		M_fo2_temp = M_fo2_temp_Copy2;
	}
	bool compareRisk(int i, int j)
	{
		if ((M_Interventions[i].Get_RiskC(M_I_T[i]) - M_Interventions[i].Get_Min_Risk()) > (M_Interventions[j].Get_RiskC(M_I_T[j]) - M_Interventions[j].Get_Min_Risk()))
			return true;
		else
			return (M_Interventions[i].Get_Min_Risk() > M_Interventions[j].Get_Min_Risk());

	}
	inline void Copy(bool lista_temp = true)
	{
		M_Waste_Copy = M_Waste;
		M_I_T_Copy = M_I_T;
		M_T_Temp_Risk_S_Copy = M_T_Temp_Risk_S;
		M_NI_Colocadas_Copy = M_NI_Colocadas;
		M_fo_temp_Copy = M_fo_temp;
		M_vfo2_temp_Copy = M_vfo2_temp;
		for (register int i = 0; i < M_N_Interventions; i++)
		{
			M_Interventions_Colocada_Copy[i] = M_Interventions_Colocada[i];
			M_I_Seasons_Copy[i] = M_I_Seasons[i];
			M_Interventions_T_Copy[i] = M_Interventions_T[i];

			M_Interventions[i].Copy(lista_temp);
		}
		for (int j = 0; j < M_N_Resources; j++)
		{
			M_Resources[j].Copy();
		}
	};
	void QuitarNoVisitadas(set<int>& vec);
	inline void CopyBack(bool lista_temp = true)
	{
		M_Waste = M_Waste_Copy;
		M_I_T = M_I_T_Copy;
		M_T_Temp_Risk_S = M_T_Temp_Risk_S_Copy;
		M_fo_temp = M_fo_temp_Copy;
		M_vfo2_temp = M_vfo2_temp_Copy;
		M_NI_Colocadas = M_NI_Colocadas_Copy;
		for (register int i = 0; i < M_N_Interventions; i++)
		{
			M_Interventions_Colocada[i] = M_Interventions_Colocada_Copy[i];
			M_I_Seasons[i] = M_I_Seasons_Copy[i];
			M_Interventions_T[i] = M_Interventions_T_Copy[i];
			if (M_I_T_Copy[i] >= 0)
				M_Interventions[i].Set_T_Temp(M_I_T_Copy[i]);
			M_Interventions[i].CopyBack(lista_temp);
		}
		for (int j = 0; j < M_N_Resources; j++)
		{
			M_Resources[j].CopyBack();
		}
	};



	inline void Copy2(bool lista_temp = true)
	{
		M_Waste_Copy2 = M_Waste;
		M_T_Temp_Risk_S_Copy2 = M_T_Temp_Risk_S;
		M_NI_Colocadas_Copy2 = M_NI_Colocadas;
		M_I_T_Copy2 = M_I_T;
		M_fo_temp_Copy2 = M_fo_temp;
		M_vfo2_temp_Copy2 = M_vfo2_temp;
		for (register int i = 0; i < M_N_Interventions; i++)
		{
			M_Interventions_Colocada_Copy2[i] = M_Interventions_Colocada[i];
			M_I_Seasons_Copy2[i] = M_I_Seasons[i];
			M_Interventions_T_Copy2[i] = M_Interventions_T[i];
			M_Interventions[i].Copy2(lista_temp);
		}
		for (int j = 0; j < M_N_Resources; j++)
		{
			M_Resources[j].Copy2();
		}
	};
	inline void CopyBack2(bool lista_temp = true)
	{

		M_Waste = M_Waste_Copy2;
		M_fo_temp = M_fo_temp_Copy2;
		M_vfo2_temp = M_vfo2_temp_Copy2;
		M_NI_Colocadas = M_NI_Colocadas_Copy2;
		M_T_Temp_Risk_S = M_T_Temp_Risk_S_Copy2;
		M_I_T = M_I_T_Copy2;
		for (register int i = 0; i < M_N_Interventions; i++)
		{
			M_Interventions_Colocada[i] = M_Interventions_Colocada_Copy2[i];
			M_I_Seasons[i] = M_I_Seasons_Copy2[i];
			M_Interventions_T[i] = M_Interventions_T_Copy2[i];
			if (M_I_T_Copy2[i] >= 0)
				M_Interventions[i].Set_T_Temp(M_I_T_Copy2[i]);
			M_Interventions[i].CopyBack2(lista_temp);
		}
		for (int j = 0; j < M_N_Resources; j++)
		{
			M_Resources[j].CopyBack2();
		}
	};
	inline void Copy3(bool lista_temp = true)
	{
		M_Waste_Copy3 = M_Waste;
		M_T_Temp_Risk_S_Copy3 = M_T_Temp_Risk_S;
		M_NI_Colocadas_Copy3 = M_NI_Colocadas;
		M_I_T_Copy3 = M_I_T;
		M_fo_temp_Copy3 = M_fo_temp;
		M_vfo2_temp_Copy3 = M_vfo2_temp;
		for (register int i = 0; i < M_N_Interventions; i++)
		{
			M_Interventions_Colocada_Copy3[i] = M_Interventions_Colocada[i];
			M_I_Seasons_Copy3[i] = M_I_Seasons[i];
			M_Interventions_T_Copy3[i] = M_Interventions_T[i];
			M_Interventions[i].Copy3(lista_temp);
		}
		for (int j = 0; j < M_N_Resources; j++)
		{
			M_Resources[j].Copy3();
		}
	};
	inline void CopyBack3(bool lista_temp = true)
	{

		M_Waste = M_Waste_Copy3;
		M_fo_temp = M_fo_temp_Copy3;
		M_vfo2_temp = M_vfo2_temp_Copy3;
		M_NI_Colocadas = M_NI_Colocadas_Copy3;
		M_T_Temp_Risk_S = M_T_Temp_Risk_S_Copy3;
		M_I_T = M_I_T_Copy3;
		for (register int i = 0; i < M_N_Interventions; i++)
		{
			M_Interventions_Colocada[i] = M_Interventions_Colocada_Copy3[i];
			M_I_Seasons[i] = M_I_Seasons_Copy3[i];
			M_Interventions_T[i] = M_Interventions_T_Copy3[i];
			if (M_I_T_Copy3[i] >= 0)
				M_Interventions[i].Set_T_Temp(M_I_T_Copy3[i]);
			M_Interventions[i].CopyBack3(lista_temp);
		}
		for (int j = 0; j < M_N_Resources; j++)
		{
			M_Resources[j].CopyBack3();
		}
	};
	inline void CopyBest(void)
	{
		M_Best_I_T = M_I_T;
		M_fo_temp_Best = M_fo_temp;
		for (register int i = 0; i < M_N_Interventions; i++)
		{
			M_Interventions_Colocada_Best[i] = M_Interventions_Colocada[i];
			M_I_Seasons_Best[i] = M_I_Seasons[i];
			M_Interventions_T_Best[i] = M_Interventions_T[i];
			M_Interventions[i].CopyBest();
		}
		for (int j = 0; j < M_N_Resources; j++)
		{
			M_Resources[j].CopyBest();
		}
	};
	inline void CopyBackBest(void)
	{
		M_fo_temp = M_fo_temp_Best;
		M_I_T = M_Best_I_T;
		for (register int i = 0; i < M_N_Interventions; i++)
		{
			M_Interventions_Colocada[i] = M_Interventions_Colocada_Best[i];
			M_I_Seasons[i] = M_I_Seasons_Best[i];
			M_Interventions_T[i] = M_Interventions_T_Best[i];
			if (M_Interventions_T_Best[i] >= 0)
				M_Interventions[i].Set_T_Temp(M_Interventions_T_Best[i]);
			M_Interventions[i].CopyBackBest();
		}
		for (int j = 0; j < M_N_Resources; j++)
		{
			M_Resources[j].CopyBackBest();
		}
	};


	void CalcularSumasT(vector<double>& SumaT, vector<int>& sol);
	//Get
	vector<int>& Get_Best_Solution() { return M_Best_I_T; };
	int Get_Total_Scenarios() { return M_Total_Scenarios; };
	int Get_Id(void) { return M_Id; };
	int Get_Iter(void) { return M_Iter; };
	void Set_Iter(int a) { M_Iter=a; };
	void Set_Iter_Grasp(int a) { M_Iter_Grasp=a; };
	double Get_Best_Obj() { return M_Best_fo; };
	double Get_Cota() { return M_Cota; };
	timeb Get_Time_Initial() { return M_Time_Initial; };
	void Set_Time_Initial(timeb a) { M_Time_Initial = a; };
		void Set_Iter_Formulation(int a) { M_Iter_Formulation = a; };
	double Get_Max_Min_Risk() { return M_Max_Min_Riesgo; };
	int Get_T(void) { return M_T; };
	int Get_Best_Iter() { return M_Best_Iter; };
	double Get_Quantile(void) { return M_Quantile; };
	double Get_Alpha(void) { return M_Alpha; };
	int Get_NResources(void) { return M_N_Resources; };
	int Get_NInterventions(void) { return M_N_Interventions; };
	int Get_NExclusions(void) { return M_N_Exclusions; };
	double Get_Sol_Formulation() { return M_sol_formulation; };
	void Set_fo_Formulation2(double a) { M_fo_formulation1 = a; };
	double Get_Obj_Formulation1() { return M_fo_formulation1; };
	double Get_Obj_Formulation2() { return M_fo_formulation2; };
	double Get_Time_Formulation1() { return M_Time_Modelo1; };
	double Get_Time_Formulation2() { return M_Time_Modelo2; };
	double Get_Time_Formulation3() { return M_Time_Modelo3; };
	vector<Resource>& Get_Resources(void) { return M_Resources; };
	vector<Intervention>& Get_Interventions(void) { return M_Interventions; };
	vector<Season>& Get_Seasons(void) { return M_Seasons; };
	vector<int>& Get_Scenarios_Number(void) { return M_Scenarios_number; };
	list< vector< int> >& Get_Exclusions(void) { return M_Exclusions; };
	//

	void CalcularSumasT(vector<double>& SumaT);

	int AleatorizarIgualesListInterventions(int tipo);
	void CopyListInterventionsTemp();
	bool PuedoPonerSola(Intervention& I, int t);
	void leer_problem(string& name);
	void leer_problem_wojson(string& name);
	void leer_problem_wojsonOLD(string& name);
	void leer_problem_wojsonFast(string& name_file);
	int Get_Sol_Pool() { return M_Pool_Solutions_Integer.size(); };
	int Get_Sol_PoolQ() { return M_Pool_Solutions_IntegerQuantiles.size(); };
	int Get_Sol_PoolM() { return M_Pool_Solutions_IntegerMax.size(); };
	void PrintTimes(string& name);
	void Problema(int);
	int Min(vector<double>&);
	int Max(vector<double>&);
	void VectoresOrdenados(void);
	void PrintSolution(string& name);
	void InsertarOrdenado(vector<pair<int, double> >& Vec, int pos, double val, bool decreciente = true);
	void InsertarOrdenadoTRisk(int, double val, bool creciente = false);
	void PrintValores();
	void PrintResourcesT(int);
	void PrintResources();
	void LimpiarVectores();
	//Algorithm
	bool VerifySolution2();
	void Constructive(int);
	double FuncionObjetivo();
	double FuncionObjetivo(vector<int> Interventions_T);
	bool ColocarEnTiempo(int n_int);
	bool ColocarEnTiempo(int n_int, int t);
	bool Exclusion(int tiempo, int nint);
	bool VerifySolution();
	bool VerifySolution(list<pair<int, int>> sol);
	void GRASP();
	void SetVND(bool a) { M_VND = a; }
	bool ConstructiveR(int tipo);
	bool ConstructiveRCL(int tipo);
	bool ColocarEnTiempoR(int n_int);
	bool PuedoQuitar(int n_int, int t);
	bool ConstructiveRValor(int tipo);
	bool ColocarEnTiempoRValor(int n_int);
	bool DestruirYReparar(void);
	int IndiceTipo(int type, int index);
	void CopyListInterventionsTempBias();
	void CopySolution(void);
	void ModificarRiskTemp(int, int);
	int Pos(int val, vector<pair<int, double>>& Vector);
	void CotaPrimeraParte();
	bool ColocarEnTiempoRGreedy(int n_int);
	void ColocarIEnTiempoValor(int n_int);
	void BusquedaLocalIntercambio();
	bool QuitarInterventionParaMeter(int Nint, int time);
	double RepararSolutionCopy();
	int Get_Lugar(){return M_Lugar;};
	bool ActualizarMejorSolution(double val,int lugar);
	double MejoraQuitarT(double fobjAntiguo);
	int BuscarIntervention(int time, int resource);
	void EjecutarValidadorPython(void);
	void PrintPSolutionStatistics(vector<int>& lista);
	void Borrar(int nint, int t);
	bool ConstructiveRXT(int tipo);
	int BuscarIntervention(int time, double threshold);
	bool PuedoColocar(int nint, int t);
//	void ColocarIEnTiempo(int n_int, int t);
	bool ConstructiveRA(int tipo);
	void ModificarRiskTemp(int Int);
	void ModificarRiskTempAll(int ,int);
	void ComprobarRiskTemp(int Int);
	double RepararSolutionCopyA(double );
	bool ConstructiveRBias(int tipo);
	double RellenarSolutionA(int type);
	double RellenarSolutionAv2(int type);
	double MejoraIntercambiosMejorT();
	bool MejoraIntercambiosFo2();
	bool MejoraIntercambiosFo1Order();

	inline void AddScenarios(int Nint);
	inline void SubtractScenarios(int Nint);

	void CalcularMaximosEscenario();
	void PrintSolution(string& name, vector<int>& lista);
	void PrintSolutionStatistics(string& name, vector<int>& lista);
	double RellenarSolution(int type);
	bool RepararSolutionBestT();
	int ElegirMayor(int type);
	int ElegirMayorAleatorizado(int type);
	int ElegirMenorAleatorizadoSPG(int type);
	int ElegidoMayorDif(void);
	void QuitarRecursos(int n_int);
	void AddRecursos(int n_int);
	void ColocarIEnBestTiempo(int n_int);
	void ColocarIEnTiempo(int n_int, int telegido);
	void PruebaHash();
	int  hash_operator(vector<int> & vec)
	{

//		string oss(vec.begin(), vec.end());

//		return hash<std::string>{}(oss);

//		return get_random(0, 11111111111);
		int seed = vec.size();
		for (auto& i : vec) {
			seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		}
		return seed;
	};
	//Cplex operator
	int IntegerFormulation(int Val);
	void EjecutarValidadorPython(vector<int> & );
	void LimpiarRiskTempS();
	void LimpiarRiskTempS2();
	double FuncionObjetivoTemp();
	double FuncionObjetivoTemp( int ti1, int tf1);
	double FuncionObjetivoTemp(int ,int ti1,int tf1);
	int ElegirMayorTFuncionObjetivo(int num_int);
	int ElegirMayorAleatorizadoSPG(int type);
	double SegundaParteFObj(int numero_i, int timeini, int timefin,double );
	void ChapuzaDiversidad(string& file_to_read);
	void CambiarSolution(int inter, int time);
	void PonerAlFinalSolution(int inter, int time);
	void leer_problem_wojson2(string& name_file);
	int ElegirMenor(int type);
	//Para el beam search
	void BeamSearch();
	bool InsertPartialSolution();
	void InsertClassical();
	void SelectByGlobalObjectiveFunction();
	void FinishSolutions();
	void RecoverPartialSolution(int i);
	bool PerturbarSolution();
	bool MejoraIntercambiosFo1Best();
	void ColocarIEnTiempoSPG(int n_int);
	double MejoraIntercambiosEjection(int );
	bool MejoraIntercambiosFo1OrderFast();
	bool MejoraIntercambiosFo1Fast();
	double SimpleTabuMejorT();
	void InsertarOrdenadoPool(double val, vector<int> sol, list<pair<vector<int>, double>>& lista);

	double MejoraQuitar(double );
	int ElegirMayorAbsoluto(int type);
	int ElegirMayorTFuncionObjetivoAbsoluto(int num_int);
	double TerminarSolutionCopy(double);
	int ElegirMenorAbsoluto(int type);
	double MejoraQuitarF2(int Type);
	inline void AddScenarios(int Nint, int t_ini, int t_fin);
	inline void SubtractScenarios(int Nint, int t_ini, int t_fin);
	double PerturbarSolution(double fobjAntiguo);
	bool comp_descending1(int i, int j);
	double MejoraIntercambiosDosI(double);
	double VND(double &val);
	void SetIntensification(bool a) { M_Intensification = a; };
	void MejorarPoolSolutions();
	void VerificarRiskMap();
	double PathRelinkingDeAaB(vector<int>& A, vector<int>& B);
	void ComputeExcessTemp();
	double IntegerFormulationPartial(int Val);
	double MejoraQuitarCplex(double fobjAntiguo);
	void Formulations(int a);
	void MejorarPoolSolutions2(int i);
	void CrearListasFormulations();
	void PoolGoodSolutions(vector<int>&, double );
	double RepararCplex(double fobjAntiguo);
	bool HaCambiadoT(int time, int duracion, vector<bool> &TiemposCambiado);
		
};



#endif // MAINTENANCE_H
